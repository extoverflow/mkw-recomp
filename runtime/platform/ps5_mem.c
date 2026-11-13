// PS5 implementation of platform.h, virtual memory, faults and crashes. linux_mem.c (8cbbfe1) keeps the contract; the
// mechanism is the console's (Gate 0, verdict 1-8):
// - ranges are reserved with sceKernelReserveVirtualRange (an anonymous PROT_NONE mapping would be charged to the
//   ~442 MiB flexible-memory budget of a title);
// - memory is direct memory mapped over a reservation, whole 16 KiB kernel pages, and only when first touched: a commit
//   only records the state, and the fault handler backs a page at its first access with zeroed memory, as Linux's
//   kernel backs anonymous memory (plat_mem_alloc's sparse tables and an 8 GiB heap cost only what is touched);
// - the guest's state is kept per 4 KiB page (pstate, as on Linux) and applied per 16 KiB page as the strictest
//   protection among its committed 4 KiB pages; an access that its own 4 KiB page allows but a stricter sibling
//   blocks goes to the fault hook for that sibling (a frozen page thaws: counted as written, never missed);
// - write watch in software, per 16 KiB page (Linux has the kernel's asynchronous userfaultfd, Windows GetWriteWatch,
//   both per 4 KiB page): a watched page is writable only while all four of its 4 KiB pages count as written; the
//   first write after a reset faults, marks all four (a superset: which of them the later writes hit is not seen) and
//   opens the page. A query reports the marked 4 KiB pages; a reset unmarks those reported and closes the page again,
//   so a write to any of them faults again;
// - no populated-page scan (UINT64_MAX: as Windows).
// Code map and divergences: kept outside this repo.
#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <ucontext.h>
#include <unistd.h>

#include "platform.h"
#include "ps5_int.h"

int64_t sceKernelGetDirectMemorySize(void);
int32_t sceKernelAllocateDirectMemory(int64_t start, int64_t end, size_t len, size_t align, int type,
                                     int64_t *phys_out);
int32_t sceKernelReleaseDirectMemory(int64_t start, size_t len);
int32_t sceKernelMapDirectMemory(void **addr, size_t len, int prot, int flags, int64_t phys, size_t align);
int32_t sceKernelReserveVirtualRange(void **addr, size_t len, int flags, size_t align);

#define PG 0x1000ull                 // the guest's page
#define KP 0x4000ull                 // the kernel's page (Gate 0)
#define SUB (KP / PG)                // guest pages per kernel page
#define PS_LIMIT (1ull << 38)        // host.h RT_ADDRESS_LIMIT: the guest's addresses
#define CHUNK (1ull << 30)           // pstate and the backing records are made per GiB, on first use
#define NCHUNK (PS_LIMIT / CHUNK)
#define NO_REPLACE 0x80              // with MAP_FIXED: fail on an occupied range (MAP_EXCL is ignored; Gate 0)
#define DMEM_TYPE 12                 // write-back memory, as Gate 0 and Gate 1 used
#define BLOCK (2ull << 20)           // backed at once where a whole aligned block is committed read-write
enum { PS_FREE, PS_RESERVED, PS_NONE, PS_READ, PS_RW };   // PS_NONE.. are committed: PLAT_PROT_* + PS_NONE

#define WW_ALL ((1u << SUB) - 1)     // ww[]: bits 0-3, one per 4 KiB page written since its last reset
#define WW_PEND_SHIFT SUB            // ww[]: bits 4-7, reported and to unmark once the page is closed (a reset)
#define WW_ON (1u << (2 * SUB))      // ww[]: the 16 KiB page lies in a watched reservation

typedef struct {
    uint8_t state[CHUNK / PG];       // per 4 KiB page
    uint16_t ww[CHUNK / KP];         // per 16 KiB page: WW_ON, the written 4 KiB pages, those being unmarked
    uint8_t backed[CHUNK / KP / 8];  // per 16 KiB page: direct memory mapped there
    int64_t *phys;                   // per 16 KiB page: its direct memory; made on the first backing in the chunk
} Chunk;

static Chunk *chunks[NCHUNK];
static PlatMutex vm_lock = PLAT_MUTEX_INIT;

// Lazily backed ranges outside the guest's addresses (plat_mem_alloc): the fault handler backs them like committed
// read-write guest pages. Their backing is not released (kept until exit, as the contract says).
#define LAZY_MAX 64
static struct { uint64_t lo, hi; } lazy[LAZY_MAX];
static int nlazy;

static Chunk *chunk_of(uint64_t a, int make) {
    if (a >= PS_LIMIT) return NULL;
    Chunk **c = &chunks[a / CHUNK];
    if (!*c && make) *c = calloc(1, sizeof **c);
    return *c;
}

static int st_get(uint64_t a) {
    Chunk *c = chunk_of(a, 0);
    return c ? c->state[(a % CHUNK) / PG] : PS_FREE;
}

static void st_set(uint64_t a, uint64_t n, uint8_t v) {
    for (uint64_t end = a + n; a < end;) {
        Chunk *c = chunk_of(a, 1);
        uint64_t stop = (a / CHUNK + 1) * CHUNK < end ? (a / CHUNK + 1) * CHUNK : end;
        if (c) memset(c->state + (a % CHUNK) / PG, v, (size_t)((stop - a + PG - 1) / PG));
        a = stop;
    }
}

static int is_backed(uint64_t k) {
    Chunk *c = chunk_of(k, 0);
    size_t i = (k % CHUNK) / KP;
    return c && (c->backed[i / 8] >> (i % 8) & 1);
}

static uint16_t *ww_of(uint64_t k) {
    Chunk *c = chunk_of(k, 0);
    return c ? &c->ww[(k % CHUNK) / KP] : NULL;
}

// A watched 16 KiB page not wholly marked written stays closed to writes, so the next write is seen.
static int ww_closed(uint64_t k) {
    uint16_t *w = ww_of(k);
    return w && (*w & WW_ON) && (*w & WW_ALL) != WW_ALL;
}

// The fault on a write to a watched page: all four marked written, the page may open (vm_lock held).
static void ww_mark(uint64_t k) {
    uint16_t *w = ww_of(k);
    if (w && (*w & WW_ON)) *w |= WW_ALL;
}

static int prot_of(int ps) { return ps == PS_RW ? PROT_READ | PROT_WRITE : ps == PS_READ ? PROT_READ : PROT_NONE; }

// The protection a backed 16 KiB page gets: the strictest of its committed 4 KiB pages (none committed: none).
static int kpage_prot(uint64_t k) {
    int best = PS_RW + 1;
    for (uint64_t i = 0; i < SUB; i++) {
        int s = st_get(k + i * PG);
        if (s >= PS_NONE && s < best) best = s;
    }
    int prot = best > PS_RW ? PROT_NONE : prot_of(best);
    return ww_closed(k) ? prot & ~PROT_WRITE : prot;
}

static void apply(uint64_t k) {
    if (is_backed(k)) mprotect((void *)(uintptr_t)k, KP, kpage_prot(k));
}

// Maps zeroed direct memory over [k, k + n) (reserved kernel pages, none backed), one allocation; the pages are left
// read-write. 0 when memory is gone.
static int back_run(uint64_t k, uint64_t n) {
    int64_t phys = 0;
    void *at = (void *)(uintptr_t)k;
    size_t align = n >= BLOCK ? BLOCK : KP;
    if (sceKernelAllocateDirectMemory(0, sceKernelGetDirectMemorySize(), n, align, DMEM_TYPE, &phys)) return 0;
    if (sceKernelMapDirectMemory(&at, n, PROT_READ | PROT_WRITE, MAP_FIXED, phys, align) ||
        at != (void *)(uintptr_t)k) {
        sceKernelReleaseDirectMemory(phys, n);
        return 0;
    }
    memset(at, 0, n);   // direct memory is not cleared for us
    for (uint64_t o = 0; o < n; o += KP) {
        Chunk *c = chunk_of(k + o, 1);
        if (!c) continue;
        size_t i = ((k + o) % CHUNK) / KP;
        if (!c->phys) c->phys = calloc(CHUNK / KP, sizeof *c->phys);
        if (c->phys) c->phys[i] = phys + (int64_t)o;   // released per page later: a sub-range of the allocation
        c->backed[i / 8] |= (uint8_t)(1u << (i % 8));
    }
    return 1;
}

// The 16 KiB page k backed with zeroed direct memory and given prot. Where the whole aligned 2 MiB block around it is
// committed read-write and unbacked (a heap's commit), the block is backed at once: one allocation instead of 128
// faults (161 us per 16 KiB page each on its own, mem_test 13/11/2026).
static int back(uint64_t k, int prot) {
    uint64_t b = k & ~(BLOCK - 1);
    int whole = b + BLOCK <= PS_LIMIT;
    for (uint64_t a = b; a < b + BLOCK && whole; a += PG) whole = st_get(a) == PS_RW && !is_backed(a & ~(KP - 1));
    if (whole && back_run(b, BLOCK)) {
        // back_run leaves the block read-write: the watched pages not marked written are closed again (one call
        // where none of the block is marked, as after a heap's commit)
        int any_closed = 0, all_closed = 1;
        for (uint64_t a = b; a < b + BLOCK; a += KP) {
            int c = ww_closed(a);
            any_closed |= c, all_closed &= c;
        }
        if (all_closed) mprotect((void *)(uintptr_t)b, BLOCK, PROT_READ);
        else if (any_closed)
            for (uint64_t a = b; a < b + BLOCK; a += KP)
                if (ww_closed(a)) mprotect((void *)(uintptr_t)a, KP, PROT_READ);
        return 1;
    }
    if (!back_run(k, KP)) return 0;
    if (prot != (PROT_READ | PROT_WRITE)) mprotect((void *)(uintptr_t)k, KP, prot);
    return 1;
}

// Back to reserved: the page's direct memory released, the range reserved again in its place (Gate 0 verdict 4).
static void unback(uint64_t k) {
    Chunk *c = chunk_of(k, 0);
    size_t i = (k % CHUNK) / KP;
    if (!c || !(c->backed[i / 8] >> (i % 8) & 1)) return;
    void *at = (void *)(uintptr_t)k;
    sceKernelReserveVirtualRange(&at, KP, MAP_FIXED, KP);
    if (c->phys && c->phys[i]) sceKernelReleaseDirectMemory(c->phys[i], KP);
    if (c->phys) c->phys[i] = 0;
    c->backed[i / 8] &= (uint8_t)~(1u << (i % 8));
    c->ww[i] &= (uint16_t)~WW_ALL;   // its pages read zero again: nothing written (as Linux, unpopulated pages)
}

int plat_mem_reserve(uint64_t addr, uint64_t n, int watch) {
    if (!n || addr + n > PS_LIMIT) return 0;
    plat_mutex_lock(&vm_lock);
    int ok = 1;
    for (uint64_t a = addr; a < addr + n && ok; a += PG) ok = st_get(a) == PS_FREE;
    // The kernel's pages the range touches that nothing of ours holds yet: reserved there exactly, or refused.
    uint64_t lo = addr & ~(KP - 1), hi = (addr + n + KP - 1) & ~(KP - 1), done = lo;
    for (uint64_t k = lo; k < hi && ok; k += KP) {
        int ours = 0;
        for (uint64_t i = 0; i < SUB; i++) ours |= st_get(k + i * PG) != PS_FREE;
        if (ours) continue;
        void *at = (void *)(uintptr_t)k;
        if (sceKernelReserveVirtualRange(&at, KP, MAP_FIXED | NO_REPLACE, KP) || at != (void *)(uintptr_t)k) ok = 0;
        else done = k + KP;
    }
    if (ok) {
        st_set(addr, n, PS_RESERVED);
        for (uint64_t k = lo; k < hi && watch; k += KP) {
            uint16_t *w = ww_of(k);
            if (w) *w = WW_ON;
        }
    } else
        for (uint64_t k = lo; k < done; k += KP) {   // undo ours
            int ours = 0;
            for (uint64_t i = 0; i < SUB; i++) ours |= st_get(k + i * PG) != PS_FREE;
            if (!ours) munmap((void *)(uintptr_t)k, KP);
        }
    plat_mutex_unlock(&vm_lock);
    return ok;
}

void *plat_mem_alloc(size_t n) {
    void *at = NULL;
    n = (n + KP - 1) & ~(size_t)(KP - 1);
    if (!n || sceKernelReserveVirtualRange(&at, n, 0, KP)) return NULL;
    plat_mutex_lock(&vm_lock);
    int ok = nlazy < LAZY_MAX;
    if (ok) lazy[nlazy].lo = (uint64_t)(uintptr_t)at, lazy[nlazy].hi = (uint64_t)(uintptr_t)at + n, nlazy++;
    plat_mutex_unlock(&vm_lock);
    if (!ok) munmap(at, n);
    return ok ? at : NULL;
}

int plat_mem_commit(uint64_t addr, size_t n) {
    plat_mutex_lock(&vm_lock);
    st_set(addr, n, PS_RW);
    for (uint64_t k = addr & ~(KP - 1); k < addr + n; k += KP) apply(k);
    plat_mutex_unlock(&vm_lock);
    return 1;
}

int plat_mem_decommit(uint64_t addr, uint64_t n) {
    plat_mutex_lock(&vm_lock);
    st_set(addr, n, PS_RESERVED);
    for (uint64_t k = addr & ~(KP - 1); k < addr + n; k += KP) {
        if (!is_backed(k)) continue;
        int any = 0;
        for (uint64_t i = 0; i < SUB; i++) any |= st_get(k + i * PG) >= PS_NONE;
        if (!any) {
            unback(k);
            continue;
        }
        // Some of the 16 KiB page stays committed: the decommitted 4 KiB pages are zeroed in place.
        mprotect((void *)(uintptr_t)k, KP, PROT_READ | PROT_WRITE);
        for (uint64_t i = 0; i < SUB; i++) {
            uint64_t p = k + i * PG;
            if (p >= addr && p < addr + n) memset((void *)(uintptr_t)p, 0, PG);
        }
        apply(k);
    }
    plat_mutex_unlock(&vm_lock);
    return 1;
}

int plat_mem_protect(uint64_t addr, uint64_t n, int prot) {
    plat_mutex_lock(&vm_lock);
    st_set(addr, n, (uint8_t)(PS_NONE + prot));
    for (uint64_t k = addr & ~(KP - 1); k < addr + n; k += KP) apply(k);
    plat_mutex_unlock(&vm_lock);
    return 1;
}

// Through the protection: the kernel pages made read-write for the copy, under the VM lock (Gate 0 verdict 7; a
// thread writing a sealed sibling meanwhile is not stopped by the page then).
static int through(uint64_t addr, void *buf, size_t n, int write) {
    if (!n) return 1;
    plat_mutex_lock(&vm_lock);
    int ok = 1;
    for (uint64_t k = addr & ~(KP - 1); k < addr + n && ok; k += KP) {
        int committed = 0;
        for (uint64_t i = 0; i < SUB; i++) committed |= st_get(k + i * PG) >= PS_NONE;
        if (!committed && !is_backed(k)) ok = 0;
        else if (!is_backed(k) && !back(k, PROT_READ | PROT_WRITE)) ok = 0;
    }
    if (ok) {
        for (uint64_t k = addr & ~(KP - 1); k < addr + n; k += KP)
            mprotect((void *)(uintptr_t)k, KP, PROT_READ | PROT_WRITE);
        if (write) memcpy((void *)(uintptr_t)addr, buf, n);
        else memcpy(buf, (const void *)(uintptr_t)addr, n);
        for (uint64_t k = addr & ~(KP - 1); k < addr + n && write; k += KP) ww_mark(k);
        for (uint64_t k = addr & ~(KP - 1); k < addr + n; k += KP) apply(k);
    }
    plat_mutex_unlock(&vm_lock);
    return ok;
}

int plat_mem_poke(uint64_t addr, const void *src, size_t n) { return through(addr, (void *)src, n, 1); }
int plat_mem_peek(void *dst, uint64_t addr, size_t n) { return through(addr, dst, n, 0); }

int plat_mem_state(uint64_t addr, uint64_t limit, uint64_t *end) {
    if (addr >= PS_LIMIT) {
        *end = limit;
        return PLAT_MEM_FREE;
    }
    uint64_t lim = limit < PS_LIMIT ? limit : PS_LIMIT, a = addr & ~(PG - 1);
    int v = st_get(a);
    while (a + PG < lim && st_get(a + PG) == v) a += PG;
    *end = a + PG < lim ? a + PG : lim;
    return v == PS_FREE ? PLAT_MEM_FREE : v == PS_RESERVED ? PLAT_MEM_RESERVED : v - PS_NONE;
}

// Closes [run, run_end) to writes, then unmarks what plat_mem_written reported there (the pending bits).
static void ww_close_run(uint64_t run, uint64_t run_end, int prot) {
    if (run_end <= run) return;
    mprotect((void *)(uintptr_t)run, run_end - run, prot);
    for (uint64_t k = run; k < run_end; k += KP) {
        uint16_t *w = ww_of(k);
        if (w) *w = (uint16_t)(*w & (WW_ON | WW_ALL) & ~(*w >> WW_PEND_SHIFT & WW_ALL));
    }
}

// The committed 4 KiB pages marked written, in address order. reset: those reported are unmarked, and their 16 KiB
// pages closed to writes again. A range that is not wholly watched is untracked.
uint64_t plat_mem_written(uint64_t addr, uint64_t n, uint64_t *pages, uint64_t max, int reset) {
    if (!max || !n) return 0;
    if (addr + n > PS_LIMIT) return UINT64_MAX;
    uint64_t got = 0, end = addr + n, run = 0, run_end = 0;
    int run_prot = 0;
    for (uint64_t k = addr & ~(KP - 1); k < end; k += KP) {   // watched throughout, or untracked (set at reservation)
        uint16_t *w = ww_of(k);
        if (!w || !(*w & WW_ON)) return UINT64_MAX;
    }
    plat_mutex_lock(&vm_lock);
    for (uint64_t k = addr & ~(KP - 1); k < end && got < max; k += KP) {
        uint16_t *w = ww_of(k);
        if (!(*w & WW_ALL)) continue;
        uint8_t cleared = 0;
        for (uint64_t i = 0; i < SUB && got < max; i++) {
            uint64_t p = k + i * PG;
            if (p < addr || p >= end || !(*w >> i & 1) || st_get(p) < PS_NONE) continue;
            pages[got++] = p;
            cleared |= (uint8_t)(1u << i);
        }
        if (!reset || !cleared) continue;
        // Closed first, unmarked after: a write that lands before the close was reported now; one after it faults
        // and waits for vm_lock in ps5_fault, then marks the page again. Open pages are closed in runs of the same
        // protection (an mprotect costs about 36 us per call on the console, mem_test 13/11/2026), so the marks of a
        // run are cleared when the run is closed.
        int prot = kpage_prot(k) & ~PROT_WRITE;
        if ((*w & WW_ALL) == WW_ALL && is_backed(k)) {
            if (run_end != k || run_prot != prot) {
                ww_close_run(run, run_end, run_prot);
                run = k, run_prot = prot;
            }
            run_end = k + KP;
            *w |= (uint16_t)(cleared << WW_PEND_SHIFT);   // unmarked once the run is closed
            continue;
        }
        *w &= (uint16_t)~cleared;
    }
    ww_close_run(run, run_end, run_prot);
    plat_mutex_unlock(&vm_lock);
    return got;
}

uint64_t plat_mem_populated(uint64_t addr, uint64_t n, uint64_t (*runs)[2], uint64_t max) {
    (void)addr, (void)n, (void)runs;
    return max ? UINT64_MAX : 0;
}

// ---- faults: linux_mem.c's handler with the PS5's own cases first (a first touch, a stricter sibling)
static int (*fault_hook)(uint64_t addr);
static int (*watch_fn)(uint64_t addr, uint64_t rip);
static void (*crash_fn)(const PlatCrash *c);
static uint64_t sibling_lifts;   // accesses the strictest-of-four rule blocked and the hook did not clear (diagnosis)

void plat_mem_watch_hook(int (*fn)(uint64_t addr, uint64_t rip)) { watch_fn = fn; }

_Thread_local int plat_fault_write;
_Thread_local uint64_t plat_fault_pc, plat_fault_ret;

static int in_lazy(uint64_t a) {
    for (int i = 0; i < nlazy; i++)
        if (a >= lazy[i].lo && a < lazy[i].hi) return 1;
    return 0;
}

// 1 when the access at a (write or not) is one the PS5 mechanism caused and has now cleared: retry it.
static int ps5_fault(uint64_t a, int write) {
    uint64_t k = a & ~(KP - 1);
    if (in_lazy(a)) {
        plat_mutex_lock(&vm_lock);
        int ok = is_backed(k) || back(k, PROT_READ | PROT_WRITE);
        plat_mutex_unlock(&vm_lock);
        return ok;
    }
    int s = st_get(a);
    if (s < PS_NONE) return 0;   // not committed: a real fault
    plat_mutex_lock(&vm_lock);
    if (!is_backed(k) && !back(k, kpage_prot(k))) {
        plat_mutex_unlock(&vm_lock);
        return 0;
    }
    plat_mutex_unlock(&vm_lock);
    int own_ok = write ? s == PS_RW : s >= PS_READ;
    if (!own_ok) return 0;   // its own page forbids it: the hook's (a trap, a frozen page) or a crash
    int need = write ? PS_RW : PS_READ;
    for (uint64_t i = 0; i < SUB; i++) {   // the siblings that block it: their hooks first
        uint64_t p = k + i * PG;
        int ps = st_get(p);
        if (ps >= PS_NONE && ps < need && fault_hook) fault_hook(p);
    }
    plat_mutex_lock(&vm_lock);
    // A write to a watched page: all four of its 4 KiB pages marked written (the first write since the last reset
    // faulted here), under the same lock as the opening below, so a reset cannot come between them.
    if (write) ww_mark(k);
    int now = kpage_prot(k), allowed = write ? (now & PROT_WRITE) != 0 : (now & PROT_READ) != 0;
    if (!allowed) {   // a sibling stays stricter: the page opens for this access anyway (counted)
        mprotect((void *)(uintptr_t)k, KP, write ? PROT_READ | PROT_WRITE : PROT_READ);
        sibling_lifts++;
    } else {
        apply(k);   // the protection the state gives now (the write mark above opens a watched page)
    }
    plat_mutex_unlock(&vm_lock);
    return 1;
}

static void fault_handler(int sig, siginfo_t *si, void *uc) {
    ucontext_t *u = uc;
    long err = (long)u->uc_mcontext.mc_err;   // x86's page fault error code: 2 a write, 16 an instruction fetch
    uint64_t a = (uint64_t)(uintptr_t)si->si_addr;
    int segv = sig == SIGSEGV || sig == SIGBUS;   // FreeBSD reports a protection fault as SIGBUS
    if (segv && watch_fn && (err & 2) && watch_fn(a, (uint64_t)u->uc_mcontext.mc_rip)) return;
    plat_fault_write = (err & 2) != 0, plat_fault_pc = (uint64_t)u->uc_mcontext.mc_rip;
    plat_fault_ret = *(const uint64_t *)(uintptr_t)u->uc_mcontext.mc_rsp;
    if (segv && !(err & 16) && ps5_fault(a, (err & 2) != 0)) return;
    if (segv && fault_hook && fault_hook(a)) return;
    uintptr_t rip = (uintptr_t)u->uc_mcontext.mc_rip;
    Dl_info at = {0}, self = {0};
    int in_module = dladdr((void *)rip, &at) != 0, ours = 0;
    if (in_module && dladdr((void *)plat_crash_hook, &self)) ours = at.dli_fbase == self.dli_fbase;
    if (crash_fn && (!in_module || ours)) {
        PlatCrash c = {sig == SIGSEGV ? "access violation" : sig == SIGBUS ? "bus error"
                       : sig == SIGILL ? "illegal instruction" : "arithmetic exception",
                       (uint32_t)sig, rip, in_module ? rip - (uintptr_t)at.dli_fbase : rip, -1, 0,
                       !in_module ? "no module" : "mkw", 0};
        uintptr_t sp = (uintptr_t)u->uc_mcontext.mc_rsp;
        if (!in_module && sp && !(sp & 7) && plat_mem_readable(sp, 8)) c.ret = *(const uintptr_t *)sp;
        if (segv) c.access = (err & 16) ? 8 : (err & 2) ? 1 : 0, c.addr = a;
        crash_fn(&c);
    } else {
        char line[256];
        int n = snprintf(line, sizeof line, "[host] fatal signal %d at %#lx (%s+%#lx) addr %p err %#lx\n", sig,
                         (unsigned long)rip, at.dli_fname ? at.dli_fname : "?",
                         (unsigned long)(rip - (uintptr_t)at.dli_fbase), si->si_addr, (unsigned long)err);
        if (n > 0) (void)!write(2, line, (size_t)n < sizeof line ? (size_t)n : sizeof line - 1);
    }
    signal(sig, SIG_DFL);   // the faulting instruction runs again and gets the default action
}

static void fault_install(void) {
    static int done;
    if (done) return;
    done = 1;
    alt_stack();
    struct sigaction sa = {.sa_sigaction = fault_handler, .sa_flags = SA_SIGINFO | SA_ONSTACK | SA_NODEFER};
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGBUS, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
}

void plat_mem_fault_hook(int (*fn)(uint64_t addr)) {
    fault_hook = fn;
    fault_install();
}

void plat_crash_hook(void (*fn)(const PlatCrash *c)) {
    crash_fn = fn;
    fault_install();
}

// The kernel's handling is needed for memory.c's first fault on a page too, so the handler is installed on the first
// reservation as well, hook or not.
__attribute__((constructor)) static void ps5_mem_init(void) { fault_install(); }

// A title cannot exec itself (Linux execv("/proc/self/exe")); the caller's reason is already on stderr.
void plat_reexec(void) {
    fputs("FATAL: the PS5 cannot run this program again in place (plat_reexec)\n", stderr);
    plat_exit(70);
}

// From the state, without touching the memory: a guest page is readable when committed and readable (an untouched
// one reads zero); outside the guest's addresses, mincore tells whether anything is mapped there.
int plat_mem_readable(uint64_t addr, uint64_t n) {
    uint64_t end = addr + (n ? n : 1);
    for (uint64_t a = addr & ~(PG - 1); a < end; a += PG) {
        if (a < PS_LIMIT && st_get(a) != PS_FREE) {
            if (st_get(a) < PS_READ) return 0;
            continue;
        }
        if (in_lazy(a)) continue;
        char v;
        if (mincore((void *)(uintptr_t)(a & ~(KP - 1)), KP, &v)) return 0;
    }
    return 1;
}

int plat_mem_read(uint64_t addr, void *out, size_t n) {
    if (!plat_mem_readable(addr, n)) return 0;
    if (addr < PS_LIMIT && st_get(addr) != PS_FREE) return plat_mem_peek(out, addr, n);
    memcpy(out, (const void *)(uintptr_t)addr, n);
    return 1;
}

void *plat_aligned_alloc(size_t size, size_t align) {
    void *p = NULL;
    return posix_memalign(&p, align < sizeof(void *) ? sizeof(void *) : align, size) ? NULL : p;
}

void plat_aligned_free(void *p) { free(p); }

// Fresh read-write pages over a range of a reservation (vk_memory.c undoing a placed map): whatever was mapped there
// goes, the pages are committed read-write and backed again at their first touch.
int plat_mem_replace_anon(uint64_t addr, uint64_t n) {
    plat_mutex_lock(&vm_lock);
    for (uint64_t k = addr & ~(KP - 1); k < addr + n; k += KP) {
        if (is_backed(k)) unback(k);
        else {
            void *at = (void *)(uintptr_t)k;
            sceKernelReserveVirtualRange(&at, KP, MAP_FIXED, KP);
        }
    }
    st_set(addr, n, PS_RW);
    plat_mutex_unlock(&vm_lock);
    return 1;
}

uint64_t ps5_mem_sibling_lifts(void) { return __atomic_load_n(&sibling_lifts, __ATOMIC_RELAXED); }
