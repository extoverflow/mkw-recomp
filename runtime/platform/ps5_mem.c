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
int sceKernelVirtualQuery(const void *addr, int flags, void *info, size_t size);
// The process's CPU and GPU page tables, in use and free (libkernel; read the same way in EglantinePS5
// src/host_memory.cpp:509-516). A 16 KiB mapping in a 2 MiB stretch with none needs one: the console's cap on them
// (--ps5-map-test (3), 13/11/2026) is what guest backing ran into.
int sceKernelGetPageTableStats(int *cpu_total, int *cpu_free, int *gpu_total, int *gpu_free);
// sceKernelVirtualQuery's entry, as PS5_Vulkan's tooling reads it (tooling/gpu-broker/client.c:54).
typedef struct { void *start, *end; uint64_t offset; int32_t protection, type; uint8_t flags; char name[32];
                 uint8_t padding[7]; } VqEntry;

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
// Hot pages (MKW_PS5_WW_HOT=1; off by default since two silent crashes, below): a page reported written by WW_HOT_AFTER resets in a row (the game rewrites
// its per-frame buffers every frame) stays open and is reported written by every query instead of being closed and
// faulting again: each such cycle cost a signal, the lock and two mprotects (race, run 42: 1.18 M write-watch faults,
// 117 us each with the lock waits; mprotect 36 us). A report may hold more pages than were written, never fewer. Every
// WW_HOT_AGE resets a hot page is closed again, so pages the game stopped writing go back to being watched.
// Off by default: a report is a generation stamp ("the CPU wrote here"), and the shadows copy stamped pages into the
// GPU's copy at each submit and write the GPU's results back; a hot page stamped at every query can let an older guest
// copy overwrite fresh GPU output (runs 45 and 48 ended with the console's "Game or App Error" CE-108255-1 near a race
// start, no FATAL; none in five race runs without hot pages). Needs pages the GPU writes kept out before it is on.
#define WW_HOT (1u << (2 * SUB + 1))  // ww[]: bit 9, the page is hot (open, always reported written)
#define WW_SC_SHIFT (2 * SUB + 2)     // ww[]: bits 10-15, resets in a row that found it written; when hot, its age
#define WW_SC_MASK (63u << WW_SC_SHIFT)
#define WW_HOT_AFTER 3
#define WW_HOT_AGE 32

typedef struct {
    uint8_t state[CHUNK / PG];       // per 4 KiB page
    uint16_t ww[CHUNK / KP];         // per 16 KiB page: WW_ON, the written 4 KiB pages, those being unmarked
    uint8_t backed[CHUNK / KP / 8];  // per 16 KiB page: direct memory mapped there
    int64_t *phys;                   // per 16 KiB page: its direct memory; made on the first backing in the chunk
    uint8_t gpu[CHUNK / KP];         // per 16 KiB page: imports covering it (ps5_mem_gpu_share/unshare); not given back while >0
    uint8_t zseen[CHUNK / KP / 8];   // per 16 KiB page: read zero at the last census (ps5_mem_zero_census)
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

// A watched 16 KiB page not wholly marked written stays closed to writes, so the next write is seen (a hot page stays
// open).
static int ww_closed(uint64_t k) {
    uint16_t *w = ww_of(k);
    return w && (*w & WW_ON) && !(*w & WW_HOT) && (*w & WW_ALL) != WW_ALL;
}
static int ww_score(uint16_t w) { return (int)(w >> WW_SC_SHIFT & 63); }
static uint16_t ww_with_score(uint16_t w, int sc) {
    return (uint16_t)((w & ~WW_SC_MASK) | ((unsigned)(sc > 63 ? 63 : sc) << WW_SC_SHIFT));
}
static int ww_hot_wanted(void) {
    static int on = -1;
    if (on < 0) {
        const char *e = getenv("MKW_PS5_WW_HOT");
        on = e && !strcmp(e, "1");
    }
    return on;
}
static uint64_t st_hot_made, st_hot_aged;   // pages made hot, hot pages closed again by age
static uint64_t st_gpu_shared, st_gpu_backed, st_gpu_kept;   // ps5_mem_gpu_share: pages marked, backed there, kept

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

// Counters for the title's memory line (ps5_mem_stats): what the faults, the backing and the protection changes cost.
static uint64_t st_kprot_n, st_kprot_ns, st_back_n, st_back_bytes, st_back_ns, st_fault_n, st_fault_ns;
static uint64_t st_fault_touch, st_fault_ww, st_fault_other, st_lockwait_ns, st_ww_n, st_ww_bytes, st_ww_ns;
static uint64_t st_block_fail, st_page_fail, st_hook_after_fail, st_unback_n, st_unback_fail, st_release_fail;
static volatile int st_unback_r, st_release_r;
static volatile uint64_t st_fail_k, st_fail_n, st_fail_state;   // the last failed backing (diagnosis)
static volatile int st_fail_ra, st_fail_rm, st_fail_vq = -1;
static VqEntry st_fail_entry;   // what the kernel has at the last failed page (diagnosis)
// Distinct 2 MiB stretches of the guest's addresses that ever had a 16 KiB backing (each needs a page-table page; the
// console allows a process ~16k of them: --ps5-map-test (3), 13/11/2026), and those backed as whole blocks.
static uint8_t st_region_bits[PS_LIMIT / BLOCK / 8];
static uint64_t st_regions, st_region_blocks;
static int64_t st_guest_now;   // bytes of guest memory backed right now (backed less given back)
static inline void st_add(uint64_t *v, uint64_t d) { __atomic_fetch_add(v, d, __ATOMIC_RELAXED); }

// The GPU's access bits of a mapping (as PS5_Mesa's RADV_PS5_PROTECTION, radv_ps5_platform.c:207-209, and Pokopia's
// platform_prospero.cpp:48-60, ref/). Guest memory is GPU memory too: the runtime imports the game's CPU-visible pools
// (vk_mem_back.c import_pool) and the GPU reads them at the CPU's address. On Linux an imported page stays the GPU's
// whatever the CPU's protection; here every guest page carries GPU read and write while the CPU may touch it at all,
// so a write-watch close or a page backed after the import never takes the GPU's access away (play test p5, run 55:
// "GPU Protection fault … Unmapped page access" at 0x1182911000 in the alias region, GPU_FAULT_PAGE_FAULT_ASYNC, the
// title killed after 36 min).
#define PROT_GPU_RW 0x30   // GPU read 0x10, GPU write 0x20
int sceKernelMprotect(const void *addr, size_t len, int prot);

static int kprot(void *a, size_t n, int prot) {   // mprotect, counted
    uint64_t t0 = plat_now_ns();
    int r = sceKernelMprotect(a, n, prot ? prot | PROT_GPU_RW : prot);
    st_add(&st_kprot_n, 1), st_add(&st_kprot_ns, plat_now_ns() - t0);
    return r;
}

static void apply(uint64_t k) {
    if (is_backed(k)) kprot((void *)(uintptr_t)k, KP, kpage_prot(k));
}

// Maps zeroed direct memory over [k, k + n) (reserved kernel pages, none backed), one allocation; the pages are left
// read-write. 0 when memory is gone.
static int back_run(uint64_t k, uint64_t n) {
    uint64_t t0 = plat_now_ns();
    st_add(&st_back_n, 1), st_add(&st_back_bytes, n);
    int64_t phys = 0;
    void *at = (void *)(uintptr_t)k;
    size_t align = n >= BLOCK ? BLOCK : KP;
    int ra = sceKernelAllocateDirectMemory(0, sceKernelGetDirectMemorySize(), n, align, DMEM_TYPE, &phys), rm = 0;
    if (!ra) rm = sceKernelMapDirectMemory(&at, n, PROT_READ | PROT_WRITE | PROT_GPU_RW, MAP_FIXED, phys, align);
    if (ra || rm || at != (void *)(uintptr_t)k) {
        st_add(n >= BLOCK ? &st_block_fail : &st_page_fail, 1);
        if (n < BLOCK) {   // the last failed page, for the memory line (no output from a signal handler)
            st_fail_k = k, st_fail_n = n, st_fail_ra = ra, st_fail_rm = rm;
            VqEntry e = {0};
            st_fail_vq = sceKernelVirtualQuery((void *)(uintptr_t)k, 0, &e, sizeof e);
            st_fail_entry = e;
            st_fail_state = (uint64_t)st_get(k) | (uint64_t)st_get(k + PG) << 8 | (uint64_t)st_get(k + 2 * PG) << 16 |
                            (uint64_t)st_get(k + 3 * PG) << 24 | (uint64_t)(at != (void *)(uintptr_t)k) << 32;
        }
        if (!ra) sceKernelReleaseDirectMemory(phys, n);
        return 0;
    }
    memset(at, 0, n);   // direct memory is not cleared for us
    st_add(&st_back_ns, plat_now_ns() - t0);
    __atomic_add_fetch(&st_guest_now, (int64_t)n, __ATOMIC_RELAXED);
    if (k < PS_LIMIT) {
        uint64_t r = k / BLOCK;
        if (n >= BLOCK) st_add(&st_region_blocks, 1);
        else if (!(st_region_bits[r / 8] >> (r % 8) & 1)) st_region_bits[r / 8] |= (uint8_t)(1u << (r % 8)), st_add(&st_regions, 1);
    }
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
        if (all_closed) kprot((void *)(uintptr_t)b, BLOCK, PROT_READ);
        else if (any_closed)
            for (uint64_t a = b; a < b + BLOCK; a += KP)
                if (ww_closed(a)) kprot((void *)(uintptr_t)a, KP, PROT_READ);
        return 1;
    }
    if (!back_run(k, KP)) return 0;
    if (prot != (PROT_READ | PROT_WRITE)) kprot((void *)(uintptr_t)k, KP, prot);
    return 1;
}

// Back to reserved: the page's direct memory released, the range reserved again in its place (Gate 0 verdict 4).
static void unback(uint64_t k) {
    Chunk *c = chunk_of(k, 0);
    size_t i = (k % CHUNK) / KP;
    if (!c || !(c->backed[i / 8] >> (i % 8) & 1)) return;
    if (c->gpu[i]) {   // the GPU may still read it: kept and zeroed (decommitted pages read zero) until unshared
        kprot((void *)(uintptr_t)k, KP, PROT_READ | PROT_WRITE);
        memset((void *)(uintptr_t)k, 0, KP);
        c->ww[i] &= (uint16_t)WW_ON;
        st_add(&st_gpu_kept, 1);
        return;
    }
    void *at = (void *)(uintptr_t)k;
    st_add(&st_unback_n, 1);
    __atomic_sub_fetch(&st_guest_now, (int64_t)KP, __ATOMIC_RELAXED);
    int rr = sceKernelReserveVirtualRange(&at, KP, MAP_FIXED, KP);
    if (rr || at != (void *)(uintptr_t)k) st_add(&st_unback_fail, 1), st_unback_r = rr;
    if (c->phys && c->phys[i]) {
        int rl = sceKernelReleaseDirectMemory(c->phys[i], KP);
        if (rl) st_add(&st_release_fail, 1), st_release_r = rl;
    }
    if (c->phys) c->phys[i] = 0;
    c->backed[i / 8] &= (uint8_t)~(1u << (i % 8));
    c->ww[i] &= (uint16_t)WW_ON;   // its pages read zero again: nothing written (as Linux, unpopulated pages), not hot
}

// Whether no 4 KiB page of [a, a + n) is ours: a GiB without a chunk is all free.
static int range_free(uint64_t a, uint64_t n) {
    for (uint64_t end = a + n; a < end;) {
        uint64_t stop = (a / CHUNK + 1) * CHUNK < end ? (a / CHUNK + 1) * CHUNK : end;
        Chunk *c = chunk_of(a, 0);
        if (c)
            for (uint64_t i = (a % CHUNK) / PG, e = i + (stop - a + PG - 1) / PG; i < e; i++)
                if (c->state[i] != PS_FREE) return 0;
        a = stop;
    }
    return 1;
}

static int kpage_ours(uint64_t k) { return !range_free(k, KP); }

// The range must be free; only its first and last kernel pages can be shared with a neighbouring reservation (already
// reserved then), so everything between is reserved with one call (a 16 KiB page at a time took 15 s for the guest's
// 64 GiB alias region and the rest at the game's start, 13/11/2026).
// MKW_PS5_WRITE_WATCH=0 (args.txt): no write watch, the ranges untracked (UINT64_MAX), as Linux without userfaultfd:
// the back end then uploads them in full. To compare against the software write watch's faults (Step 4, run 15).
static int ww_wanted(void) {
    static int on = -1;
    if (on < 0) {
        const char *e = getenv("MKW_PS5_WRITE_WATCH");
        on = !(e && !strcmp(e, "0"));
    }
    return on;
}

int plat_mem_reserve(uint64_t addr, uint64_t n, int watch) {
    if (!n || addr + n > PS_LIMIT) return 0;
    watch = watch && ww_wanted();
    plat_mutex_lock(&vm_lock);
    int ok = range_free(addr, n);
    uint64_t lo = addr & ~(KP - 1), hi = (addr + n + KP - 1) & ~(KP - 1), start = lo, end = hi;
    if (ok && kpage_ours(lo)) start += KP;
    if (ok && end > start && kpage_ours(hi - KP)) end -= KP;
    if (ok && start < end) {   // reserved there exactly, or refused
        void *at = (void *)(uintptr_t)start;
        if (sceKernelReserveVirtualRange(&at, end - start, MAP_FIXED | NO_REPLACE, KP) || at != (void *)(uintptr_t)start)
            ok = 0;
    }
    if (ok) {
        st_set(addr, n, PS_RESERVED);
        for (uint64_t k = lo; k < hi && watch; k += KP) {
            uint16_t *w = ww_of(k);
            if (w) *w = WW_ON;
        }
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

// Windows lazy commit (memory.c lazy_fault): not needed here, the fault handler backs a committed page at its first
// touch as Linux does (linux_mem.c:270 plat_mem_lazy_hook, a no-op).
void plat_mem_lazy_hook(int (*fn)(uint64_t addr)) { (void)fn; }

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
        kprot((void *)(uintptr_t)k, KP, PROT_READ | PROT_WRITE);
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
    // What apply() does page by page, one mprotect per run of backed 16 KiB pages that take the same protection (as
    // ww_close_run): memory.c's mem_freeze makes idle 2 MiB regions read-only through here, 128 calls of ~36 us each
    // page by page (run 94, racing: mprotect under mem_freeze 5 % of the replay thread's samples).
    uint64_t run = 0, run_end = 0;
    int run_prot = 0;
    for (uint64_t k = addr & ~(KP - 1); k < addr + n; k += KP) {
        if (!is_backed(k)) continue;   // a hole ends the run (run_end != k at the next backed page)
        int p = kpage_prot(k);
        if (run_end != k || p != run_prot) {
            if (run_end > run) kprot((void *)(uintptr_t)run, run_end - run, run_prot);
            run = k, run_prot = p;
        }
        run_end = k + KP;
    }
    if (run_end > run) kprot((void *)(uintptr_t)run, run_end - run, run_prot);
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
            kprot((void *)(uintptr_t)k, KP, PROT_READ | PROT_WRITE);
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
    kprot((void *)(uintptr_t)run, run_end - run, prot);
    for (uint64_t k = run; k < run_end; k += KP) {
        uint16_t *w = ww_of(k);
        if (w) *w = (uint16_t)(*w & ~(WW_ALL << WW_PEND_SHIFT) & ~(*w >> WW_PEND_SHIFT & WW_ALL));   // hot/score kept
    }
}

// The committed 4 KiB pages marked written, in address order. reset: those reported are unmarked, and their 16 KiB
// pages closed to writes again. A range that is not wholly watched is untracked.
uint64_t plat_mem_written(uint64_t addr, uint64_t n, uint64_t *pages, uint64_t max, int reset) {
    if (!max || !n) return 0;
    if (addr + n > PS_LIMIT) return UINT64_MAX;
    uint64_t got = 0, end = addr + n, run = 0, run_end = 0;
    int run_prot = 0;
    uint64_t q0 = plat_now_ns();
    st_add(&st_ww_n, 1), st_add(&st_ww_bytes, n);
    for (uint64_t k = addr & ~(KP - 1); k < end; k += KP) {   // watched throughout, or untracked (set at reservation)
        uint16_t *w = ww_of(k);
        if (!w || !(*w & WW_ON)) return UINT64_MAX;
    }
    plat_mutex_lock(&vm_lock);
    int hot_on = ww_hot_wanted();
    for (uint64_t k = addr & ~(KP - 1); k < end && got < max; k += KP) {
        // The lock is let go at each 2 MiB boundary of a long range (its run closed first): a page's state is handled
        // whole under the lock, and the query need not hold it across the range, while faulting threads wait for it
        // (race, run 42: 89 s of fault-lock waits; a frame's ~500 queries hold it 2.4-5.7 ms).
        if (k != (addr & ~(KP - 1)) && !(k & (BLOCK - 1))) {
            ww_close_run(run, run_end, run_prot);
            run = run_end = 0;
            plat_mutex_unlock(&vm_lock);
            plat_mutex_lock(&vm_lock);
        }
        uint16_t *w = ww_of(k);
        if (*w & WW_HOT) {   // open: every committed 4 KiB page of it counts as written
            uint8_t rep = 0;
            for (uint64_t i = 0; i < SUB && got < max; i++) {
                uint64_t p = k + i * PG;
                if (p >= addr && p < end && st_get(p) >= PS_NONE) pages[got++] = p, rep |= (uint8_t)(1u << i);
            }
            if (!reset) continue;
            int age = ww_score(*w) + 1;
            if (age < WW_HOT_AGE || !rep) {
                *w = ww_with_score(*w, age < WW_HOT_AGE ? age : WW_HOT_AGE - 1);
                continue;
            }
            // Closed again to see whether the game still writes it (a write since faults and marks it as usual), as
            // the closing below for a wholly marked page: closed first, then only what this query reported unmarked.
            // The 4 KiB pages it did not report (outside [addr, end), or past max) stay marked written: clearing the
            // whole page here lost the writes made to them since the last query that covered them (run 91, hot pages
            // on: flicker and geometry spikes near other karts, the course map without its coloured layer).
            *w = (uint16_t)(WW_ON | WW_ALL | ((unsigned)rep << WW_PEND_SHIFT));
            st_add(&st_hot_aged, 1);
            if (is_backed(k)) {
                int prot = kpage_prot(k) & ~PROT_WRITE;
                if (run_end != k || run_prot != prot) {
                    ww_close_run(run, run_end, run_prot);
                    run = k, run_prot = prot;
                }
                run_end = k + KP;
            }
            continue;
        }
        if (!(*w & WW_ALL)) {
            if (reset && ww_score(*w)) *w = ww_with_score(*w, 0);   // a reset that found it unwritten: not hot
            continue;
        }
        uint8_t cleared = 0;
        for (uint64_t i = 0; i < SUB && got < max; i++) {
            uint64_t p = k + i * PG;
            if (p < addr || p >= end || !(*w >> i & 1) || st_get(p) < PS_NONE) continue;
            pages[got++] = p;
            cleared |= (uint8_t)(1u << i);
        }
        if (!reset || !cleared) continue;
        int sc = ww_score(*w) + 1;
        if (hot_on && sc >= WW_HOT_AFTER && (*w & WW_ALL) == WW_ALL && is_backed(k) && (kpage_prot(k) & PROT_WRITE)) {
            *w = (uint16_t)((*w & ~WW_SC_MASK) | WW_HOT);   // open already (wholly marked): stays so, age 0
            st_add(&st_hot_made, 1);
            continue;
        }
        *w = ww_with_score(*w, sc);
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
    st_add(&st_ww_ns, plat_now_ns() - q0);
    return got;
}

// A write by the kernel itself (pread into a buffer) does not fault into the handler: it fails with EFAULT on a page
// that has no direct memory yet or that write watch keeps closed (the game's first RomFS read, 13/11/2026: "the buffer
// is not writable"); Linux's kernel backs and tracks such pages itself. So a buffer the kernel is about to fill is made
// ready as a first write would make it: backed, marked written where watched, opened. Pages the guest may not write
// stay as they are, and the call fails as it should.
static int in_lazy(uint64_t a);

void ps5_mem_prepare_write(uint64_t a, uint64_t n) {
    if (!n) return;
    plat_mutex_lock(&vm_lock);
    for (uint64_t k = a & ~(KP - 1); k < a + n; k += KP) {
        if (in_lazy(k)) {
            if (!is_backed(k)) back(k, PROT_READ | PROT_WRITE);
            continue;
        }
        if (k >= PS_LIMIT) continue;
        int writable = 0;
        for (uint64_t i = 0; i < SUB; i++) {
            uint64_t p = k + i * PG;
            writable |= p + PG > a && p < a + n && st_get(p) == PS_RW;
        }
        if (!writable || (!is_backed(k) && !back(k, kpage_prot(k)))) continue;
        ww_mark(k);
        apply(k);
    }
    plat_mutex_unlock(&vm_lock);
}

// [a, a + n) becomes GPU memory (vk_mem_back.c import_pool, before the import): every 16 KiB page of it that is ours is
// backed now, as the GPU does not fault into the handler that backs pages at the CPU's first touch (Pokopia backs a
// range before importing it, paged_vm.cpp:545-605 in ref/Pokopia-Native-PS5), and marked so it is never given back.
void ps5_mem_gpu_share(uint64_t a, uint64_t n) {
    if (!n) return;
    plat_mutex_lock(&vm_lock);
    for (uint64_t k = a & ~(KP - 1); k < a + n && k < PS_LIMIT; k += KP) {
        Chunk *c = chunk_of(k, 1);   // every page counted, committed or not: ps5_mem_gpu_unshare takes the same range
        size_t i = (k % CHUNK) / KP;
        if (c && c->gpu[i] < 255 && !c->gpu[i]++) st_add(&st_gpu_shared, 1);
        int any = 0;
        for (uint64_t j = 0; j < SUB; j++) any |= st_get(k + j * PG) >= PS_NONE;
        if (!any || is_backed(k)) continue;   // not committed: nothing to back (the GPU should not read it either)
        if (!back(k, kpage_prot(k))) continue;   // out of memory: the import goes on, a GPU read there would fault
        st_add(&st_gpu_backed, 1);
    }
    plat_mutex_unlock(&vm_lock);
}

// The import over [a, a + n) is gone (vk_mem_back.c destroy_rec, after its vkFreeMemory; or a failed import): one
// count less per page. A page no import covers any more and that the guest decommitted meanwhile (kept, zeroed, by
// unback) is released now. Without this the kept pages only grew: 29,807 (466 MiB) after 25 min of racing (run 70),
// the census's "unreadable" pages (backed, nothing committed).
static uint64_t st_gpu_unshared, st_gpu_released;
void ps5_mem_gpu_unshare(uint64_t a, uint64_t n) {
    if (!n) return;
    plat_mutex_lock(&vm_lock);
    for (uint64_t k = a & ~(KP - 1); k < a + n && k < PS_LIMIT; k += KP) {
        Chunk *c = chunk_of(k, 0);
        size_t i = (k % CHUNK) / KP;
        if (!c || !c->gpu[i] || c->gpu[i] == 255) continue;   // 255: saturated, kept for the run
        if (--c->gpu[i]) continue;
        st_add(&st_gpu_unshared, 1);
        int any = 0;
        for (uint64_t j = 0; j < SUB; j++) any |= st_get(k + j * PG) >= PS_NONE;
        if (!any && is_backed(k)) unback(k), st_add(&st_gpu_released, 1);
    }
    plat_mutex_unlock(&vm_lock);
}

// Whether the guest may write every 4 KiB page of [a, a + n) (committed read-write, or a lazily backed range), and
// (diagnosis) the state and the kernel protection of the first page where the kernel page itself refuses writes.
int ps5_mem_writable(uint64_t a, uint64_t n, char *why, size_t cap) {
    if (why && cap) why[0] = 0;
    int ok = 1;
    plat_mutex_lock(&vm_lock);
    for (uint64_t p = a & ~(PG - 1); p < a + n && ok; p += PG)
        ok = in_lazy(p) || (p < PS_LIMIT && st_get(p) == PS_RW);
    for (uint64_t k = a & ~(KP - 1); k < a + n && why && !why[0]; k += KP) {
        if (k >= PS_LIMIT || in_lazy(k) || (kpage_prot(k) & PROT_WRITE)) continue;
        uint16_t *w = ww_of(k);
        snprintf(why, cap, "kernel page %#llx: states %d %d %d %d, backed %d, watch %#x", (unsigned long long)k,
                 st_get(k), st_get(k + PG), st_get(k + 2 * PG), st_get(k + 3 * PG), is_backed(k), w ? *w : 0);
    }
    plat_mutex_unlock(&vm_lock);
    return ok;
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
// 1 handled, 0 not ours (a real fault, or the hook's), -1 a committed page whose memory could not be mapped: on Linux
// the kernel never fails a first touch (the OOM killer ends the process instead), so the title stops with a report
// rather than retrying the access for ever (Step 4: run 21 retried 30,166 failed backings and hung).
static int ps5_fault(uint64_t a, int write) {
    uint64_t k = a & ~(KP - 1);
    if (in_lazy(a)) {
        plat_mutex_lock(&vm_lock);
        int ok = is_backed(k) || back(k, PROT_READ | PROT_WRITE);
        plat_mutex_unlock(&vm_lock);
        return ok ? 1 : -1;
    }
    int s = st_get(a);
    if (s < PS_NONE) return 0;   // not committed: a real fault
    uint64_t w0 = plat_now_ns();
    plat_mutex_lock(&vm_lock);
    st_add(&st_lockwait_ns, plat_now_ns() - w0);
    int fresh = !is_backed(k), closed = write && ww_closed(k);
    st_add(fresh ? &st_fault_touch : closed ? &st_fault_ww : &st_fault_other, 1);
    if (fresh && !back(k, kpage_prot(k))) {
        plat_mutex_unlock(&vm_lock);
        return -1;
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
        kprot((void *)(uintptr_t)k, KP, write ? PROT_READ | PROT_WRITE : PROT_READ);
        sibling_lifts++;
    } else {
        apply(k);   // the protection the state gives now (the write mark above opens a watched page)
    }
    plat_mutex_unlock(&vm_lock);
    return 1;
}

static void fault_handler(int sig, siginfo_t *si, void *uc) {
    uint64_t t0 = plat_now_ns();
    st_add(&st_fault_n, 1);
    ucontext_t *u = uc;
    long err = (long)u->uc_mcontext.mc_err;   // x86's page fault error code: 2 a write, 16 an instruction fetch
    uint64_t a = (uint64_t)(uintptr_t)si->si_addr;
    int segv = sig == SIGSEGV || sig == SIGBUS;   // FreeBSD reports a protection fault as SIGBUS
    if (segv && watch_fn && (err & 2) && watch_fn(a, (uint64_t)u->uc_mcontext.mc_rip)) return;
    plat_fault_write = (err & 2) != 0, plat_fault_pc = (uint64_t)u->uc_mcontext.mc_rip;
    plat_fault_ret = *(const uint64_t *)(uintptr_t)u->uc_mcontext.mc_rsp;
    int mine = segv && !(err & 16) ? ps5_fault(a, (err & 2) != 0) : 0;
    if (mine > 0) {
        st_add(&st_fault_ns, plat_now_ns() - t0);
        return;
    }
    if (mine < 0) {   // out of memory or page tables for a committed page: say so, then the crash report below
        int ct = -1, cf = -1, gt = -1, gf = -1;
        (void)sceKernelGetPageTableStats(&ct, &cf, &gt, &gf);
        char line[256];
        int n = snprintf(line, sizeof line,
                         "[host] FATAL: no memory to back committed page %p (allocate %#x map %#x; cpu page tables %d "
                         "free %d, gpu %d free %d)\n",
                         si->si_addr, (unsigned)st_fail_ra, (unsigned)st_fail_rm, ct, cf, gt, gf);
        if (n > 0) (void)!write(ps5_log_fd, line, (size_t)n < sizeof line ? (size_t)n : sizeof line - 1);
    }
    if (mine == 0 && segv && fault_hook && fault_hook(a)) {
        if (a < PS_LIMIT && st_get(a) >= PS_NONE && !is_backed(a & ~(KP - 1))) st_add(&st_hook_after_fail, 1);
        return;
    }
    uintptr_t rip = (uintptr_t)u->uc_mcontext.mc_rip;
    Dl_info at = {0}, self = {0};
    int in_module = dladdr((void *)rip, &at) != 0, ours = 0;
    if (in_module && dladdr((void *)plat_crash_hook, &self)) ours = at.dli_fbase == self.dli_fbase;
    {   // A raw line first: the crash report below takes the log stream's and the heap's locks, and a thread stopped
        // holding one of them turned crashes into hangs with nothing logged (play test p4: picture frozen, log and
        // thread dump silent). write(2) takes no lock.
        char raw[200];
        int n = snprintf(raw, sizeof raw, "[host] ps5: fatal signal %d at %#lx (exe+%#lx) addr %p err %#lx rsp %#lx\n",
                         sig, (unsigned long)rip, (unsigned long)(in_module ? rip - (uintptr_t)at.dli_fbase : rip),
                         si->si_addr, (unsigned long)err, (unsigned long)u->uc_mcontext.mc_rsp);
        if (n > 0) (void)!write(ps5_log_fd, raw, (size_t)n < sizeof raw ? (size_t)n : sizeof raw - 1);
    }
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
        if (n > 0) (void)!write(ps5_log_fd, line, (size_t)n < sizeof line ? (size_t)n : sizeof line - 1);
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

// Zero-page census (measurement for the 2-hour goal, 13/11/2026): of the guest's backed 16 KiB pages, how many read
// all zero right now, split by what giving them back would have to respect (a page the GPU may read, a watched one).
// Guest memory only grows (39-51 min to the pool's end, runs 57 and 60); a page that reads zero could be released and
// backed again, zeroed, at its next touch, as a kernel's shared zero page. The lock is taken per 2 MiB, so faulting
// threads wait at most one 2 MiB scan; pages the guest cannot read (PROT_NONE) are skipped, never touched.
// MKW_PS5_ZERO_RECLAIM (on; 0 turns it off): a page that read zero at two censuses in a row is given back (unback: reserved again, its
// direct memory released; the next touch backs it zeroed, so the guest reads the same bytes). Not given back: pages
// the GPU may read (kept, as on decommit), hot pages, and watched pages with written or pending marks (a write the
// write watch has not reported yet would be lost; with none, the page read zero when last reported and reads zero
// after, so nothing changed for the GPU side).
static int page_zero(const uint64_t *p) {
    for (size_t i = 0; i < KP / 8; i += 8)
        if (p[i] | p[i + 1] | p[i + 2] | p[i + 3] | p[i + 4] | p[i + 5] | p[i + 6] | p[i + 7]) return 0;
    return 1;
}

static uint64_t st_zero_given;   // pages given back by the census, in all

static int zero_reclaim_wanted(void) {
    static int on = -1;
    if (on < 0) {
        const char *e = getenv("MKW_PS5_ZERO_RECLAIM");   // on unless 0 (runs 63, 65, 69: ~600 MiB given back)
        on = !(e && !strcmp(e, "0"));
    }
    return on;
}

int ps5_mem_zero_census(char *out, size_t cap) {
    uint64_t t0 = plat_now_ns(), backed = 0, zero = 0, zero_gpu = 0, zero_watched = 0, unreadable = 0, given = 0,
             zero_shut = 0;
    static int peek_shut = -1;   // MKW_PS5_ZERO_PEEK=1: no-access pages are tested too (run 64 stalled with it: suspect)
    if (peek_shut < 0) {
        const char *e = getenv("MKW_PS5_ZERO_PEEK");
        peek_shut = e && !strcmp(e, "1");
    }
    int reclaim = zero_reclaim_wanted();
    for (size_t ci = 0; ci < NCHUNK; ci++) {
        Chunk *c = chunks[ci];
        if (!c) continue;
        for (uint64_t b = 0; b < CHUNK; b += BLOCK) {
            size_t i0 = b / KP, i1 = (b + BLOCK) / KP, any = 0;
            for (size_t i = i0; i < i1 && !any; i += 8) any = c->backed[i / 8];
            if (!any) continue;
            plat_mutex_lock(&vm_lock);
            for (size_t i = i0; i < i1; i++) {
                if (!(c->backed[i / 8] >> (i % 8) & 1)) continue;
                uint64_t k = (uint64_t)ci * CHUNK + (uint64_t)i * KP;
                backed++;
                uint8_t bit = (uint8_t)(1u << (i % 8));
                int shut = !(kpage_prot(k) & PROT_READ);
                if (shut) {   // a page the guest cannot read (41k pages at 40 min, run 63): opened to read here alone
                    unreadable++;
                    if (!peek_shut) { c->zseen[i / 8] &= (uint8_t)~bit; continue; }
                    kprot((void *)(uintptr_t)k, KP, PROT_READ);
                }
                int z = page_zero((const uint64_t *)(uintptr_t)k);
                if (shut) apply(k);   // its protection back (vm_lock held: no guest access saw it open)
                if (!z) { c->zseen[i / 8] &= (uint8_t)~bit; continue; }
                zero++;
                if (shut) zero_shut++;
                int gpu = c->gpu[i] != 0, w = c->ww[i];
                if (gpu) zero_gpu++;
                else if (w & WW_ON) zero_watched++;
                int marks = (w & WW_ON) && (w & (WW_HOT | WW_ALL | (WW_ALL << WW_PEND_SHIFT)));
                if (reclaim && (c->zseen[i / 8] & bit) && !gpu && !marks) {
                    unback(k);   // clears backed; ww keeps WW_ON only
                    c->zseen[i / 8] &= (uint8_t)~bit;
                    given++;
                    continue;
                }
                c->zseen[i / 8] |= bit;
            }
            plat_mutex_unlock(&vm_lock);
        }
    }
    st_zero_given += given;
    return snprintf(out, cap, "zero census: %llu of %llu backed 16 KiB pages read zero (%.1f of %.1f MiB): GPU-shared %llu, "
                    "watched %llu, other %llu; unreadable %llu (zero %llu); given back now %llu (%.1f MiB), in all %.1f MiB%s; %.0f ms",
                    (unsigned long long)zero, (unsigned long long)backed, zero * (KP / 1048576.0), backed * (KP / 1048576.0),
                    (unsigned long long)zero_gpu, (unsigned long long)zero_watched,
                    (unsigned long long)(zero - zero_gpu - zero_watched), (unsigned long long)unreadable,
                    (unsigned long long)zero_shut, (unsigned long long)given, given * (KP / 1048576.0), st_zero_given * (KP / 1048576.0),
                    reclaim ? "" : " (MKW_PS5_ZERO_RECLAIM off)", (plat_now_ns() - t0) / 1e6);
}

// The counters above as one line for the title's memory report (ps5_main.c).
// One short line for the page-table sampler (ps5_main.c pt_watch): guest backing so far.
int ps5_mem_brief(char *out, size_t cap) {
    return snprintf(out, cap, "guest: %.1f MiB backed now; backing %llu calls %.1f MiB, stretches with pages %llu, blocks %llu, unback %llu, "
                    "failed %llu",
                    __atomic_load_n(&st_guest_now, __ATOMIC_RELAXED) / 1048576.0,
                    (unsigned long long)__atomic_load_n(&st_back_n, __ATOMIC_RELAXED),
                    __atomic_load_n(&st_back_bytes, __ATOMIC_RELAXED) / 1048576.0,
                    (unsigned long long)__atomic_load_n(&st_regions, __ATOMIC_RELAXED),
                    (unsigned long long)__atomic_load_n(&st_region_blocks, __ATOMIC_RELAXED),
                    (unsigned long long)__atomic_load_n(&st_unback_n, __ATOMIC_RELAXED),
                    (unsigned long long)(__atomic_load_n(&st_block_fail, __ATOMIC_RELAXED) +
                                         __atomic_load_n(&st_page_fail, __ATOMIC_RELAXED)));
}

int ps5_mem_stats(char *out, size_t cap) {
    uint64_t fn = __atomic_load_n(&st_fault_n, __ATOMIC_RELAXED), kn = __atomic_load_n(&st_kprot_n, __ATOMIC_RELAXED),
             bn = __atomic_load_n(&st_back_n, __ATOMIC_RELAXED);
    VqEntry e = st_fail_entry;
    e.name[sizeof e.name - 1] = 0;
    int ct = -1, cf = -1, gt = -1, gf = -1;
    (void)sceKernelGetPageTableStats(&ct, &cf, &gt, &gf);
    int k = snprintf(out, cap, "guest backed now %.1f MiB; page tables cpu %d free %d, gpu %d free %d; 2 MiB stretches with 16 KiB backings %llu, whole 2 MiB blocks %llu; query of the last failed page %#x: %p-%p offset %#llx prot %#x type %d flags %#x '%s'; ",
                     __atomic_load_n(&st_guest_now, __ATOMIC_RELAXED) / 1048576.0, ct, cf, gt, gf,
                     (unsigned long long)st_regions, (unsigned long long)st_region_blocks, (unsigned)st_fail_vq, e.start, e.end, (unsigned long long)e.offset, (unsigned)e.protection, e.type,
                     (unsigned)e.flags, e.name);
    if (k < 0 || (size_t)k >= cap) return k;
    out += k, cap -= (size_t)k;
    return k + snprintf(out, cap,
                    "faults %llu (first touch %llu, write watch %llu, other %llu) %.2f s in all; backing %llu calls %.1f MiB "
                    "%.2f s (2 MiB blocks failed %llu, pages failed %llu; last %#llx states %#llx allocate %#x map %#x; hook "
                    "retried unbacked %llu; unback %llu, re-reserve failed %llu (%#x), release failed %llu (%#x)); mprotect %llu %.2f s; fault lock waits %.2f s; write-watch queries %llu, "
                    "%.1f GiB, %.2f s; hot pages made %llu, closed again %llu; GPU pages shared %llu (backed there %llu), kept on decommit %llu, unshared %llu (released then %llu)",
                    (unsigned long long)fn, (unsigned long long)st_fault_touch, (unsigned long long)st_fault_ww,
                    (unsigned long long)st_fault_other, st_fault_ns / 1e9, (unsigned long long)bn,
                    st_back_bytes / 1048576.0, st_back_ns / 1e9, (unsigned long long)st_block_fail,
                    (unsigned long long)st_page_fail, (unsigned long long)st_fail_k, (unsigned long long)st_fail_state,
                    (unsigned)st_fail_ra, (unsigned)st_fail_rm, (unsigned long long)st_hook_after_fail,
                    (unsigned long long)st_unback_n, (unsigned long long)st_unback_fail, (unsigned)st_unback_r,
                    (unsigned long long)st_release_fail, (unsigned)st_release_r, (unsigned long long)kn,
                    st_kprot_ns / 1e9, st_lockwait_ns / 1e9, (unsigned long long)st_ww_n, st_ww_bytes / 1073741824.0,
                    st_ww_ns / 1e9, (unsigned long long)st_hot_made, (unsigned long long)st_hot_aged, (unsigned long long)st_gpu_shared, (unsigned long long)st_gpu_backed, (unsigned long long)st_gpu_kept,
                    (unsigned long long)st_gpu_unshared, (unsigned long long)st_gpu_released);
}
