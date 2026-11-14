/*
 * PS5 title: the heap's wraps of libc's allocator, with a sampled record of the live allocations by call site.
 * The wraps are copied from mihawk-99/PS5_PayloadSDK platform/src/heap_wrap.c @ 3345cde (lines 1-200):
 *   Copyright (C) 2026 Mihawk
 *   SPDX-License-Identifier: GPL-3.0-or-later
 * This file: GPL-3.0-or-later (the port's PS5 code, 13/11/2026).
 *
 * Linked in place of the platform library's heap_wrap.o (this object defines every symbol that member does, so the
 * archive member is never pulled). Why: the title's heap grows ~6 MiB a minute in races (run 63: 485 -> 822 MiB in
 * use over 55 min) and nothing says whose memory it is. MKW_PS5_ALLOC_TRACE=1 (args.txt; read by ps5_main.c, which
 * sets ps5_alloc_trace_on): one allocation in 64 under 64 KiB and every larger one is recorded with its call site
 * (the return address and the next code addresses on the stack); frees drop their record; ps5_alloc_trace_report
 * prints the sites holding the most (estimated) live bytes. Off: one branch per call.
 */
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

void *ps5_heap_malloc(size_t bytes);
void *ps5_heap_calloc(size_t count, size_t bytes);
void *ps5_heap_realloc(void *pointer, size_t bytes);
void *ps5_heap_memalign(size_t alignment, size_t bytes);
void ps5_heap_free(void *pointer);
int ps5_heap_owns(const void *pointer);
size_t ps5_heap_usable_size(const void *pointer);
void ps5p_heap_count_fallback(void);

void *__real_malloc(size_t bytes);
void *__real_calloc(size_t count, size_t bytes);
void *__real_realloc(void *pointer, size_t bytes);
void __real_free(void *pointer);
int __real_posix_memalign(void **out, size_t alignment, size_t bytes);
__attribute__((weak)) size_t __real_malloc_usable_size(const void *pointer);

// ---- the sampled record
int ps5_alloc_trace_on;
extern const char __ehdr_start[];
extern const char mkw_text_end[] __attribute__((weak));

#define AT_SITES 4                   // return address + 3 code addresses found above it on the stack
#define AT_CAP (1u << 18)            // records (open addressing; 48 bytes each: 12 MiB, made at the first record)
#define AT_STRIPES 64
#define AT_EVERY 64                  // one small allocation in AT_EVERY
#define AT_BIG (64u << 10)           // this size and over: every one
typedef struct { uintptr_t p; uint32_t size, weight; uintptr_t site[AT_SITES]; } AtRec;
#define AT_EMPTY 0u
#define AT_TOMB 1u                   // a removed record (heap pointers are never 1)
static AtRec *at_tab;
static volatile int at_lock[AT_STRIPES];
static volatile uint64_t at_count, at_dropped;

static void at_acquire(volatile int *l) { while (__atomic_exchange_n(l, 1, __ATOMIC_ACQUIRE)) __builtin_ia32_pause(); }
static void at_release(volatile int *l) { __atomic_store_n(l, 0, __ATOMIC_RELEASE); }
static uint32_t at_hash(uintptr_t p) { return (uint32_t)((p >> 4) * 0x9E3779B97F4A7C15ull >> 40) & (AT_CAP - 1); }
// A stripe covers whole runs of slots so a probe that stays in its run needs one lock: the probe stops at the run end.
#define AT_RUN (AT_CAP / AT_STRIPES)

static void __attribute__((noinline)) at_sites(uintptr_t *site, uintptr_t ret) {
    uintptr_t lo = (uintptr_t)__ehdr_start, hi = mkw_text_end ? (uintptr_t)mkw_text_end : lo;
    site[0] = ret;
    int k = 1;
    const uintptr_t *w = (const uintptr_t *)__builtin_frame_address(0);
    for (int i = 0; i < 48 && k < AT_SITES; i++)
        if (w[i] >= lo && w[i] < hi && w[i] != ret) site[k++] = w[i];
    while (k < AT_SITES) site[k++] = 0;
}

static void at_add(void *p, size_t size, uintptr_t ret) {
    if (!p) return;
    uint32_t weight = 1;
    if (size < AT_BIG) {
        if (__atomic_add_fetch(&at_count, 1, __ATOMIC_RELAXED) % AT_EVERY) return;
        weight = AT_EVERY;
    }
    if (!at_tab) {   // made once, from the platform heap's libc fallback: never traced itself
        static volatile int made;
        if (!__atomic_exchange_n(&made, 1, __ATOMIC_ACQ_REL)) {
            AtRec *t = __real_calloc(AT_CAP, sizeof *t);
            __atomic_store_n(&at_tab, t, __ATOMIC_RELEASE);
        }
        if (!__atomic_load_n(&at_tab, __ATOMIC_ACQUIRE)) return;
    }
    uintptr_t site[AT_SITES];
    at_sites(site, ret);
    uint32_t h = at_hash((uintptr_t)p), run = h / AT_RUN, end = (run + 1) * AT_RUN;
    at_acquire(&at_lock[run]);
    uint32_t i = h;
    while (i < end && at_tab[i].p > AT_TOMB) i++;   // an empty slot or a tombstone takes it
    if (i < end) {
        at_tab[i].p = (uintptr_t)p, at_tab[i].size = size > UINT32_MAX ? UINT32_MAX : (uint32_t)size, at_tab[i].weight = weight;
        memcpy(at_tab[i].site, site, sizeof site);
    } else __atomic_add_fetch(&at_dropped, 1, __ATOMIC_RELAXED);
    at_release(&at_lock[run]);
}

static void at_del(void *p) {
    AtRec *t = __atomic_load_n(&at_tab, __ATOMIC_ACQUIRE);
    if (!p || !t) return;
    uint32_t h = at_hash((uintptr_t)p), run = h / AT_RUN, end = (run + 1) * AT_RUN;
    at_acquire(&at_lock[run]);
    for (uint32_t i = h; i < end && t[i].p != AT_EMPTY; i++)   // linear probe: an empty slot ends it
        if (t[i].p == (uintptr_t)p) {
            t[i].p = AT_TOMB;   // a tombstone: later records may sit past it
            break;
        }
    at_release(&at_lock[run]);
}

#define RET ((uintptr_t)__builtin_return_address(0))
#define TRACE_ADD(p, n) do { if (ps5_alloc_trace_on) at_add((p), (n), RET); } while (0)
#define TRACE_DEL(p) do { if (ps5_alloc_trace_on) at_del(p); } while (0)

// The sites holding the most live bytes (estimates: small allocations count AT_EVERY times), one line each to fd.
int ps5_alloc_trace_report(FILE *f, int top) {
    AtRec *t = __atomic_load_n(&at_tab, __ATOMIC_ACQUIRE);
    if (!ps5_alloc_trace_on || !t) return 0;
    enum { NS = 8192 };
    static struct { uintptr_t site[AT_SITES]; uint64_t bytes, n; } s[NS];
    memset(s, 0, sizeof s);
    uint64_t total = 0, recs = 0;
    for (uint32_t run = 0; run < AT_STRIPES; run++) {
        at_acquire(&at_lock[run]);
        for (uint32_t i = run * AT_RUN; i < (run + 1) * AT_RUN; i++) {
            if (t[i].p <= AT_TOMB) continue;
            uint64_t b = (uint64_t)t[i].size * t[i].weight;
            total += b, recs++;
            uint64_t h = 1469598103934665603ull;
            for (int k = 0; k < AT_SITES; k++) h = (h ^ t[i].site[k]) * 1099511628211ull;
            uint32_t j = (uint32_t)(h % NS);
            for (int probe = 0; probe < NS; probe++, j = (j + 1) % NS)
                if (!s[j].n || !memcmp(s[j].site, t[i].site, sizeof s[j].site)) break;
            if (!s[j].n) memcpy(s[j].site, t[i].site, sizeof s[j].site);
            s[j].bytes += b, s[j].n += t[i].weight;
        }
        at_release(&at_lock[run]);
    }
    uintptr_t base = (uintptr_t)__ehdr_start;
    fprintf(f, "[host] alloc trace: ~%.1f MiB live in sampled records (%llu records, %llu dropped); top sites (offsets from the "
               "image start):\n", total / 1048576.0, (unsigned long long)recs, (unsigned long long)at_dropped);
    for (int r = 0; r < top; r++) {
        int best = -1;
        for (int j = 0; j < NS; j++)
            if (s[j].n && (best < 0 || s[j].bytes > s[best].bytes)) best = j;
        if (best < 0) break;
        fprintf(f, "[host] alloc site %2d: ~%.1f MiB in ~%llu blocks:", r + 1, s[best].bytes / 1048576.0,
                (unsigned long long)s[best].n);
        for (int k = 0; k < AT_SITES; k++)
            if (s[best].site[k]) fprintf(f, " %#llx", (unsigned long long)(s[best].site[k] - base));
        fprintf(f, "\n");
        s[best].n = 0;
    }
    return 1;
}

// ---- the wraps (heap_wrap.c @ 3345cde, with TRACE_ADD/TRACE_DEL added)
void *__wrap_malloc(size_t bytes) {
    void *const pointer = ps5_heap_malloc(bytes);
    if (pointer) {
        TRACE_ADD(pointer, bytes);
        return pointer;
    }
    ps5p_heap_count_fallback();
    return __real_malloc(bytes);
}

void *__wrap_calloc(size_t count, size_t bytes) {
    void *const pointer = ps5_heap_calloc(count, bytes);
    if (pointer) {
        TRACE_ADD(pointer, count * bytes);
        return pointer;
    }
    ps5p_heap_count_fallback();
    return __real_calloc(count, bytes);
}

void __wrap_free(void *pointer) {
    if (!pointer) return;
    if (ps5_heap_owns(pointer)) {
        TRACE_DEL(pointer);
        ps5_heap_free(pointer);
    } else
        __real_free(pointer);
}

size_t __wrap_malloc_usable_size(const void *pointer) {
    if (!pointer) return 0;
    if (ps5_heap_owns(pointer)) return ps5_heap_usable_size(pointer);
    return __real_malloc_usable_size ? __real_malloc_usable_size(pointer) : 0;
}

void *__wrap_realloc(void *pointer, size_t bytes) {
    if (!pointer) {
        void *const fresh = ps5_heap_malloc(bytes);   // __wrap_malloc's body, so the record names this caller
        if (fresh) {
            TRACE_ADD(fresh, bytes);
            return fresh;
        }
        ps5p_heap_count_fallback();
        return __real_malloc(bytes);
    }
    /* FreeBSD's realloc gives a zero-size request a minimum-size object. */
    if (bytes == 0) bytes = 1;
    if (ps5_heap_owns(pointer)) {
        TRACE_DEL(pointer);
        void *const moved = ps5_heap_realloc(pointer, bytes);
        if (moved) {
            TRACE_ADD(moved, bytes);
            return moved;
        }
        /* Direct memory is exhausted: move the block to libc. */
        void *const copy = __real_malloc(bytes);
        if (!copy) return NULL;
        ps5p_heap_count_fallback();
        const size_t kept = ps5_heap_usable_size(pointer);
        memcpy(copy, pointer, kept < bytes ? kept : bytes);
        ps5_heap_free(pointer);
        return copy;
    }
    void *const moved = __real_realloc(pointer, bytes);
    if (moved || !__real_malloc_usable_size) return moved;
    /* libc's heap is exhausted: move its block to the title heap. */
    void *const copy = ps5_heap_malloc(bytes);
    if (!copy) return NULL;
    TRACE_ADD(copy, bytes);
    const size_t kept = __real_malloc_usable_size(pointer);
    memcpy(copy, pointer, kept < bytes ? kept : bytes);
    __real_free(pointer);
    return copy;
}

void *__wrap_reallocf(void *pointer, size_t bytes) {
    void *const moved = __wrap_realloc(pointer, bytes);
    if (!moved) __wrap_free(pointer);
    return moved;
}

void *__wrap_reallocarray(void *pointer, size_t count, size_t bytes) {
    if (bytes != 0 && count > SIZE_MAX / bytes) {
        errno = ENOMEM;
        return NULL;
    }
    return __wrap_realloc(pointer, count * bytes);
}

int __wrap_posix_memalign(void **out, size_t alignment, size_t bytes) {
    if (alignment < sizeof(void *) || (alignment & (alignment - 1)) != 0) return EINVAL;
    void *const pointer = ps5_heap_memalign(alignment, bytes);
    if (pointer) {
        TRACE_ADD(pointer, bytes);
        *out = pointer;
        return 0;
    }
    ps5p_heap_count_fallback();
    return __real_posix_memalign(out, alignment, bytes);
}

void *__wrap_aligned_alloc(size_t alignment, size_t bytes) {
    void *pointer = NULL;
    const int result = __wrap_posix_memalign(&pointer, alignment < sizeof(void *) ? sizeof(void *) : alignment, bytes);
    if (result != 0) {
        errno = result;
        return NULL;
    }
    return pointer;
}

void *__wrap_memalign(size_t alignment, size_t bytes) { return __wrap_aligned_alloc(alignment, bytes); }

/* libc's getdelim grows the caller's buffer with libc's realloc, which must never see a title-heap block: this one
 * grows it with the wrap. */
ssize_t __wrap_getdelim(char **line, size_t *capacity, int delimiter, FILE *stream) {
    if (!line || !capacity || !stream) {
        errno = EINVAL;
        return -1;
    }
    if (!*line) *capacity = 0;
    size_t length = 0;
    for (;;) {
        const int c = fgetc(stream);
        if (c == EOF) {
            if (length == 0 || ferror(stream)) return -1;
            break;
        }
        if (length + 2 > *capacity) {
            size_t grown = *capacity < 64 ? 128 : *capacity * 2;
            if (grown > (size_t)SSIZE_MAX) {
                errno = EOVERFLOW;
                return -1;
            }
            char *const moved = __wrap_realloc(*line, grown);
            if (!moved) return -1;
            *line = moved;
            *capacity = grown;
        }
        (*line)[length++] = (char)c;
        if (c == (unsigned char)delimiter) break;
    }
    (*line)[length] = '\0';
    return (ssize_t)length;
}

ssize_t __wrap_getline(char **line, size_t *capacity, FILE *stream) { return __wrap_getdelim(line, capacity, '\n', stream); }
