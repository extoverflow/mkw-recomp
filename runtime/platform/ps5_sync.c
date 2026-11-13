// PS5 implementation of platform.h, time, locks and condition variables: linux_sync.c (8cbbfe1) with FreeBSD's
// _umtx_op where Linux has futex, the ops PS5_Mesa's RADV runs on the console (src/util/futex.c, 0f86253). The code
// map and the differences: kept outside this repo.
#include <errno.h>
#include <limits.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/umtx.h>
#include <time.h>

#include "platform.h"

// ---- time

uint64_t plat_now_ns(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (uint64_t)t.tv_sec * 1000000000u + (uint64_t)t.tv_nsec;
}

uint64_t plat_counter(void) { return plat_now_ns(); }
uint64_t plat_counter_hz(void) { return 1000000000u; }

// Linux shortens its timer slack here; FreeBSD has no timer slack to set.
void plat_timer_1ms(void) {}

// The last PLAT_SPIN_NS before the deadline are spent yielding so it is met closely, as on Linux and Windows.
#define PLAT_SPIN_NS 100000u

void plat_sleep_ns(uint64_t ns) {
    uint64_t end = plat_now_ns() + ns;
    if (ns > PLAT_SPIN_NS) {
        uint64_t at = end - PLAT_SPIN_NS;
        struct timespec ts = {(time_t)(at / 1000000000u), (long)(at % 1000000000u)};
        while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL) == EINTR) {}
    }
    while (plat_now_ns() < end) sched_yield();
}

void plat_wait_ns(uint64_t ns) {
    struct timespec ts = {(time_t)(ns / 1000000000u), (long)(ns % 1000000000u)};
    while (nanosleep(&ts, &ts) == -1 && errno == EINTR) {}
}

void plat_sleep_ms(uint32_t ms) {
    struct timespec ts = {(time_t)(ms / 1000), (long)(ms % 1000) * 1000000};
    while (nanosleep(&ts, &ts) == -1 && errno == EINTR) {}
}

// ---- locks: a reader-writer lock in one 32-bit word, zero when free (linux_sync.c's, on _umtx_op).
// bit 31: a writer holds it; bit 30: some thread sleeps on the word; bits 0-29: readers holding it.
// A thread that cannot take the lock sets the sleeper bit and waits on the futex; every release that finds the bit
// clears it and wakes all sleepers, which retry (and set it again if they still cannot get in). Readers do not enter
// while the sleeper bit is set, so a waiting writer is not starved by a stream of readers.

#define LK_W 0x80000000u
#define LK_S 0x40000000u
#define LK_R 0x3fffffffu
#define LK_SPINS 64

// Linux's FUTEX_WAIT and FUTEX_WAKE on FreeBSD: sleep while *w equals val (ts: relative, NULL for none), -1 with errno
// (ETIMEDOUT, EINTR) as Linux returns; wake up to n sleepers.
static int futex_wait(volatile uint32_t *w, uint32_t val, const struct timespec *ts) {
    struct _umtx_time t = {._flags = 0, ._clockid = CLOCK_MONOTONIC};
    if (!ts) return _umtx_op((void *)w, UMTX_OP_WAIT_UINT, val, NULL, NULL);
    t._timeout = *ts;
    return _umtx_op((void *)w, UMTX_OP_WAIT_UINT, val, (void *)(uintptr_t)sizeof t, &t);
}

static void futex_wake(volatile uint32_t *w, int n) { _umtx_op((void *)w, UMTX_OP_WAKE, (u_long)n, NULL, NULL); }

static inline volatile uint32_t *lk(PlatMutex *m) { return (volatile uint32_t *)&m->p; }

// Sets the sleeper bit on s and sleeps while the word still equals the result.
static void lk_sleep(volatile uint32_t *w, uint32_t s) {
    if (!(s & LK_S) && !__atomic_compare_exchange_n(w, &s, s | LK_S, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED)) return;
    futex_wait(w, s | LK_S, NULL);
}

static void lk_wake(volatile uint32_t *w) {
    __atomic_fetch_and(w, ~LK_S, __ATOMIC_RELAXED);
    futex_wake(w, INT_MAX);
}

void plat_mutex_init(PlatMutex *m) { m->p = 0; }

int plat_mutex_trylock(PlatMutex *m) {
    volatile uint32_t *w = lk(m);
    uint32_t s = __atomic_load_n(w, __ATOMIC_RELAXED);
    while (!(s & (LK_W | LK_R)))
        if (__atomic_compare_exchange_n(w, &s, s | LK_W, 1, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)) return 1;
    return 0;
}

// MKW_LOCK_PROF=1: time from finding a lock taken to getting it, per lock, for plat_lock_report (which locks the
// replay waits on: its recording spent 25-30 % of its wall time off the CPU, rt-k0/rt-k1). Off: one load per contention.
// Also how long a holder kept a lock that some thread slept waiting for, per the holder's caller: the waits name who
// waited, not which section to shorten (shadow_lock's ~1 ms waits at the fence, aq-on2, 06/11/2026).
#define LP_N 512
typedef struct { volatile uintptr_t a, ra, lk; volatile int64_t ns, n; } LpEnt;   // a: key of (lock, caller)
static LpEnt lp[LP_N], lp_hold[LP_N];
static int lp_on = -1;

// The locks this thread holds exclusively since MKW_LOCK_PROF found its first contention, with when and by whom.
#define LH_DEPTH 16
static __thread struct { const void *m; uintptr_t ra; uint64_t t; } lh[LH_DEPTH];
static __thread int nlh;

static uint64_t lp_begin(void) {
    if (lp_on < 0) {
        const char *e = getenv("MKW_LOCK_PROF");
        lp_on = e && *e && *e != '0';
    }
    return lp_on ? plat_now_ns() : 0;
}

static void lp_add(LpEnt *lp, const void *m, uint64_t t0, uintptr_t ra) {
    uintptr_t a = (uintptr_t)m, key = a ^ ra * 0x9E3779B97F4A7C15ull;
    uint64_t d = plat_now_ns() - t0;
    for (uint32_t i = (uint32_t)((key >> 3) * 0x9E3779B1u) % LP_N, k = 0; k < LP_N; k++, i = (i + 1) % LP_N) {
        uintptr_t cur = __atomic_load_n(&lp[i].a, __ATOMIC_RELAXED);
        if (!cur && __atomic_compare_exchange_n(&lp[i].a, &cur, key, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED))
            cur = key, lp[i].ra = ra, lp[i].lk = a;
        if (cur == key && lp[i].ra == ra) {
            __atomic_fetch_add(&lp[i].ns, (int64_t)d, __ATOMIC_RELAXED), __atomic_fetch_add(&lp[i].n, 1, __ATOMIC_RELAXED);
            return;
        }
    }
}

static void lh_push(const void *m, uintptr_t ra) {
    if (nlh == LH_DEPTH) memmove(lh, lh + 1, sizeof lh - sizeof *lh), nlh--;   // one released by another thread
    lh[nlh].m = m, lh[nlh].ra = ra, lh[nlh++].t = plat_now_ns();
}

static void lh_pop(const void *m, int slept) {
    for (int i = nlh - 1; i >= 0; i--)
        if (lh[i].m == m) {
            if (slept) lp_add(lp_hold, m, lh[i].t, lh[i].ra);
            lh[i] = lh[--nlh];
            return;
        }
}

int plat_lock_report(int holds, char *buf, int size) {
    LpEnt *t = holds ? lp_hold : lp;
    int k = 0;
    for (int top = 0; top < 10; top++) {   // the ten longest waits since the last report, then reset
        int best = -1;
        for (int i = 0; i < LP_N; i++)
            if (t[i].a && t[i].ns > 0 && (best < 0 || t[i].ns > t[best].ns)) best = i;
        if (best < 0) break;
        if (k < size)
            k += snprintf(buf + k, (size_t)(size - k), "%s exe+%#lx from exe+%#lx %.1f ms (%lld)", top ? ";" : "",
                          (unsigned long)(t[best].lk - plat_exe_base()), (unsigned long)(t[best].ra - plat_exe_base()),
                          t[best].ns / 1e6, (long long)t[best].n);
        t[best].ns = -t[best].ns;   // taken; restored to zero below
    }
    for (int i = 0; i < LP_N; i++) t[i].ns = 0, t[i].n = 0;
    return k;
}

static inline __attribute__((always_inline)) void lock_at(PlatMutex *m, uintptr_t ra) {
    volatile uint32_t *w = lk(m);
    uint64_t t0 = 0;
    for (int spin = 0;; spin++) {
        uint32_t s = __atomic_load_n(w, __ATOMIC_RELAXED);
        if (!(s & (LK_W | LK_R))) {
            if (__atomic_compare_exchange_n(w, &s, s | LK_W, 1, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)) {
                if (t0) lp_add(lp, m, t0, ra);
                if (lp_on > 0) lh_push(m, ra);
                return;
            }
            continue;
        }
        if (!spin) t0 = lp_begin();
        if (spin < LK_SPINS) plat_pause();
        else lk_sleep(w, s);
    }
}

void plat_mutex_lock(PlatMutex *m) { lock_at(m, (uintptr_t)__builtin_return_address(0)); }
void plat_mutex_lock_from(PlatMutex *m, uintptr_t ra) { lock_at(m, ra); }

void plat_mutex_unlock(PlatMutex *m) {
    volatile uint32_t *w = lk(m);
    uint32_t s = __atomic_fetch_and(w, ~LK_W, __ATOMIC_RELEASE);
    if (lp_on > 0) lh_pop(m, (s & LK_S) != 0);
    if (s & LK_S) lk_wake(w);
}

int plat_mutex_trylock_shared(PlatMutex *m) {
    volatile uint32_t *w = lk(m);
    uint32_t s = __atomic_load_n(w, __ATOMIC_RELAXED);
    while (!(s & (LK_W | LK_S)))
        if (__atomic_compare_exchange_n(w, &s, s + 1, 1, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)) return 1;
    return 0;
}

void plat_mutex_lock_shared(PlatMutex *m) {
    volatile uint32_t *w = lk(m);
    uint64_t t0 = 0;
    for (int spin = 0;; spin++) {
        uint32_t s = __atomic_load_n(w, __ATOMIC_RELAXED);
        if (!(s & (LK_W | LK_S))) {
            if (__atomic_compare_exchange_n(w, &s, s + 1, 1, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)) {
                if (t0) lp_add(lp, m, t0, (uintptr_t)__builtin_return_address(0));
                return;
            }
            continue;
        }
        if (!spin) t0 = lp_begin();
        if (spin < LK_SPINS) plat_pause();
        else lk_sleep(w, s);
    }
}

void plat_mutex_unlock_shared(PlatMutex *m) {
    volatile uint32_t *w = lk(m);
    uint32_t s = __atomic_sub_fetch(w, 1, __ATOMIC_RELEASE);
    if (!(s & LK_R) && (s & LK_S)) lk_wake(w);
}

// ---- condition variables: a sequence number in the first 32 bits. A waiter reads it before releasing the lock and
// sleeps only while it is unchanged, so a signal between the release and the sleep is not lost.

static inline volatile uint32_t *cv(PlatCond *c) { return (volatile uint32_t *)&c->p; }

void plat_cond_init(PlatCond *c) { c->p = 0; }

void plat_cond_wait(PlatCond *c, PlatMutex *m) {
    uint32_t seq = __atomic_load_n(cv(c), __ATOMIC_RELAXED);
    plat_mutex_unlock(m);
    futex_wait(cv(c), seq, NULL);
    plat_mutex_lock(m);
}

int plat_cond_wait_ns(PlatCond *c, PlatMutex *m, uint64_t ns) {
    uint32_t seq = __atomic_load_n(cv(c), __ATOMIC_RELAXED);
    struct timespec ts = {(time_t)(ns / 1000000000u), (long)(ns % 1000000000u)};
    plat_mutex_unlock(m);
    int r = futex_wait(cv(c), seq, &ts);
    int timed_out = r == -1 && errno == ETIMEDOUT;
    plat_mutex_lock(m);
    return !timed_out;
}

void plat_cond_signal(PlatCond *c) {
    __atomic_add_fetch(cv(c), 1, __ATOMIC_RELEASE);
    futex_wake(cv(c), 1);
}

void plat_cond_broadcast(PlatCond *c) {
    __atomic_add_fetch(cv(c), 1, __ATOMIC_RELEASE);
    futex_wake(cv(c), INT_MAX);
}
