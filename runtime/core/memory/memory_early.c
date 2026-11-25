// The early-read watch as in mkw-pit-stop (MIT, Copyright (c) 2026 Adrià; PS5 work by exploitizm)
// runtime/core/memory/memory_early.c @ 0109457 (commits b907279, 9edd0e2, e50225b, 2238f9b and earlier), taken whole
// over upstream's (which it extends; upstream's is unchanged since f430329), except: their hang watch's lines and
// their profiler's names left out (ldr_describe names the code instead).
// MKW_EARLY_READ=1 (diagnosis): does the game read memory the GPU has yet to write? With the replay ~3 submits behind
// the game, what the GPU writes (copies into buffers, counters, cleared buffers, storage buffers a shader may write)
// reaches memory later than on the console, and the game decides what it draws with some of it (menus:
// trees and cacti dark for a frame). MKW_SUBMIT_CATCHUP makes every submit wait for its GPU work instead, at ~10 ms a
// frame. Here the destinations a submit lists (nvn_api_cmd.c resolve_deferred) are made inaccessible at the submit, with
// that submit's fence; the first access to such a page puts its protection back and is counted: early when the GPU has
// not finished the submit, late when it has, by a game thread or by the runtime. Nothing waits: it only measures where a
// wait would be needed. Linux and PS5 (read traps, memory.c). One summary line every 600 frames with the first early reads.
// MKW_EARLY_READ=2/3 wait, but only the game's own code (kern_fault_in_game): a guest thread at a lifted function's
// rip with no host call in progress, which holds no host lock. Runtime threads, and host code on guest threads, release
// the watch and go on: they order on the GPU's writes through the runtime's own fences. A wait for a fence the replay has
// not handed to the queue is bounded (ISSUE_WAIT_NS) and named in the hang watch (run 25 hung with waits by host code).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cfg.h"
#include "host.h"
#include "kernel_int.h"
#include "memory_int.h"
#include "prof.h"

#define PAGE 0x1000ull
#define EW_KINDS 6
#define EW_FIRST 12
#define EW_CHUNK_SHIFT 24
#define EW_CHUNK_SIZE (1ull << EW_CHUNK_SHIFT)
#define EW_CHUNK_PAGES (EW_CHUNK_SIZE / PAGE)
#define EW_CHUNKS (1u << (39 - EW_CHUNK_SHIFT))

static const char *const kind_name[EW_KINDS] = {"counter", "copy", "texture to buffer", "clear", "storage buffer", "uniform update"};
int64_t mem_early_pages;
// MKW_EARLY_READ=3: per guest page, the last submit listed as writing it and the one before. On the console a read
// right after submit N finds N-1's results (the GPU has only begun N): the console-like wait is for the previous writer
// when it has not finished, not for N (=2 waited for N: ~1 wait a frame of 7-16 ms, ews5).
int mem_fence_early;
uint64_t mem_early_rel;
// Per guest page, the 64-byte pieces the two newest recorded writers write (fence, mask): which bytes an unfinished
// writer leaves pending, for mem_early_invalidate. wm_lock.
typedef struct { uint64_t f[2], m[2]; } WMask;
typedef struct {
    uint64_t meta[EW_CHUNK_PAGES];
    uint64_t gpu_wrote[EW_CHUNK_PAGES];
    uint64_t lastf[EW_CHUNK_PAGES];
    uint64_t prevf[EW_CHUNK_PAGES];
    WMask wmask[EW_CHUNK_PAGES];
} EarlyChunk;
static EarlyChunk *early_chunks[EW_CHUNKS];
static PlatMutex chunk_lock = PLAT_MUTEX_INIT;
static PlatMutex wm_lock = PLAT_MUTEX_INIT;
uint64_t (*mem_fence_done)(void), (*mem_fence_queued)(void);
static int (*fence_wait)(uint64_t value, uint64_t timeout_ns);
static int (*fence_issued_wait)(uint64_t value, uint64_t timeout_ns);   // value handed to the queue whole, at most that
// A wait for a fence the replay has not handed to the queue yet: at most this long for it to be (the replay is behind),
// then the read goes on without the GPU's bytes. The waiting game thread holds no host lock (kern_fault_in_game) and
// the replay needs no guest core, but a wait that depends on the replay's progress is bounded all the same.
#define ISSUE_WAIT_NS 50000000ull
static int64_t not_game, not_issued;   // since the last log line: touches by host code on guest threads (no wait); waits
                                       // given up because the fence was not handed to the queue in ISSUE_WAIT_NS
static int wait_mode;                 // MKW_EARLY_READ=2: an early read of bytes the GPU writes waits for its submit
static int early_batch = -1;          // MKW_EARLY_BATCH (default 1): coalesce contiguous protect calls
static int early_wait_away = -1;      // MKW_EARLY_WAIT_AWAY (default 0): release virtual core while waiting on GPU
static int64_t waits, wait_ns;        // since the last log line
static int64_t range_skips;           // frz_lock: watches released by mem_early_invalidate since the last log line
// The wait the fault on this thread asked for, logged once it ends when it was long: which page, who read it and how
// far behind the GPU was (lwab1: ~6 waits of ~7.5 ms every 600 frames, each a one-frame hitch).
static _Thread_local struct { uint64_t page, pc, done, last, frame, addr, watched; int diff; } wq;
static int nwait_logged;
// The waits by the code that read (the fault's pc), for the 600-frame line: the wait lines above stop after the first
// 60, so a race's waits named no reader (task 36, 16/11/2026: the sibling A/B run, whose 60 lines all fell in the race,
// gave 58 to main+0x154e14 at 0x25a052c0e8, ~1 a frame of ~9.7 ms). sib: waits for another 4 KiB page of the native page.
#define WREAD_N 8
static struct { uint64_t pc, addr; int64_t n, ns, sib; } wread[WREAD_N];
static int64_t wread_other;   // waits that found the table full
static PlatMutex wread_lock = PLAT_MUTEX_INIT;
static int64_t cnt[EW_KINDS][4];   // frz_lock: early read by the game, early write by the game, late by the game, by
                                   // the runtime
static struct { uint64_t page, fence, done, frame, pc; int kind, write; } first[EW_FIRST];
static int nfirst;
uint64_t mem_early_frame;   // the last frame mem_early_log saw
// A bound storage buffer is listed whether or not its shader writes it. What the GPU does write the replay reports
// later (mem_early_gpu_wrote, from vk_mem_gpu_pending), so the game's early reads are kept and told apart at the log:
// of a page the GPU writes (a real early read) or of one it only reads (no wait needed).
#define EW_RING (1u << 16)
static struct { uint64_t addr, pc; } ring[EW_RING];   // the byte read: told against the pieces the GPU writes
static uint32_t nring;
static struct { uint64_t pc, n; } rt_pc[8];   // frz_lock: where the runtime's own touches come from (a fault each)

static EarlyChunk *early_chunk(uint64_t a, int create) {
    if (a >= RT_ADDRESS_LIMIT) return NULL;
    size_t cidx = (size_t)(a >> EW_CHUNK_SHIFT);
    EarlyChunk *c = __atomic_load_n(&early_chunks[cidx], __ATOMIC_ACQUIRE);
    if (c || !create) return c;
    plat_mutex_lock(&chunk_lock);
    c = early_chunks[cidx];
    if (!c) {
        c = calloc(1, sizeof *c);
        if (c) __atomic_store_n(&early_chunks[cidx], c, __ATOMIC_RELEASE);
    }
    plat_mutex_unlock(&chunk_lock);
    return c;
}

static inline size_t early_idx(uint64_t a) { return (size_t)((a & (EW_CHUNK_SIZE - 1)) / PAGE); }

void mem_early_page_fences(uint64_t addr, uint64_t *lastf, uint64_t *prevf) {
    EarlyChunk *c = early_chunk(addr, 0);
    if (!c) { *lastf = *prevf = 0; return; }
    size_t idx = early_idx(addr);
    *lastf = c->lastf[idx];
    *prevf = c->prevf[idx];
}

// MKW_EARLY_READ: 3 by default on Linux and PS5, where with MKW_SHADOW_CPU_WINS it stands for MKW_SUBMIT_CATCHUP;
// off on Windows, which has no read traps (win32.c).
static int early_cfg(void) {
    const char *e = cfg_get("MKW_EARLY_READ");
#ifndef _WIN32
    if (!e) e = "3";
#endif
    return e ? atoi(e) : 0;
}

int mem_early_wanted(void) { return early_cfg(); }

static void sibling_emu_init(void);

int mem_early_on(void) {
    static int on = -1;
    if (on < 0) {
        on = early_cfg() && mem_trapped_init();
        wait_mode = on ? early_cfg() - 1 : 0;   // 1 for N, 2 for the previous writer
        if (wait_mode < 0 || wait_mode > 2) wait_mode = 0;
        if (on) {
            sibling_emu_init();  // (its code reads checked here, outside the fault handler)
        }
        if (on) rt_log("mem: early-read watch on (MKW_EARLY_READ=%d, %zu KiB): what a submit's GPU work writes is watched until touched%s",
                       wait_mode + 1, plat_mem_early_page_size() >> 10,
                       wait_mode == 1 ? "; an early read of bytes the GPU writes waits for that submit"
                                      : wait_mode == 2 ? "; an early read waits for the previous submit writing them" : "");
    }
    return on;
}

// The runtime's reads of guest memory that may hold watched pages (the shadow sync, the uniform snapshot): a watched page
// is read through its protection, so the watch stays for the game (ews4: 96 641 faults by shadow_copy in 600 frames,
// each lifting a watch the game's early read would then pass).
void mem_copy_out(void *dst, uint64_t a, uint64_t n) {
    if (mem_early_on() <= 0 || !watched(a, n)) {
        memcpy(dst, (const void *)(uintptr_t)a, n);
        return;
    }
    for (uint64_t x = a, y; x < a + n; x = y) {
        int w = __atomic_load_n(&trapped[x / PAGE], __ATOMIC_RELAXED) == 2;
        for (y = (x & ~(PAGE - 1)) + PAGE; y < a + n && (__atomic_load_n(&trapped[y / PAGE], __ATOMIC_RELAXED) == 2) == w; y += PAGE) {}
        if (y > a + n) y = a + n;
        uint8_t *d = (uint8_t *)dst + (x - a);
        if (!w) memcpy(d, (const void *)(uintptr_t)x, y - x);
        else if (!plat_mem_peek(d, x, y - x)) memcpy(d, (const void *)(uintptr_t)x, y - x);   // falls back to the fault
    }
}

// The same copy for a caller that must not wait (vk_usnap_take on the game's thread inside the submit, or on the
// replay's when it lets a held submission go): a watched page that cannot be read through its protection is not read
// the plain way, whose fault would wait for the page's GPU writer, which can be the very submission being taken (the
// suspected cause of a hang with the held take, ucmp 21/11/2026; with this, none in uats-1/2). 0 when such a page was
// left out.
int mem_copy_out_try(void *dst, uint64_t a, uint64_t n) {
    if (mem_early_on() <= 0 || !watched(a, n)) {
        memcpy(dst, (const void *)(uintptr_t)a, n);
        return 1;
    }
    for (uint64_t x = a, y; x < a + n; x = y) {
        int w = __atomic_load_n(&trapped[x / PAGE], __ATOMIC_RELAXED) == 2;
        for (y = (x & ~(PAGE - 1)) + PAGE; y < a + n && (__atomic_load_n(&trapped[y / PAGE], __ATOMIC_RELAXED) == 2) == w; y += PAGE) {}
        if (y > a + n) y = a + n;
        uint8_t *d = (uint8_t *)dst + (x - a);
        if (!w) memcpy(d, (const void *)(uintptr_t)x, y - x);
        else if (!plat_mem_peek(d, x, y - x)) return 0;
    }
    return 1;
}

void mem_early_set_done(uint64_t (*done)(void), int (*wait)(uint64_t, uint64_t), uint64_t (*queued)(void),
                        int (*issued_wait)(uint64_t, uint64_t)) {
    mem_fence_done = done, fence_wait = wait, mem_fence_queued = queued, fence_issued_wait = issued_wait;
}

// The fault's wait, after frz_lock is released (the GPU's bytes come back under it, on the completion thread, before
// the fence counts as done): the read is then retried and finds them, or a read trap that fills it.
void mem_early_wait(uint64_t fence) {
    static uint8_t before[PAGE], after[PAGE];   // MKW_EARLY_READ_DRY=3: the page without and with the wait
    static PlatMutex diff_lock = PLAT_MUTEX_INIT;
    int diff = wq.diff && plat_mutex_trylock(&diff_lock);
    if (diff) plat_mem_peek(before, wq.page, PAGE);
    uint64_t t0 = plat_now_ns();
    // Only the game's own code reaches here (kern_fault_in_game), holding no host lock. A fence the replay has not
    // handed to the queue yet is waited for ISSUE_WAIT_NS at most: past that the read goes on without the wait.
    if (early_wait_away < 0) early_wait_away = rt_knob("EARLY_WAIT_AWAY", "0");
    int away = RT_KNOB_ON(early_wait_away) ? kern_core_away() : 0;
    int issued = !fence_issued_wait || fence_issued_wait(fence, ISSUE_WAIT_NS);
    if (issued && fence_wait) {
        fence_wait(fence, 500000000ull);
    } else if (!issued) {
        __atomic_add_fetch(&not_issued, 1, __ATOMIC_RELAXED);
    }
    kern_core_back(&away);
    uint64_t ns = plat_now_ns() - t0;
    if (diff) {   // the page as the game reads it now: a trap left by the write-back is filled through the fault
        mem_copy_out(after, wq.page, PAGE);
        int nd = 0, lo = -1, hi = -1;
        for (int i = 0; i < (int)PAGE; i++)
            if (before[i] != after[i]) nd++, hi = i, lo = lo < 0 ? i : lo;
        int o = (int)(wq.addr & (PAGE - 1) & ~15ull);
        rt_log("mem: early-read wait at frame %llu changed %d bytes of page %#llx (offsets %#x-%#x); at %#llx without "
               "the wait %02x%02x%02x%02x %02x%02x%02x%02x, with it %02x%02x%02x%02x %02x%02x%02x%02x",
               (unsigned long long)wq.frame, nd, (unsigned long long)wq.page, lo, hi, (unsigned long long)(wq.page + o),
               before[o], before[o + 1], before[o + 2], before[o + 3], before[o + 4], before[o + 5], before[o + 6],
               before[o + 7], after[o], after[o + 1], after[o + 2], after[o + 3], after[o + 4], after[o + 5],
               after[o + 6], after[o + 7]);
        plat_mutex_unlock(&diff_lock);
    }
    __atomic_add_fetch(&waits, 1, __ATOMIC_RELAXED);
    __atomic_add_fetch(&wait_ns, (int64_t)ns, __ATOMIC_RELAXED);
    plat_mutex_lock(&wread_lock);   // after a wait of milliseconds: the lock costs nothing beside it
    int j = 0;
    while (j < WREAD_N && wread[j].n && wread[j].pc != wq.pc) j++;
    if (j < WREAD_N) {
        wread[j].pc = wq.pc, wread[j].addr = wq.addr, wread[j].n++, wread[j].ns += (int64_t)ns;
        wread[j].sib += wq.page != wq.watched;
    } else {
        wread_other++;
    }
    plat_mutex_unlock(&wread_lock);
    if (ns > 1000000 && __atomic_fetch_add(&nwait_logged, 1, __ATOMIC_RELAXED) < 60) {   // the close criterion: 1 ms
        char who[48] = "?", code[192];
        plat_sample_self_name(who, sizeof who);
        ldr_describe(wq.pc, code, sizeof code);   // (theirs: prof_pc_describe, their profiler)
        rt_log("mem: early-read wait %.1f ms at frame %llu: address %#llx (watched page %#llx) for fence %llu%s (GPU at "
               "%llu, the page's newest writer %llu), by %s thread %#x in %s", (double)ns / 1e6,
               (unsigned long long)wq.frame, (unsigned long long)wq.addr, (unsigned long long)wq.watched,
               (unsigned long long)fence, issued ? "" : ", not handed to the queue in time: not waited",
               (unsigned long long)wq.done, (unsigned long long)wq.last, who, tcur ? tcur->handle : 0, code);
    }
}

// The 64-byte pieces of page p that [a, end) covers, a bit each.
static uint64_t page_bits(uint64_t p, uint64_t a, uint64_t end) {
    uint64_t lo = p > a ? p : a, hi = p + PAGE < end ? p + PAGE : end;
    unsigned first = (unsigned)(lo % PAGE / 64), last = (unsigned)((hi - 1) % PAGE / 64);
    return (~0ull >> (63 - last)) & (~0ull << first);
}

// One atomic a page, and none for a page already all marked: one a 64-byte piece cost the replay thread ~3 % of its
// samples (def3, 08/11/2026: the 1-4 MiB ranges the shadow binds are 16-64 K pieces each).
void mem_early_gpu_wrote(uint64_t a, uint64_t n, uint64_t fence) {
    if (!n || mem_early_on() <= 0 || a >= RT_ADDRESS_LIMIT) return;
    uint64_t end = a + n < RT_ADDRESS_LIMIT ? a + n : RT_ADDRESS_LIMIT;
    for (uint64_t p = a & ~(PAGE - 1); p < end; p += PAGE) {
        EarlyChunk *c = early_chunk(p, 1);
        if (!c) return;
        size_t idx = early_idx(p);
        uint64_t bits = page_bits(p, a, end);
        if ((__atomic_load_n(&c->gpu_wrote[idx], __ATOMIC_RELAXED) & bits) != bits)
            __atomic_fetch_or(&c->gpu_wrote[idx], bits, __ATOMIC_RELAXED);
    }
    if (!fence) return;
    plat_mutex_lock(&wm_lock);
    for (uint64_t p = a & ~(PAGE - 1); p < end; p += PAGE) {
        EarlyChunk *c = early_chunk(p, 0);
        if (!c) continue;
        WMask *w = &c->wmask[early_idx(p)];
        uint64_t bits = page_bits(p, a, end);
        if (w->f[0] != fence) {
            if (fence < w->f[0]) continue;   // recorded out of order: left unknown
            w->f[1] = w->f[0], w->m[1] = w->m[0], w->f[0] = fence, w->m[0] = 0;
        }
        w->m[0] |= bits;
    }
    plat_mutex_unlock(&wm_lock);
}

static int arm_watch(uint64_t lo, size_t n) {
    PlatEarlyResult result = plat_mem_early_protect(lo, n, 1, 0);
    if (result == PLAT_EARLY_UNRESTORED)
        rt_fatal("memory: early-read watch: rollback failed at %#llx (%zu bytes)", (unsigned long long)lo, n);
    return result == PLAT_EARLY_OK;
}

static void flush_watch_run(uint64_t *lo, uint64_t *hi, size_t ep) {
    if (*lo >= *hi) return;
    if (!arm_watch(*lo, (size_t)(*hi - *lo))) {
        for (uint64_t np = *lo; np < *hi; np += ep) {
            if (!arm_watch(np, ep)) {
                for (uint64_t q = np; q < np + ep && q < RT_ADDRESS_LIMIT; q += PAGE)
                    if (trapped[q / PAGE] == 2) {
                        trapped[q / PAGE] = 0;
                        mem_early_pages--;
                    }
            }
        }
    }
    *lo = *hi = 0;
}

static void flush_unprotect_run(uint64_t *lo, uint64_t *hi, int *prot) {
    if (*lo >= *hi) return;
    if (plat_mem_early_protect(*lo, (size_t)(*hi - *lo), 0, *prot) != PLAT_EARLY_OK)
        rt_fatal("memory: early-read watch: page %#llx kept no access", (unsigned long long)*lo);
    for (uint64_t p = *lo; p < *hi && p < RT_ADDRESS_LIMIT; p += PAGE)
        if (trapped[p / PAGE] == 2) {
            trapped[p / PAGE] = 0;
            mem_early_pages--;
        }
    *lo = *hi = 0;
    *prot = 0;
}

void mem_early_invalidate(uint64_t a, uint64_t n) {
    static int on = -1;
    if (on < 0) on = rt_knob("EARLY_READ_RANGE", "1");
    if (early_batch < 0) early_batch = rt_knob("EARLY_BATCH", "1");
    // (MKW_FENCE_EARLY: the writer a read waits for may be the newest one, whose bytes this rule does not look at)
    if (!n || wait_mode != 2 || !RT_KNOB_ON(on) || !watched(a, n) || !mem_fence_done || mem_fence_early) return;
    uint64_t done = mem_fence_done();
    size_t ep = plat_mem_early_page_size();
    int batch = RT_KNOB_ON(early_batch);
    uint64_t rel_lo = 0, rel_hi = 0;
    int rel_prot = 0;
    frz_take();
    for (uint64_t p = a & ~(PAGE - 1); p < a + n; p += PAGE) {
        EarlyChunk *c = early_chunk(p, 0);
        if (!c || trapped[p / PAGE] != 2) continue;
        size_t idx = early_idx(p);
        uint64_t f = c->prevf[idx];
        if (f <= done) continue;
        uint64_t lo = p > a ? p : a, hi = p + PAGE < a + n ? p + PAGE : a + n, want = 0;
        for (uint64_t x = lo & ~63ull; x < hi; x += 64) want |= 1ull << (x % PAGE / 64);
        plat_mutex_lock(&wm_lock);
        const WMask *w = &c->wmask[idx];
        int known = w->f[0] == f || w->f[1] == f;
        uint64_t m = w->f[0] == f ? w->m[0] : w->m[1];
        plat_mutex_unlock(&wm_lock);
        if (!known || (m & want)) continue;
        if (ep > PAGE) {
            // Do not drop native protection if another 4 KiB page in the same native page still needs it.
            uint64_t np = p & ~(uint64_t)(ep - 1);
            int other_live = 0;
            for (uint64_t q = np; q < np + ep && q < RT_ADDRESS_LIMIT; q += PAGE) {
                if (q != p && trapped[q / PAGE] == 2) { other_live = 1; break; }
            }
            if (other_live) {
                trapped[p / PAGE] = 0;
                mem_early_pages--;
                range_skips++;
                continue;
            }
        }
        if (!batch) {
            mem_early_release_locked(p, 0);
        } else {
            uint64_t np = p & ~(uint64_t)(ep - 1);
            int rp = (int)(c->meta[idx] & 0xf);
            if (rel_lo && (np != rel_hi || (ep == PAGE && rp != rel_prot)))
                flush_unprotect_run(&rel_lo, &rel_hi, &rel_prot);
            if (!rel_lo) {
                rel_lo = np;
                rel_hi = np + ep;
                rel_prot = rp;
            } else {
                rel_hi += ep;
            }
        }
        range_skips++;
    }
    flush_unprotect_run(&rel_lo, &rel_hi, &rel_prot);
    frz_drop();
}

void mem_early_watch(uint64_t a, uint64_t n, uint64_t fence, int kind) {
    if (!n || !mem_early_on() || !watched(a, n)) return;
    static int skip = -1;   // MKW_EARLY_READ_SKIP (default 1; "ab"): the rule below
    if (skip < 0) skip = rt_knob("EARLY_READ_SKIP", "1");
    if (early_batch < 0) early_batch = rt_knob("EARLY_BATCH", "1");
    uint64_t done = wait_mode == 2 && mem_fence_done && RT_KNOB_ON(skip) ? mem_fence_done() : 0;
    size_t ep = plat_mem_early_page_size();
    int batch = RT_KNOB_ON(early_batch);
    uint64_t w_lo = 0, w_hi = 0;
    frz_take();
    for (uint64_t p = a & ~(PAGE - 1); p < a + n; p += PAGE) {
        EarlyChunk *c = early_chunk(p, 1);
        if (!c) continue;
        size_t idx = early_idx(p);
        if (c->lastf[idx] != fence) c->prevf[idx] = c->lastf[idx], c->lastf[idx] = fence;
        // =3 waits only for the previous writer: with it finished, no read of the page can wait, and a watch would only
        // cost faults (cwb3: ~37,000 touches and 12 waits every 600 frames; the kernel went from 13 to 16-19 % of the
        // game thread).
        // (MKW_FENCE_EARLY: this fence's own bytes may be read after its early release)
        if (done && c->prevf[idx] <= done && !mem_fence_early) continue;
        uint8_t t = trapped[p / PAGE];
        if (t == 1) continue;   // the GPU's bytes of an earlier fence are still pending: a read trap already
        if (t == 3) mem_base_fill_locked(p), t = 0;   // a base's page: its bytes go to the base, then it is watched
        if (t == 4) mem_spill_fill_locked(p, 2), t = 0;   // a spilled page: back from the file, then it is watched
        // With waits, only pages the GPU has written in earlier frames (a frame repeats the last one): a bound storage
        // buffer the shader only reads costs a fault per touch, by the game and by the shadow sync (ews1: ~270 a frame).
        if (wait_mode && !c->gpu_wrote[idx]) continue;
        if (t == 2) {           // watched already: the newer submit's fence
            c->meta[idx] = fence << 8 | (c->meta[idx] & 0xff);
            continue;
        }
        uint64_t end;
        int st = plat_mem_state(p, p + PAGE, &end);
        if (st != PLAT_PROT_RW && st != PLAT_PROT_READ) continue;   // a hole or a guest protection
        uint64_t np = p & ~(uint64_t)(ep - 1);
        int already_np = 0;
        if (ep > PAGE) {
            for (uint64_t q = np; q < np + ep && q < RT_ADDRESS_LIMIT; q += PAGE)
                if (trapped[q / PAGE] == 2) { already_np = 1; break; }
        }
        trapped[p / PAGE] = 2;
        c->meta[idx] = fence << 8 | (uint64_t)kind << 4 | (uint64_t)st;
        mem_early_pages++;
        if (!already_np) {
            if (!batch) {
                if (!arm_watch(np, ep)) {
                    trapped[p / PAGE] = 0;
                    mem_early_pages--;
                    continue;
                }
            } else {
                if (w_lo && np != w_hi) flush_watch_run(&w_lo, &w_hi, ep);
                if (!w_lo) {
                    w_lo = np;
                    w_hi = np + ep;
                } else {
                    w_hi += ep;
                }
            }
        }
    }
    flush_watch_run(&w_lo, &w_hi, ep);
    frz_drop();
}

static uint64_t siblings_released;   // MKW_EARLY_SIBLING_WAIT=0: sibling-page watches given up without a wait

static uint64_t early_release_page_locked(uint64_t p, uint64_t access_a, int access, int is_fault_page,
                                          int do_protect, size_t ep) {
    EarlyChunk *c = early_chunk(p, 0);
    if (!c) {
        trapped[p / PAGE] = 0;
        mem_early_pages--;
        return 0;
    }
    size_t idx = early_idx(p);
    uint64_t m = c->meta[idx], fence = m >> 8;
    int kind = (int)(m >> 4 & 0xf);
    if (do_protect) {
        uint64_t np = p & ~(uint64_t)(ep - 1);
        if (plat_mem_early_protect(np, ep, 0, (int)(m & 0xf)) != PLAT_EARLY_OK)
            rt_fatal("memory: early-read watch: page %#llx kept no access", (unsigned long long)np);
    }
    trapped[p / PAGE] = 0;
    mem_early_pages--;
    if (!access || kind >= EW_KINDS) return 0;
    uint64_t done = mem_fence_done ? mem_fence_done() : 0;
    // The game's own code (kern_fault_in_game) is counted as the game; a guest thread in host code as the runtime.
    int game = tcur && kern_fault_in_game(plat_fault_pc);
    int early = fence > done, who = !game ? 3 : !early ? 2 : plat_fault_write;
    if (is_fault_page && tcur && !game) not_game++;
    if (is_fault_page) {
        cnt[kind][who]++;
        if (early && game && !plat_fault_write && nring < EW_RING) ring[nring].addr = access_a, ring[nring++].pc = plat_fault_pc;
    }
    uint64_t at = plat_fault_pc - plat_exe_base() < (1ull << 32) ? plat_fault_pc : plat_fault_ret;
    if (is_fault_page && !game)
        for (int j = 0; j < 8; j++)
            if (!rt_pc[j].n || rt_pc[j].pc == at) {
                rt_pc[j].pc = at, rt_pc[j].n++;
                break;
            }
    if (is_fault_page && early && game && !plat_fault_write && nfirst < EW_FIRST) {
        first[nfirst].page = p, first[nfirst].fence = fence, first[nfirst].done = done, first[nfirst].kind = kind;
        first[nfirst].frame = mem_early_frame, first[nfirst].pc = plat_fault_pc, first[nfirst].write = 0;
        nfirst++;
    }
    uint64_t gw = c->gpu_wrote[idx];
    int hit = is_fault_page ? (int)(gw >> (access_a % PAGE / 64) & 1) : (gw != 0);
    // MKW_EARLY_SIBLING_WAIT (default 1, "ab" to compare; 0: off): an access to another 4 KiB page of the watched page's
    // 16 KiB native page waits for the watched page's writer too. On the PS5 every wait of a race was such a sibling
    // (15/11/2026, wake-20261115-185222: address 0x251ee85252, watched page 0x251ee87000), ~1 a frame of ~8 ms on the
    // game's job threads (4.5-4.8 s a 600-frame window), while "storage buffer 0 early reads 0": no game read touched
    // bytes the GPU had yet to write. Off, the sibling's watch is given up without a wait (siblings_released counts
    // them): a later early read of that page would no longer be caught.
    static int sib = -1;
    if (sib < 0) sib = rt_knob("EARLY_SIBLING_WAIT", "1");
    if (!is_fault_page && hit && !RT_KNOB_ON(sib)) hit = 0, siblings_released++;
    // Only the game's own code waits (kern_fault_in_game): a runtime thread, or host code on a guest thread, releases the
    // watch and goes on. With 16 KiB native pages the wait may come from another 4 KiB page of the faulting one
    // (is_fault_page 0): the line still names the fault (run 25 logged page 0, frame 0, exe-0x400000 from that case).
    if (wait_mode && early && game && !plat_fault_write && hit) {
        uint64_t w = wait_mode == 1 ? fence : c->prevf[idx] > done ? c->prevf[idx] : 0;
        uint64_t last = c->lastf[idx];
        if (wait_mode == 2 && mem_fence_early && last > done && last <= __atomic_load_n(&mem_early_rel, __ATOMIC_ACQUIRE))
            w = last;
        if (w) wq.page = access_a & ~(PAGE - 1), wq.watched = p, wq.pc = plat_fault_pc, wq.done = done, wq.last = last,
               wq.frame = mem_early_frame, wq.addr = access_a, wq.diff = 0;
        if (is_fault_page) {
            wq.page = p, wq.watched = p, wq.pc = plat_fault_pc, wq.done = done, wq.last = last, wq.frame = mem_early_frame;
            static int dry = -1;
            if (dry < 0) {
                const char *e = cfg_get("MKW_EARLY_READ_DRY");
                dry = e && !strcmp(e, "diff") ? 3 : rt_knob("EARLY_READ_DRY", "0");
            }
            if (w && dry < 3 && RT_KNOB_ON(dry)) {
                mem_early_ev_note(0, access_a, w, done, at);
                return 0;
            }
            wq.addr = access_a, wq.diff = dry == 3;
        }
        return w;
    }
    return 0;
}

// MKW_EARLY_SIBLING_STEP (default 0: the PS5 ended the title at the first step, step-ab6-20261117-152909, no FATAL:
// its sigreturn seems to refuse the trap flag the popf self-test accepts; "ab": in the B blocks): a read by the game's code of a 4 KiB page that is
// not watched, in a 16 KiB native page with a watched one whose GPU writer has not finished, neither waits for that
// writer nor gives the watch up: the native page opens for the one instruction (plat_fault_step, the x86 trap flag)
// and closes again after it, still watched. On the console nothing waits there: the 4 KiB page is the game's own
// memory, which the GPU does not write. Every early-read wait of a race on the PS5 was such a sibling ("storage buffer
// 0 early reads": no read of a byte the GPU had yet to write), and with the queues ordered by their event waits the
// writer's fence covers ~2/3 of the main queue's draws: 13.8 s of waits per 600 frames, ~23 ms a frame summed over
// the job threads (qp-ab2-20261117-140208; pre-ab-20261117-141445 still 4.9 s for main+0x3d4f80 alone). Giving the
// watch up instead (MKW_EARLY_SIBLING_WAIT=0) measured +1.4 ms (sib-ab-20261117-111010): the stale page is no longer
// caught. Not when the access may reach the next 4 KiB page and that one is watched (64 bytes, a 512-bit load), and
// not for writes (they release the watch as before). While the page is open another thread may read the watched
// page without a fault (one instruction's window, counted as steps): it reads what the console's memory would hold
// then, without the wait the watch adds. Without single step (plat_fault_step 0) the wait is as before.
static int sib_step = -1;
static uint64_t pend_done;   // the finished fence sibling_pending saw last (frz_lock)
static uint64_t sib_steps;   // since the last log line

static void sibling_close(uint64_t np) {   // after the instruction (SIGTRAP handler, same thread, no lock held)
    size_t ep = plat_mem_early_page_size();
    frz_take();
    int any = 0;
    for (uint64_t q = np; q < np + ep && q < RT_ADDRESS_LIMIT; q += PAGE) any |= trapped[q / PAGE] == 2;
    if (any && !arm_watch(np, ep))   // (as flush_watch_run: a page that cannot be closed is watched no more)
        for (uint64_t q = np; q < np + ep && q < RT_ADDRESS_LIMIT; q += PAGE)
            if (trapped[q / PAGE] == 2) trapped[q / PAGE] = 0, mem_early_pages--;
    frz_drop();
}

// A read by the game's code of a 4 KiB page not watched, in the native page np with a watched one whose unfinished
// writer that read would wait for (as early_release_page_locked decides): 1, with the watched pages' protection to
// restore and the newest such fence. Not when the access may reach the next 4 KiB page and that one is watched (64
// bytes, a 512-bit load), and not for writes (they release the watch as before). frz_lock held.
static int sibling_pending(uint64_t a, uint64_t p, uint64_t np, size_t ep, int *prot, uint64_t *key) {
    if (!wait_mode || plat_fault_write || trapped[p / PAGE] == 2) return 0;
    if (!tcur || !kern_fault_in_game(plat_fault_pc)) return 0;
    if ((a & (PAGE - 1)) > PAGE - 64 && p + PAGE < np + ep && trapped[(p + PAGE) / PAGE] == 2) return 0;
    uint64_t done = mem_fence_done ? mem_fence_done() : 0;
    int pending = 0;
    pend_done = done;
    *prot = -1, *key = 0;
    for (uint64_t q = np; q < np + ep && q < RT_ADDRESS_LIMIT; q += PAGE) {
        if (trapped[q / PAGE] != 2) continue;
        EarlyChunk *c = early_chunk(q, 0);
        if (!c) continue;
        size_t idx = early_idx(q);
        uint64_t m = c->meta[idx], fence = m >> 8;
        if (*prot < 0) *prot = (int)(m & 0xf);
        if ((int)(m >> 4 & 0xf) < EW_KINDS && fence > done && c->gpu_wrote[idx] &&
            (wait_mode == 1 || c->prevf[idx] > done || (mem_fence_early && c->lastf[idx] > done))) {
            pending = 1;
            if (fence > *key) *key = fence;
        }
    }
    return pending;
}

static int sibling_step(uint64_t a, uint64_t p, uint64_t np, size_t ep) {   // frz_lock held
    if (sib_step < 0) sib_step = rt_knob("EARLY_SIBLING_STEP", "0");
    int prot;
    uint64_t key;
    if (!RT_KNOB_ON(sib_step) || !sibling_pending(a, p, np, ep, &prot, &key) || !plat_fault_step(sibling_close, np))
        return 0;
    if (plat_mem_early_protect(np, ep, 0, prot) != PLAT_EARLY_OK)
        rt_fatal("memory: early-read watch: page %#llx kept no access", (unsigned long long)np);
    sib_steps++;
    return 1;
}

// MKW_EARLY_SIBLING_EMU (default 1; 0 off; "ab": in the B blocks): the same sibling read (sibling_pending), emulated
// instead of stepped: the fault handler decodes the load (plat_fault_emulate_load: mov/movzx/movsx into a general
// register, which are all six reader sites of the PS5 race: mov r64 [rax+0x10], movzx r9d byte [rdi+0x4a], mov eax
// [r13+0x58a0], movzx ecx word [rax+8], mov rcx [rcx+0x100], mov r8d [rcx+0x10]; step-ab6-20261117-152909 against
// llvm-pie.elf), reads its bytes with the native page opened for the copy and closed again under frz_lock (two
// protects, ~44 us each on the PS5, against waits of 15-20 ms), writes the register and skips the instruction; the
// watch stays. The single step died on the PS5 at its first use (step-ab6): no trap flag there. At most EMU_MAX reads
// a native page for one writer fence: past that it waits as before (a loop over the page would cost more than the
// wait). Only bytes of 4 KiB pages not watched are read; anything else waits as before (sib_emu_no counts it, and the
// three sib_no_* say why). MKW_EARLY_SIBLING_CMP (default 1, "ab" to compare) also emulates CMP with a memory operand,
// MKW_EARLY_SIBLING_TEST (the same) TEST. MKW_EARLY_SIBLING_MAX replaces EMU_MAX; MKW_EARLY_SIBLING_MAX_B replaces it in the B blocks of MKW_AB, to compare two
// limits in one race. 128, not the first 32: two races each on 1.8.0 with TEST on (19/11/2026), 22.90 and 22.40 ms
// against 23.21 and 24.87; a wait costs ~15 ms, an emulated read ~90 us.
#define EMU_MAX 128
// MKW_EARLY_SIBLING_SNAP (default 1, "ab" to compare): the first emulated read of a closed native page copies all of it, and reads
// after it come from the copy with no protection change (each read otherwise costs two, ~90 us under frz_lock; some races
// make 56-79 such reads a frame). While the page stays closed only the GPU can change it, and the game may see a GPU
// write only after its fence, so a copy serves a read only while the page was not opened or re-armed
// (plat_mem_early_gen), the watched 4 KiB pages are the same, the page's newest pending fence is the same, and no fence
// has finished since. The page is opened read-only for the copy: a thread writing meanwhile faults and queues on
// frz_lock, instead of a write the copy would miss. Such reads do not count towards the limit.
// MKW_EARLY_SIBLING_SNAP_CHECK=1 (diagnosis) also reads the real bytes for each of them and counts the differences.
#define SNAP_SIZE 16384
static struct { uint64_t np, key, done; uint32_t gen; uint8_t mask; } snap[16];
static uint8_t snap_b[16][SNAP_SIZE] __attribute__((aligned(64)));
static int sib_snap, sib_snap_check, emu_k, emu_only_hit, emu_hit, emu_limited;
static uint64_t emu_key, sib_snap_hits, sib_snap_fills, sib_snap_bad;
static int sib_emu = -1, sib_cmp, sib_test, emu_max = EMU_MAX, emu_max_b;
static uint64_t sib_emus, sib_emu_no;   // since the last log line
static uint64_t sib_no_max, sib_no_insn, sib_no_bytes;   // not emulated: the limit, the instruction, the bytes
static uint64_t sib_no_max_b;   // of sib_no_max, in the B blocks of MKW_AB
static int emu_refused;   // sibling_read said no to this read
static struct { uint64_t np, key; int n; } emu_seen[16];
static uint64_t emu_np;   // the native page sibling_read may open (frz_lock)
static int emu_prot;

static void emu_close(size_t ep) {
    if (!arm_watch(emu_np, ep))   // (as flush_watch_run: a page that cannot be closed is watched no more)
        for (uint64_t q = emu_np; q < emu_np + ep && q < RT_ADDRESS_LIMIT; q += PAGE)
            if (trapped[q / PAGE] == 2) trapped[q / PAGE] = 0, mem_early_pages--;
}

static unsigned snap_mask(uint64_t np, size_t ep) {
    unsigned m = 0;
    for (size_t i = 0; i < ep / PAGE && np + i * PAGE < RT_ADDRESS_LIMIT; i++)
        if (trapped[(np + i * PAGE) / PAGE] == 2) m |= 1u << i;
    return m;
}

// 1: dst holds the bytes from a page copy. 0: go on the old way (or, at the limit, emu_limited: no read at all).
static int snap_read(uint64_t ea, size_t n, void *dst, size_t ep) {
    __typeof__(snap[0]) *s = &snap[emu_k];
    uint8_t *b = snap_b[emu_k];
    size_t off = (size_t)(ea - emu_np);
    unsigned mask = snap_mask(emu_np, ep);
    if (s->np == emu_np && s->key == emu_key && s->done == pend_done && s->mask == mask &&
        s->gen == plat_mem_early_gen(emu_np)) {
        if (sib_snap_check && plat_mem_early_open_read(emu_np, ep)) {
            if (memcmp(b + off, (const void *)(uintptr_t)ea, n)) sib_snap_bad++;
            emu_close(ep);
            s->gen = plat_mem_early_gen(emu_np);
        }
        memcpy(dst, b + off, n);
        emu_hit = 1, sib_snap_hits++;
        return 1;
    }
    if (emu_only_hit) return emu_limited = 1, 0;
    s->np = 0;
    if (!plat_mem_early_open_read(emu_np, ep)) return 0;
    uint32_t gen = plat_mem_early_gen(emu_np);
    memcpy(b, (const void *)(uintptr_t)emu_np, ep);
    emu_close(ep);
    if (plat_mem_early_gen(emu_np) == gen + 1 && snap_mask(emu_np, ep) == mask)   // (+1: the close just made)
        s->np = emu_np, s->key = emu_key, s->done = pend_done, s->gen = gen + 1, s->mask = (uint8_t)mask;
    memcpy(dst, b + off, n);
    sib_snap_fills++;
    return 1;
}

static int sibling_read(uint64_t ea, size_t n, void *dst) {   // frz_lock held (inside mem_early_release_locked)
    size_t ep = plat_mem_early_page_size();
    emu_refused = 1;
    if (ea < emu_np || ea + n > emu_np + ep) return 0;
    for (uint64_t q = ea & ~(PAGE - 1); q < ea + n; q += PAGE)
        if (trapped[q / PAGE] == 2) return 0;
    emu_refused = 0;
    if (ep == SNAP_SIZE && RT_KNOB_ON(sib_snap)) {
        if (snap_read(ea, n, dst, ep)) return 1;
        if (emu_limited) return 0;
    }
    if (plat_mem_early_protect(emu_np, ep, 0, emu_prot) != PLAT_EARLY_OK)
        rt_fatal("memory: early-read watch: page %#llx kept no access", (unsigned long long)emu_np);
    memcpy(dst, (const void *)(uintptr_t)ea, n);
    emu_close(ep);
    return 1;
}

// Once, from mem_early_on: the knob, and whether the decoder can read the game's code (plat_code_init: on the PS5
// from /app0/eboot.bin, whose text is execute-only in memory). Off, with a line, when it cannot: never a crash.
static void sibling_emu_init(void) {
    if (plat_mem_early_page_size() == PAGE) return (void)(sib_emu = 0);   // (no siblings)
    sib_emu = rt_knob("EARLY_SIBLING_EMU", "1");
    if (!sib_emu) return;
    sib_cmp = rt_knob("EARLY_SIBLING_CMP", "1");
    sib_test = rt_knob("EARLY_SIBLING_TEST", "1");
    sib_snap = rt_knob("EARLY_SIBLING_SNAP", "1");
    sib_snap_check = rt_knob("EARLY_SIBLING_SNAP_CHECK", "0");
    const char *max = cfg_get("MKW_EARLY_SIBLING_MAX"), *max_b = cfg_get("MKW_EARLY_SIBLING_MAX_B");
    if (max && *max) emu_max = atoi(max);
    if (max_b && *max_b) emu_max_b = atoi(max_b);
    int ok = plat_code_init();
    rt_log("mem: sibling-page reads emulated%s (MKW_EARLY_SIBLING_EMU): the code-read check %s", sib_emu == 2 ? " in the B blocks" : "",
           ok ? "passed" : "failed, so they wait as before");
    if (!ok) sib_emu = 0;
}

static int sibling_emu(uint64_t a, uint64_t p, uint64_t np, size_t ep) {   // frz_lock held
    int prot;
    uint64_t key;
    if (sib_emu <= 0 || !RT_KNOB_ON(sib_emu) || !sibling_pending(a, p, np, ep, &prot, &key)) return 0;
    int k = (int)((np / ep) % 16);
    if (emu_seen[k].np != np || emu_seen[k].key != key) emu_seen[k].np = np, emu_seen[k].key = key, emu_seen[k].n = 0;
    int in_b = rt_ab(), at_max = emu_seen[k].n >= (in_b && emu_max_b > 0 ? emu_max_b : emu_max);
    if (at_max && !(ep == SNAP_SIZE && RT_KNOB_ON(sib_snap))) return sib_no_max++, sib_no_max_b += in_b, sib_emu_no++, 0;
    emu_np = np, emu_prot = prot, emu_refused = 0, emu_k = k, emu_key = key;
    emu_only_hit = at_max, emu_hit = 0, emu_limited = 0;
    int extra = (RT_KNOB_ON(sib_cmp) ? PLAT_EMU_CMP : 0) | (RT_KNOB_ON(sib_test) ? PLAT_EMU_TEST : 0);
    if (!plat_fault_emulate_load(a, extra, sibling_read)) {
        if (emu_limited) sib_no_max++, sib_no_max_b += in_b;
        else if (emu_refused) sib_no_bytes++;
        else sib_no_insn++;
        return sib_emu_no++, 0;
    }
    emu_seen[k].n += !emu_hit, sib_emus++;
    return 1;
}

uint64_t mem_early_release_locked(uint64_t a, int access) {
    size_t ep = plat_mem_early_page_size();
    uint64_t p = a & ~(PAGE - 1);
    if (ep == PAGE) return early_release_page_locked(p, a, access, 1, 1, PAGE);
    uint64_t np = a & ~(uint64_t)(ep - 1);
    if (access && (sibling_emu(a, p, np, ep) || sibling_step(a, p, np, ep))) return 0;   // (the watch stays)
    uint64_t max_w = 0;
    int did_protect = 0;
    for (uint64_t q = np; q < np + ep && q < RT_ADDRESS_LIMIT; q += PAGE) {
        if (trapped[q / PAGE] != 2) continue;
        uint64_t w = early_release_page_locked(q, a, access, q == p, !did_protect, ep);
        did_protect = 1;
        if (w > max_w) max_w = w;
    }
    return max_w;
}

void mem_early_resolve_hold(uint64_t a, size_t n) {
    if (!n || mem_early_on() <= 0 || !__atomic_load_n(&mem_early_pages, __ATOMIC_RELAXED) || !watched(a, n))
        return;
    if (early_batch < 0) early_batch = rt_knob("EARLY_BATCH", "1");
    int batch = RT_KNOB_ON(early_batch);
    size_t ep = plat_mem_early_page_size();
    uint64_t lo = a & ~(uint64_t)(ep - 1);
    uint64_t hi = n > UINT64_MAX - a ? RT_ADDRESS_LIMIT : (a + n + ep - 1) & ~(uint64_t)(ep - 1);
    if (hi > RT_ADDRESS_LIMIT) hi = RT_ADDRESS_LIMIT;
    uint64_t rel_lo = 0, rel_hi = 0;
    int rel_prot = 0;
    frz_take();
    for (uint64_t p = lo; p < hi && mem_early_pages; p += ep) {
        int has = 0, rp = 0;
        for (uint64_t q = p; q < p + ep && q < RT_ADDRESS_LIMIT; q += PAGE)
            if (trapped[q / PAGE] == 2) {
                if (!has) {
                    EarlyChunk *c = early_chunk(q, 0);
                    rp = c ? (int)(c->meta[early_idx(q)] & 0xf) : 0;
                    has = 1;
                }
                break;
            }
        if (!has) continue;
        if (!batch) {
            mem_early_release_locked(p, 0);
            continue;
        }
        if (rel_lo && (p != rel_hi || (ep == PAGE && rp != rel_prot)))
            flush_unprotect_run(&rel_lo, &rel_hi, &rel_prot);
        if (!rel_lo) {
            rel_lo = p;
            rel_hi = p + ep;
            rel_prot = rp;
        } else {
            rel_hi += ep;
        }
    }
    flush_unprotect_run(&rel_lo, &rel_hi, &rel_prot);
    frz_drop();
}

void mem_early_log(uint64_t frame) {   // every frame
    mem_early_frame = frame;
    mem_early_ev_log();
    if (mem_early_on() <= 0 || frame % 600) return;
    char b[1024];
    int k = 0;
    frz_take();
    for (int i = 0; i < EW_KINDS && k < (int)sizeof b - 96; i++)
        if (cnt[i][0] || cnt[i][1] || cnt[i][2] || cnt[i][3])
            k += snprintf(b + k, sizeof b - (size_t)k, "; %s %lld early reads %lld early writes %lld late %lld runtime",
                          kind_name[i], (long long)cnt[i][0], (long long)cnt[i][1], (long long)cnt[i][2], (long long)cnt[i][3]);
    char ev[160];
    mem_early_ev_counts(ev, sizeof ev);
    rt_log("mem: early reads at frame %llu (%lld pages watched now)%s; waits %lld, %.1f ms in all, %lld given up (fence "
           "not handed to the queue in %llu ms); touches by host code on game threads %lld (no wait); released by an "
           "invalidate of other bytes %lld; sibling-page watches given up without a wait %llu; sibling reads let through "
           "(MKW_EARLY_SIBLING_STEP) %llu, emulated (MKW_EARLY_SIBLING_EMU) %llu, not emulated %llu (at the limit %llu, %llu of them in B "
           "blocks, instruction not handled %llu, bytes on a watched page %llu; from page copies %llu, copies made %llu, "
           "differing %llu); %s",
           (unsigned long long)frame, (long long)mem_early_pages, k ? b : ": none", (long long)waits,
           (double)wait_ns / 1e6, (long long)not_issued, (unsigned long long)(ISSUE_WAIT_NS / 1000000),
           (long long)not_game, (long long)range_skips, (unsigned long long)siblings_released,
           (unsigned long long)sib_steps, (unsigned long long)sib_emus, (unsigned long long)sib_emu_no,
           (unsigned long long)sib_no_max, (unsigned long long)sib_no_max_b, (unsigned long long)sib_no_insn, (unsigned long long)sib_no_bytes,
           (unsigned long long)sib_snap_hits, (unsigned long long)sib_snap_fills, (unsigned long long)sib_snap_bad, ev);
    waits = wait_ns = range_skips = not_game = not_issued = 0;
    siblings_released = 0, sib_steps = 0, sib_emus = 0, sib_emu_no = 0;
    sib_no_max = sib_no_max_b = sib_no_insn = sib_no_bytes = 0;
    sib_snap_hits = sib_snap_fills = sib_snap_bad = 0;
    char wr[1024];
    int wo = 0;
    plat_mutex_lock(&wread_lock);
    for (int n = 0; n < WREAD_N; n++) {   // the readers by time, the longest first
        int best = -1;
        for (int j = 0; j < WREAD_N; j++)
            if (wread[j].n && (best < 0 || wread[j].ns > wread[best].ns)) best = j;
        if (best < 0) break;
        char code[192];
        ldr_describe(wread[best].pc, code, sizeof code);
        if (wo < (int)sizeof wr - 64)
            wo += snprintf(wr + wo, sizeof wr - (size_t)wo, "; %s: %lld, %.1f ms (%lld for another 4 KiB page), last at %#llx",
                           code, (long long)wread[best].n, (double)wread[best].ns / 1e6, (long long)wread[best].sib,
                           (unsigned long long)wread[best].addr);
        wread[best].n = 0;
    }
    if (wo || wread_other)
        rt_log("mem: early-read waits at frame %llu by reader%s; %lld more by other code", (unsigned long long)frame,
               wo ? wr : "", (long long)wread_other);
    memset(wread, 0, sizeof wread);
    wread_other = 0;
    plat_mutex_unlock(&wread_lock);
    // The early reads by whether the GPU writes the page (as far as the replay has reported), with the code making them.
    uint32_t wrote = 0, page_only = 0;
    uint64_t pcs[8] = {0}, pcn[8] = {0};
    for (uint32_t i = 0; i < nring; i++) {
        EarlyChunk *c = early_chunk(ring[i].addr, 0);
        uint64_t m = c ? c->gpu_wrote[early_idx(ring[i].addr)] : 0;
        if (!(m >> (ring[i].addr % PAGE / 64) & 1)) {
            page_only += m != 0;
            continue;
        }
        wrote++;
        for (int j = 0; j < 8; j++)
            if (!pcn[j] || pcs[j] == ring[i].pc) {
                pcs[j] = ring[i].pc, pcn[j]++;
                break;
            }
    }
    char w[512];
    int o = 0;
    for (int j = 0; j < 8 && pcn[j] && o < (int)sizeof w - 48; j++)
        o += snprintf(w + o, sizeof w - (size_t)o, " exe+%#llx x%llu", (unsigned long long)(pcs[j] - plat_exe_base()),
                      (unsigned long long)pcn[j]);
    rt_log("mem: early reads of bytes the GPU writes: %u of %u (%u more of other bytes in pages it writes)%s%s", wrote,
           nring, page_only, wrote ? ", by" : "", o ? w : "");
    nring = 0;
    o = 0;
    for (int j = 0; j < 8 && rt_pc[j].n && o < (int)sizeof w - 48; j++)
        o += snprintf(w + o, sizeof w - (size_t)o, " exe+%#llx x%llu", (unsigned long long)(rt_pc[j].pc - plat_exe_base()),
                      (unsigned long long)rt_pc[j].n);
    if (o) rt_log("mem: early-read watch: the runtime's touches by%s", w);
    memset(rt_pc, 0, sizeof rt_pc);
    for (int i = 0; i < nfirst; i++)
        rt_log("mem: early read: page %#llx (%s) at frame %llu, fence %llu, GPU at %llu, by exe+%#llx", (unsigned long long)first[i].page,
               kind_name[first[i].kind], (unsigned long long)first[i].frame, (unsigned long long)first[i].fence,
               (unsigned long long)first[i].done, (unsigned long long)(first[i].pc - plat_exe_base()));
    memset(cnt, 0, sizeof cnt);
    nfirst = 0;
    frz_drop();
}

int mem_early_waits(void) { return mem_early_on() > 0 && wait_mode; }
