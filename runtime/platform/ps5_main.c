// PS5 build only: the title's entry. A title gets no command line and has no console, so the mode and its options come
// from ARGS (one per line or space-separated, as on a command line; NAME=value sets an environment variable; none: run
// the game), and standard output and standard error go to LOG (a file the web file manager reads). The shell's splash
// is dismissed, and the title ends by asking the shell to close it: exit() or returning from _start makes the console report a crash
// (PS5_VulkanTemplate ps5/src/platform.c:48-56 and 82-89, MIT, Copyright (C) 2026 Mihawk: the same calls). The runtime's own
// main() is mkw_main (ps5_compat.h renames it).
#undef main
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/stat.h>

#include "platform.h"
#include "ps5_int.h"

#define PS5_DIR PS5_DATA_DIR
#define ARGS PS5_DIR "/args.txt"
#define LOG PS5_DIR "/mkw.log"
#define MAX_ARGS 64

int mkw_main(int argc, char **argv);
void cfg_init(void);
void input_poll(uint64_t *buttons, int32_t *lx, int32_t *ly, int32_t *rx, int32_t *ry);
uint32_t input_player_mask(void);
int sceSystemServiceHideSplashScreen(void);
int sceSystemServiceLoadExec(const char *path, const char *const *argv);
int sceKernelUsleep(uint32_t microseconds);
static int log_fds[2];
int ps5_log_fd = 2;   // the log's descriptor for raw writes (a signal handler's lines): 1 and 2 are not the log here

// The platform heap every malloc goes to (PS5_Vulkan's link recipe wraps malloc into it: ps5platform/heap.h of the
// SDK fork's platform layer, ref/PS5_PayloadSDK/platform/include/ps5platform/heap.h:54-79) and the flexible memory
// left: logged at the start and at the exit, to see what a run used.
struct ps5_heap_stats {
    uintptr_t range_base;
    size_t range_bytes, mapped_bytes, peak_bytes;
    unsigned segments;
    unsigned long long libc_fallbacks;
    unsigned arenas;
    size_t in_use_bytes;
};
void ps5_heap_stats(struct ps5_heap_stats *stats);
// Gives back each arena's free top and its empty segments (the SDK fork's platform heap @ 3345cde, heap.h:72-77: "a
// title calls this now and then (every 10 s, say) so that what a burst took goes back to the pool it shares with its
// GPU and its guest"). Weak: the SDK's older platform layer (b83202b) has none.
__attribute__((weak)) void ps5_heap_trim(void);
int sceKernelAvailableFlexibleMemorySize(size_t *size);
int sceKernelAvailableDirectMemorySize(int64_t start, int64_t end, size_t align, int64_t *phys, size_t *size);
int64_t sceKernelGetDirectMemorySize(void);
// The allocated direct-memory region at or after an offset (flags 1: the next one), as EglantinePS5 walks the pool
// (headless/performance.cpp:531, :738 @ 64dbcde).
typedef struct { int64_t start, end; int32_t type; } DmemRegion;
int sceKernelDirectMemoryQuery(int64_t offset, int flags, DmemRegion *out, size_t size);

// All the pool's free memory and how many allocated regions there are: against the largest free block it tells holes
// from exhaustion. -1 when the regions cannot be read.
static double log_size(void) {   // the log's own writes (stdio and raw lines go to it, not through the file layer)
    struct stat st;
    return fstat(ps5_log_fd, &st) ? 0 : (double)st.st_size;
}

static int64_t dmem_free_total(unsigned *regions) {
    int64_t total = sceKernelGetDirectMemorySize(), taken = 0, offset = 0;
    unsigned n = 0;
    DmemRegion r;
    while (offset < total && n < 1000000 && !sceKernelDirectMemoryQuery(offset, 1, &r, sizeof r) && r.end > offset)
        taken += r.end - r.start, offset = r.end, n++;
    *regions = n;
    return n ? total - taken : -1;
}

static void memory_report(const char *when) {
    struct ps5_heap_stats h = {0};
    ps5_heap_stats(&h);
    size_t flex = 0, dfree = 0;
    int64_t dphys = 0;
    int fr = sceKernelAvailableFlexibleMemorySize(&flex);
    (void)sceKernelAvailableDirectMemorySize(0, sceKernelGetDirectMemorySize(), 0, &dphys, &dfree);
    unsigned regions = 0;
    int64_t ftotal = dmem_free_total(&regions);
    // The title's writes to storage: a title writes at full speed for ~1.3 GiB, then ~2 MiB/s (ref/PS5_PayloadSDK
    // platform/docs/PROBE.md:195-230), and every writer then waits (Step 5: hangs after 10-20 min).
    struct rusage ru = {0};
    (void)getrusage(RUSAGE_SELF, &ru);
    fprintf(stderr,
            "[host] ps5: memory %s: heap range %#llx+%#llx, mapped %.1f MiB (peak %.1f), in use %.1f MiB, %u segments, "
            "%u arenas, %llu libc fallbacks; flexible free %.1f MiB (%d); direct memory: largest free %.1f MiB at %.1f s, "
            "free %.1f MiB in all, %u regions allocated; clock drift %lld us; storage blocks written %ld, read %ld; file layer wrote %.1f MiB, log %.1f MiB\n",
            when, (unsigned long long)h.range_base, (unsigned long long)h.range_bytes, h.mapped_bytes / 1048576.0,
            h.peak_bytes / 1048576.0, h.in_use_bytes / 1048576.0, h.segments, h.arenas, h.libc_fallbacks,
            flex / 1048576.0, fr, dfree / 1048576.0, plat_process_uptime_ns() / 1e9, ftotal / 1048576.0, regions,
            (long long)(ps5_clock_drift_ns() / 1000), ru.ru_oublock, ru.ru_inblock,
            __atomic_load_n(&ps5_fs_bytes_written, __ATOMIC_RELAXED) / 1048576.0, log_size() / 1048576.0);
    char st[900];
    if (ps5_mem_stats(st, sizeof st) > 0) fprintf(stderr, "[host] ps5: memory %s: %s\n", when, st);
}

// Descriptors 1 and 2, and so the streams stdout and stderr, go to LOG. A title may not dup2 (the console refuses it
// with EPERM: ref/PS5_PayloadSDK/platform/src/klog.c:86-87; title run 2 saw it too), so 1 and 2 are closed and the
// log opened twice: open takes the lowest free descriptor, and 0 stays open. Raw write(2, ...) calls (the signal
// handlers') then reach the log too, and the system libc's own stdout and stderr follow without being touched
// (reassigning the stdout pointer does not move printf: title run 2). Both are opened for appending, so their lines
// interleave. Returns whether the log took descriptors 1 and 2; if not, the streams are reopened onto it.
static int log_open(void) {
    // The previous run's log is kept as mkw-prev.log: a title the console ended (its "Game or App Error", no FATAL of
    // ours) is often started again before anyone reads its log, and the new run truncated it (play test p3).
    (void)rename(LOG, PS5_DIR "/mkw-prev.log");
    close(1);
    close(2);
    int out = open(LOG, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0644);
    int err = open(LOG, O_WRONLY | O_APPEND);
    log_fds[0] = out, log_fds[1] = err;
    ps5_log_fd = err;
    int ok = out == 1 && err == 2;
    if (!ok) {   // the streams at least (title run 3: freopen moved both)
        (void)!freopen(LOG, "a", stdout);
        (void)!freopen(LOG, "a", stderr);
    }
    setvbuf(stdout, NULL, _IONBF, 0);   // line-buffered on the PS5 (ps5_compat.h: ps5_setvbuf)
    setvbuf(stderr, NULL, _IONBF, 0);
    return ok;
}

// ---- the runtime's log lines written by a thread of their own. A write to mkw.log can block: each read of the file
// through the web file manager blocked the title's appends ~0.7-0.95 s (runs 185-193, 19/11/2026), and every thread
// that logged then stopped there, the replay thread too (the menus at 2-25 fps for minutes). rt_log_raw (main.c) puts
// its lines in this queue instead and returns; "log writer" appends them. The queue drains before the title ends
// (catchReturnFromMain, plat_exit) and, without locks, in the crash handler (ps5_mem.c). A full queue drops lines and
// says how many. Lines written straight to stderr (this file's reports, RADV's) stay synchronous.
#define LOGQ_SIZE (4u << 20)
static char logq[LOGQ_SIZE];
static volatile uint64_t logq_in, logq_out, logq_dropped;   // bytes queued, bytes written, bytes dropped (all time)
static PlatMutex logq_lock = PLAT_MUTEX_INIT;
static PlatCond logq_cond;
static volatile int logq_on;
static int logq_min;   // MKW_PS5_LOG=min: only the lines the frame-rate runs need (a baseline without logging's cost)

// MKW_PS5_LOG=min keeps these and drops the rest (owner, 19/11/2026: a baseline with as little logging as can still be
// measured): the minute's frame times, the game's phases (when a race starts), the run's settings, fatal errors,
// the end, and the log writer's own stall lines.
static int logq_keep(const char *p, size_t n) {
    static const char *const keep[] = {"[host] frametime: 60 s", "[host] seq: enter", "[host] ps5: MKW_", "FATAL",
                                       "[host] FATAL", "[host] exit status", "[host] ps5: log ", "[host] ps5: fatal"};
    for (size_t i = 0; i < sizeof keep / sizeof *keep; i++) {
        size_t k = strlen(keep[i]);
        if (n >= k && !memcmp(p, keep[i], k)) return 1;
    }
    return 0;
}

int ps5_log_queue(const char *p, size_t n) {
    if (!__atomic_load_n(&logq_on, __ATOMIC_ACQUIRE)) return 0;
    if (logq_min && !logq_keep(p, n)) return 1;
    plat_mutex_lock(&logq_lock);
    if (logq_in - logq_out + n > LOGQ_SIZE) {
        logq_dropped += n;
    } else {
        size_t at = (size_t)(logq_in % LOGQ_SIZE), first = n < LOGQ_SIZE - at ? n : LOGQ_SIZE - at;
        memcpy(logq + at, p, first);
        memcpy(logq, p + first, n - first);
        logq_in += n;
        plat_cond_signal(&logq_cond);
    }
    plat_mutex_unlock(&logq_lock);
    return 1;
}

static void write_all(const char *p, size_t n) {
    while (n) {
        ssize_t w = write(ps5_log_fd, p, n);
        if (w <= 0) return;
        p += w, n -= (size_t)w;
    }
}

static void logq_main(void *arg) {
    (void)arg;
    uint64_t told = 0;
    plat_mutex_lock(&logq_lock);
    for (;;) {
        while (logq_in == logq_out) plat_cond_wait(&logq_cond, &logq_lock);
        uint64_t from = logq_out;
        size_t at = (size_t)(from % LOGQ_SIZE), n = (size_t)(logq_in - from);
        if (n > LOGQ_SIZE - at) n = LOGQ_SIZE - at;   // up to the end of the ring; the rest next round
        uint64_t dropped = logq_dropped;
        plat_mutex_unlock(&logq_lock);
        uint64_t t0 = plat_now_ns();
        write_all(logq + at, n);   // the bytes stay put: writers only fill what logq_out has passed
        uint64_t dt = plat_now_ns() - t0;
        static uint64_t n_slow, slow_ns, worst_ns;
        if (dt >= 100000000ull) {   // a write of 100 ms or more: the storage's stall, now off the game's threads
            n_slow++, slow_ns += dt;
            if (dt > worst_ns) worst_ns = dt;
            if (n_slow <= 8 || !(n_slow & (n_slow - 1))) {
                char line[160];
                int k = snprintf(line, sizeof line, "[host] ps5: log write of %zu bytes took %.0f ms (%llu writes of 100 ms "
                                 "or more, %.1f s in all, worst %.0f ms)\n", n, dt / 1e6, (unsigned long long)n_slow,
                                 slow_ns / 1e9, worst_ns / 1e6);
                if (k > 0) write_all(line, (size_t)k);
            }
        }
        if (dropped != told) {
            char line[128];
            int k = snprintf(line, sizeof line, "[host] ps5: log queue full: %llu bytes of log lines dropped so far\n",
                             (unsigned long long)dropped);
            if (k > 0) write_all(line, (size_t)k);
            told = dropped;
        }
        plat_mutex_lock(&logq_lock);
        logq_out = from + n;
    }
}

static void logq_start(void) {
    const char *e = getenv("MKW_PS5_LOG_SYNC");   // 1: the old synchronous writes (comparisons)
    if (e && *e == '1') return;
    const char *m = getenv("MKW_PS5_LOG");
    logq_min = m && !strcmp(m, "min");
    plat_cond_init(&logq_cond);
    if (plat_thread_spawn(logq_main, NULL, "log writer", 0)) __atomic_store_n(&logq_on, 1, __ATOMIC_RELEASE);
}

// Every fprintf to stderr in the title (ours and RADV's: "radv/ps5 submissions" every 10 s from the thread that
// submits) goes through the queue too, so a blocked log write stops no thread that renders (link-title.sh
// --wrap=fprintf). Other streams, and stderr before the writer starts or after the drain, as before.
int __wrap_fprintf(FILE *f, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int r;
    if (f == stderr && __atomic_load_n(&logq_on, __ATOMIC_ACQUIRE)) {
        char buf[2048];
        r = vsnprintf(buf, sizeof buf, fmt, ap);
        if (r > 0 && !ps5_log_queue(buf, (size_t)r < sizeof buf ? (size_t)r : sizeof buf - 1)) fputs(buf, f);
    } else {
        r = vfprintf(f, fmt, ap);
    }
    va_end(ap);
    return r;
}

// What is queued, written before the title ends: waits up to 3 s for the writer, then writes the rest itself.
void ps5_log_drain(void) {
    if (!__atomic_load_n(&logq_on, __ATOMIC_ACQUIRE)) return;
    for (int i = 0; i < 300 && __atomic_load_n(&logq_out, __ATOMIC_ACQUIRE) != __atomic_load_n(&logq_in, __ATOMIC_ACQUIRE); i++)
        sceKernelUsleep(10000);
    __atomic_store_n(&logq_on, 0, __ATOMIC_RELEASE);   // later lines straight to stderr
    plat_mutex_lock(&logq_lock);
    for (uint64_t at = logq_out; at < logq_in;) {
        size_t i = (size_t)(at % LOGQ_SIZE), n = (size_t)(logq_in - at);
        if (n > LOGQ_SIZE - i) n = LOGQ_SIZE - i;
        write_all(logq + i, n);
        at += n;
    }
    logq_out = logq_in;
    plat_mutex_unlock(&logq_lock);
}

// The crash handler's: no lock (the crashed thread may hold it); lines the writer is writing may appear twice.
void ps5_log_drain_raw(void) {
    if (!__atomic_load_n(&logq_on, __ATOMIC_ACQUIRE)) return;
    __atomic_store_n(&logq_on, 0, __ATOMIC_RELEASE);
    uint64_t in = logq_in;
    for (uint64_t at = logq_out; at < in;) {
        size_t i = (size_t)(at % LOGQ_SIZE), n = (size_t)(in - at);
        if (n > LOGQ_SIZE - i) n = LOGQ_SIZE - i;
        write_all(logq + i, n);
        at += n;
    }
}

// Asks the shell to close the title and waits for it (the template's catchReturnFromMain). The C runtime calls it after
// main returns; plat_exit (ps5_sys.c) calls it too.
void catchReturnFromMain(int status) {
    ps5_log_drain();
    memory_report("at exit");
    fprintf(stderr, "[host] exit status %d: asking the shell to close the title\n", status);
    fflush(NULL);
    int r = sceSystemServiceLoadExec("exit", NULL);
    fprintf(stderr, "[host] ps5: sceSystemServiceLoadExec(\"exit\") = %#x; waiting for the shell\n", (unsigned)r);
    for (;;) sceKernelUsleep(100000);
}

// --ps5-pad-test [SECONDS]: Step 3's pad check on the console, through input.c and platform/ps5_sdl.c as the game
// reads it: every change of player 1's buttons and sticks for SECONDS (default 20), one log line each.
static int pad_test(int seconds) {
    cfg_init();
    uint64_t b0 = ~0ull, b;
    int32_t s0[4] = {0}, s[4];
    uint64_t end = plat_now_ns() + (uint64_t)seconds * 1000000000ull;
    int changes = 0;
    printf("ps5-pad-test: %d s, players mask %#x\n", seconds, input_player_mask());
    while (plat_now_ns() < end) {
        input_poll(&b, &s[0], &s[1], &s[2], &s[3]);
        int moved = 0;
        for (int i = 0; i < 4; i++) moved |= (s[i] > s0[i] ? s[i] - s0[i] : s0[i] - s[i]) > 4096;
        if (b != b0 || moved) {
            printf("ps5-pad-test: %.2f s buttons %#llx sticks %d %d %d %d\n",
                   (double)(plat_now_ns() + (uint64_t)seconds * 1000000000ull - end) / 1e9, (unsigned long long)b, s[0],
                   s[1], s[2], s[3]);
            b0 = b, memcpy(s0, s, sizeof s0);
            changes++;
        }
        plat_sleep_ms(10);
    }
    printf("ps5-pad-test: done, %d changes, players mask %#x\n", changes, input_player_mask());
    return 0;
}

// --ps5-tone-test [SECONDS]: Step 3's sound check: a 440 Hz tone at a quarter of full scale for SECONDS (default 3)
// through platform/ps5_sdl.c's audio stream, as audio_mixer.c opens it.
#include <math.h>
#include <SDL3/SDL.h>
static void tone_pull(void *user, SDL_AudioStream *stream, int additional, int total) {
    (void)total;
    double *phase = user;
    float buf[2 * 256];
    int frames = additional / (int)(2 * sizeof(float));
    if (frames > 256) frames = 256;
    for (int i = 0; i < frames; i++, *phase += 2 * M_PI * 440.0 / 48000.0) buf[2 * i] = buf[2 * i + 1] = 0.25f * (float)sin(*phase);
    SDL_PutAudioStreamData(stream, buf, frames * (int)(2 * sizeof(float)));
}

static int tone_test(int seconds) {
    static double phase;
    SDL_AudioSpec spec = {SDL_AUDIO_F32, 2, 48000};
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) return printf("ps5-tone-test: init failed (%s)\n", SDL_GetError()), 1;
    SDL_AudioStream *s = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, tone_pull, &phase);
    if (!s || !SDL_ResumeAudioStreamDevice(s)) return printf("ps5-tone-test: no stream (%s)\n", SDL_GetError()), 1;
    uint64_t t0 = plat_now_ns();
    plat_sleep_ms((uint32_t)seconds * 1000u);
    double played = phase / (2 * M_PI * 440.0 / 48000.0);
    SDL_DestroyAudioStream(s);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    double secs = (double)(plat_now_ns() - t0) / 1e9;
    printf("ps5-tone-test: %.0f frames in %.2f s = %.0f frames/s (48000 expected)\n", played, secs, played / secs);
    return 0;
}

// --ps5-list-test DIR: what a directory listing gives in a title (dirfd, fstatat and lstat of the first entries).
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
static int list_test(const char *dir) {
    DIR *d = opendir(dir);
    printf("ps5-list-test: opendir(%s) = %p errno %d\n", dir, (void *)d, d ? 0 : errno);
    if (!d) return 1;
    int fd = dirfd(d), shown = 0;
    printf("ps5-list-test: dirfd = %d\n", fd);
    for (struct dirent *e; (e = readdir(d)) && shown < 4;) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        struct stat a, b;
        errno = 0;
        int ra = fstatat(fd, e->d_name, &a, AT_SYMLINK_NOFOLLOW), ea = errno;
        char path[4096];
        snprintf(path, sizeof path, "%s/%s", dir, e->d_name);
        errno = 0;
        int rb = lstat(path, &b), eb = errno;
        struct stat c, f;
        errno = 0;
        int rc = stat(path, &c), ec = errno;
        int ofd = open(path, O_RDONLY), eo = errno, rf = ofd >= 0 ? fstat(ofd, &f) : -1;
        if (ofd >= 0) close(ofd);
        printf("ps5-list-test: %s: d_type %d; fstatat %d errno %d; lstat %d errno %d; stat %d errno %d mode %o size %lld; "
               "open %d errno %d fstat %d mode %o size %lld\n",
               e->d_name, e->d_type, ra, ea, rb, eb, rc, ec, rc ? 0 : c.st_mode, rc ? 0LL : (long long)c.st_size, ofd, ofd < 0 ? eo : 0,
               rf, rf ? 0 : f.st_mode, rf ? 0LL : (long long)f.st_size);
        (void)a, (void)b;
        shown++;
    }
    closedir(d);
    return 0;
}

// A hang's thread dump on request: whenever DUMP_TRIGGER appears (tools/ps5-put.sh of any file there), every thread's
// instruction pointer and stack code addresses are appended to DUMP_OUT (ps5.c ps5_dump_threads), and the trigger is
// removed. Not through the log: a stuck thread may hold its stream's lock.
#define DUMP_TRIGGER PS5_DIR "/dump-threads"
#define DUMP_OUT PS5_DIR "/threads.txt"
// Step 4 diagnosis: who uses the title's 16,384 CPU page tables (one per 2 MiB stretch with any mapping)? For the first
// 180 s, every 100 ms, a line whenever the free count fell by 64 or more since the last line, with guest backing so far;
// in the log among the game's own lines, so the drops can be placed.
int sceKernelGetPageTableStats(int *cpu_total, int *cpu_free, int *gpu_total, int *gpu_free);
static void pt_watch(void *arg) {
    (void)arg;
    int last = 1 << 30;
    for (int i = 0; i < 1800; i++) {
        int ct = 0, cf = 0, gt = 0, gf = 0;
        if (sceKernelGetPageTableStats(&ct, &cf, &gt, &gf)) return;
        if (cf <= last - 64 || (cf == 0 && last)) {
            char b[256];
            ps5_mem_brief(b, sizeof b);
            fprintf(stderr, "[host] ps5: page tables at %.1f s: cpu free %d, gpu free %d; %s\n",
                    plat_process_uptime_ns() / 1e9, cf, gf, b);
            last = cf;
        }
        plat_sleep_ms(100);
    }
}

// The memory line every 30 s, on a thread of its own: it takes the log stream's and the heap's locks, and a thread
// stopped holding one of them would otherwise stop the thread dump too (play test p4).
extern int ps5_alloc_trace_on;                          // ps5_alloc_trace.c
int ps5_alloc_trace_report(FILE *f, int top);

static void mem_watch(void *arg) {
    (void)arg;
    if (!ps5_heap_trim) fprintf(stderr, "[host] ps5: the platform heap has no ps5_heap_trim (older SDK): not trimmed\n");
    for (int tick = 1;; tick++) {
        plat_sleep_ms(10000);
        if (tick == 1) {   // args.txt reaches the environment after this thread starts (run 66: never on)
            const char *e = getenv("MKW_PS5_ALLOC_TRACE");
            if (e && !strcmp(e, "1")) {
                ps5_alloc_trace_on = 1;
                fprintf(stderr, "[host] ps5: allocation trace on (MKW_PS5_ALLOC_TRACE): live sampled allocations by call site every 5 min\n");
            }
        }
        if (ps5_heap_trim) ps5_heap_trim();
        if (tick % 3 == 0) memory_report("every 30 s");
        if (tick % 30 == 15 && ps5_alloc_trace_on) ps5_alloc_trace_report(stderr, 25);   // every 5 min (MKW_PS5_ALLOC_TRACE)
        if (tick % 12 == 6) {   // every 2 min: how much of the guest's backed memory reads zero (todo.md, 2-hour goal)
            char z[320];
            ps5_mem_zero_census(z, sizeof z);
            fprintf(stderr, "[host] ps5: %s\n", z);
        }
    }
}

static void dump_watch(void *arg) {
    (void)arg;
    for (int n = 1, tick = 0;; ) {
        plat_sleep_ms(2000);
        (void)tick;   // the memory report has its own thread (mem_watch): it takes the log's and the heap's locks
        if (access(DUMP_TRIGGER, F_OK)) continue;
        // The trigger's text, if any: "<count> <interval_ms> <thread name>" samples that thread alone (a profile).
        char req[128] = "", name[64] = "";
        int count = 0, every = 0, tf = open(DUMP_TRIGGER, O_RDONLY);
        if (tf >= 0) {
            ssize_t r = read(tf, req, sizeof req - 1);
            req[r > 0 ? r : 0] = 0;
            close(tf);
        }
        unlink(DUMP_TRIGGER);
        if (sscanf(req, "%d %d %63[^\n]", &count, &every, name) != 3 || count <= 0 || every <= 0) count = 0;
        int fd = open(DUMP_OUT, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) continue;
        char head[160];
        int k = snprintf(head, sizeof head, "== dump %d at %.3f s%s%s\n", n++, plat_process_uptime_ns() / 1e9,
                         count ? ": sampling " : "", count ? name : "");
        (void)!write(fd, head, (size_t)k);
        if (count) ps5_dump_sample(fd, name, count, every);
        else ps5_dump_threads(fd);
        close(fd);
    }
}

// --ps5-signal-test: which signals a title's threads get (sigaction's answer, then a pthread_kill to a sleeping thread),
// one per run step, so a signal that kills the title shows as the last line.
#include <signal.h>
#include <pthread.h>
static volatile int sig_seen;
static void sig_test_handler(int sig) { sig_seen = sig; }
static void sig_test_sleeper(void *arg) {
    (void)arg;
    for (;;) plat_sleep_ms(50);
}
static int signal_test(void) {
    static const int sigs[] = {SIGUSR1, SIGINFO, SIGWINCH, SIGPROF, SIGXCPU, SIGVTALRM, SIGUSR2};
    PlatThread *t = plat_thread_new(sig_test_sleeper, NULL, "sleeper");
    pthread_t pt = *(pthread_t *)t;   // ps5.c's PlatThread starts with its pthread_t
    plat_sleep_ms(100);
    for (unsigned i = 0; i < sizeof sigs / sizeof *sigs; i++) {
        struct sigaction sa = {.sa_handler = sig_test_handler}, old;
        sigemptyset(&sa.sa_mask);
        int r = sigaction(sigs[i], &sa, &old);
        printf("ps5-signal-test: signal %d: sigaction %d (errno %d), sending...\n", sigs[i], r, r ? errno : 0);
        sig_seen = 0;
        int k = pthread_kill(pt, sigs[i]);
        uint64_t t0 = plat_now_ns();
        while (!sig_seen && plat_now_ns() - t0 < 500000000ull) plat_sleep_ms(1);
        printf("ps5-signal-test: signal %d: pthread_kill %d, handler %s\n", sigs[i], k, sig_seen == sigs[i] ? "ran" : "did not run");
        sigaction(sigs[i], &old, NULL);
    }
    printf("ps5-signal-test: done\n");
    return 0;
}

// --ps5-map-test: how many separate direct-memory mappings a title gets (Step 4: mapping failed with ENOMEM after ~32k
// 16 KiB backings): (1) 16 KiB pages each with its own allocation in a reserved 2 GiB range; (2) one 1 GiB allocation
// mapped 16 KiB at a time at matching offsets (contiguous neighbours: does the kernel merge them?).
#include <sys/mman.h>
int sceKernelReserveVirtualRange(void **addr, size_t len, int flags, size_t align);
int sceKernelAllocateDirectMemory(int64_t start, int64_t end, size_t len, size_t align, int type, int64_t *phys);
int sceKernelMapDirectMemory(void **addr, size_t len, int prot, int flags, int64_t phys, size_t align);
// CPU and GPU page tables of the process, in use and free (libkernel; EglantinePS5 src/host_memory.cpp:509-516 reads it
// the same way to keep page tables back).
int sceKernelGetPageTableStats(int *cpu_total, int *cpu_free, int *gpu_total, int *gpu_free);
static void pt_stats(const char *when) {
    int ct = -1, cf = -1, gt = -1, gf = -1, r = sceKernelGetPageTableStats(&ct, &cf, &gt, &gf);
    printf("ps5-map-test: page tables %s: cpu %d free %d, gpu %d free %d (%#x)\n", when, ct, cf, gt, gf, (unsigned)r);
}
// Maps up to `most` whole 2 MiB blocks from `first` (2 MiB-aligned in memory and in address) and writes each; returns how
// many, with the last allocate and map results.
static int map_blocks(uint64_t first, int most, int *ba, int *bm) {
    int nb = 0;
    *ba = *bm = 0;
    for (; nb < most; nb++) {
        int64_t phys = 0;
        void *at = (void *)(uintptr_t)(first + ((uint64_t)nb << 21));
        if ((*ba = sceKernelAllocateDirectMemory(0, sceKernelGetDirectMemorySize(), 2u << 20, 2u << 20, 12, &phys))) break;
        if ((*bm = sceKernelMapDirectMemory(&at, 2u << 20, PROT_READ | PROT_WRITE, MAP_FIXED, phys, 2u << 20))) break;
        ((volatile char *)at)[0] = 1, ((volatile char *)at)[(2u << 20) - 1] = 1;
    }
    return nb;
}
static int free_tables(void) {
    int t = -1, f = -1, gt, gf;
    (void)sceKernelGetPageTableStats(&t, &f, &gt, &gf);
    return f;
}
static int map_test(void) {
    const size_t kp = 0x4000, span = 1ull << 30;
    void *base = NULL;
    int r = sceKernelReserveVirtualRange(&base, span, 0, 2u << 20), n = 0, ra = 0, rm = 0;
    printf("ps5-map-test: reserve 1 GiB %#x at %p\n", (unsigned)r, base);
    for (; !r && n < (int)(span / kp); n++) {
        int64_t phys = 0;
        void *at = (char *)base + (size_t)n * kp;
        if ((ra = sceKernelAllocateDirectMemory(0, sceKernelGetDirectMemorySize(), kp, kp, 12, &phys))) break;
        if ((rm = sceKernelMapDirectMemory(&at, kp, PROT_READ | PROT_WRITE, MAP_FIXED, phys, kp))) break;
        if (!(n % 65536)) printf("ps5-map-test: (1) %d mappings so far\n", n);
    }
    printf("ps5-map-test: (1) separate 16 KiB mappings: %d made, then allocate %#x map %#x\n", n, (unsigned)ra, (unsigned)rm);
    munmap(base, span);   // (the direct memory of (1) stays allocated: this process ends after the test)
    void *b2 = NULL;
    int64_t big = 0;
    r = sceKernelReserveVirtualRange(&b2, 1ull << 30, 0, 2u << 20);
    ra = sceKernelAllocateDirectMemory(0, sceKernelGetDirectMemorySize(), 1ull << 30, 2u << 20, 12, &big);
    int m = 0;
    rm = 0;
    for (; !r && !ra && m < (int)((1ull << 30) / kp); m++) {
        void *at = (char *)b2 + (size_t)m * kp;
        if ((rm = sceKernelMapDirectMemory(&at, kp, PROT_READ | PROT_WRITE, MAP_FIXED, big + (int64_t)((size_t)m * kp), kp)))
            break;
    }
    printf("ps5-map-test: (2) adjacent 16 KiB maps of one 1 GiB allocation: %d made of %d (reserve %#x allocate %#x, last "
           "map %#x)\n", m, (int)((1ull << 30) / kp), (unsigned)r, (unsigned)ra, (unsigned)rm);
    // (3) scattered: one 16 KiB page per 2 MiB (each needs its own page-table page), as the game's guest memory is
    // touched, over two 64 GiB ranges reserved at the guest's fixed addresses (free in this test mode): how many before
    // mapping fails?
    static const uint64_t bases[2] = {0x1000000000ull, 0x3000000000ull};
    pt_stats("before (3)");
    int q = 0, rr[2];
    ra = rm = 0;
    for (int g = 0; g < 2; g++) {
        void *at = (void *)(uintptr_t)bases[g];
        rr[g] = sceKernelReserveVirtualRange(&at, 64ull << 30, 0x10 | 0x80, 2u << 20);   // MAP_FIXED | no replace
    }
    // (4a) while page tables are free: 64 whole 2 MiB blocks in a 1 GiB region of their own (the second range's top half,
    // which (3) never reaches). How many page tables do they take?
    if (!rr[0] && !rr[1]) {
        int ba, bm, f0 = free_tables(), nb = map_blocks(bases[1] + (32ull << 30), 64, &ba, &bm);
        printf("ps5-map-test: (4a) whole 2 MiB blocks: %d of 64 mapped and written (allocate %#x map %#x); free cpu page "
               "tables %d -> %d\n", nb, (unsigned)ba, (unsigned)bm, f0, free_tables());
    }
    for (int g = 0; g < 2 && !rr[0] && !rr[1] && !ra && !rm; g++)
        for (int i = 0; i < 32768; i++, q++) {
            int64_t phys = 0;
            void *at = (void *)(uintptr_t)(bases[g] + ((uint64_t)i << 21));
            if ((ra = sceKernelAllocateDirectMemory(0, sceKernelGetDirectMemorySize(), kp, kp, 12, &phys))) break;
            if ((rm = sceKernelMapDirectMemory(&at, kp, PROT_READ | PROT_WRITE, MAP_FIXED, phys, kp))) break;
            if (!(q % 8192)) printf("ps5-map-test: (3) %d scattered so far\n", q), pt_stats("during (3)");
        }
    printf("ps5-map-test: (3) scattered 16 KiB mappings, one per 2 MiB: %d made of 65536 (reserve %#x %#x, then allocate "
           "%#x map %#x)\n", q, (unsigned)rr[0], (unsigned)rr[1], (unsigned)ra, (unsigned)rm);
    pt_stats("after (3)");
    // (4b) with no page table free, whole 2 MiB blocks in the 1 GiB region where (3) stopped: its directory page exists,
    // so a large page there needs no new table. Does it map?
    {
        uint64_t first = bases[0] + ((uint64_t)(q + 1) << 21), end = (first + (1ull << 30) - 1) & ~((1ull << 30) - 1);
        int most = (int)((end - first) >> 21), ba, bm, f0 = free_tables();
        if (most > 64) most = 64;
        int nb = map_blocks(first, most, &ba, &bm);
        printf("ps5-map-test: (4b) whole 2 MiB blocks from %#llx: %d of %d mapped and written (allocate %#x map %#x); free "
               "cpu page tables %d -> %d\n", (unsigned long long)first, nb, most, (unsigned)ba, (unsigned)bm, f0, free_tables());
    }
    return 0;
}

// --ps5-block-test: ps5_mem.c in a title on real 2 MiB blocks (Step 4: re-backing 2 MiB-aligned pages failed with
// ENOMEM in the game): a block backed at once, page 0 and page 1 decommitted and backed again, then the counters.
static int block_test(void) {
    const uint64_t at = 0x3800000000ull, blk = 2ull << 20, kp = 0x4000;   // a free spot in the guest's window
    int ok = plat_mem_reserve(at, 4 * blk, 0) && plat_mem_commit(at, 4 * blk);
    printf("ps5-block-test: reserve+commit 8 MiB at %#llx: %d\n", (unsigned long long)at, ok);
    for (int b = 0; ok && b < 4; b++) {
        volatile uint8_t *p = (volatile uint8_t *)(uintptr_t)(at + (uint64_t)b * blk);
        p[kp] = 1;                                   // backs the whole block
        uint64_t page = b & 1 ? kp : 0;              // blocks 0, 2: page 0; blocks 1, 3: page 1
        plat_mem_decommit((uint64_t)(uintptr_t)p + page, kp);
        plat_mem_commit((uint64_t)(uintptr_t)p + page, kp);
        p[page] = 7;                                 // re-backs that page alone
        printf("ps5-block-test: block %d page %llu: read %d (want 7)\n", b, (unsigned long long)(page / kp), p[page]);
    }
    char st[900];
    ps5_mem_stats(st, sizeof st);
    printf("ps5-block-test: %s\n", st);
    return 0;
}

int main(int argc, char **argv) {
    (void)argc;
    ps5_dump_register("main");
    plat_thread_spawn(dump_watch, NULL, "dump watch", 0);
    logq_start();
    plat_thread_spawn(mem_watch, NULL, "mem watch", 0);
    // The SDK's stdio.h expands getc/putc/putchar/fileno/feof inline on FreeBSD's FILE layout unless __isthreaded is
    // set, and the system libc's FILE is not that layout (title run: putchar wrote through a bad pointer). The payload
    // SDK's start code sets it (ref/PS5_PayloadSDK/crt/crt.c:65-69); the title's (PS5_Vulkan tooling/native/app_crt.cpp)
    // does not, so main does, before anything prints.
    __isthreaded = 1;
    plat_file_mkdir_p(PS5_DIR);
    int log_ok = log_open();
    sceSystemServiceHideSplashScreen();
    static char text[4096];
    static char *args[MAX_ARGS + 1];
    int n = 0;
    args[n++] = argv && argv[0] ? argv[0] : "/app0/eboot.bin";
    FILE *f = fopen(ARGS, "r");
    size_t len = f ? fread(text, 1, sizeof text - 1, f) : 0;
    if (f) fclose(f);
    text[len] = 0;
    // NAME=value sets an environment variable (the MKW_* settings, the probes' child modes); the rest are arguments
    int envs = 0;
    for (char *t = strtok(text, " \t\r\n"); t && n < MAX_ARGS; t = strtok(NULL, " \t\r\n")) {
        size_t name = strspn(t, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_");
        char *eq = name && t[name] == '=' && !(t[0] >= '0' && t[0] <= '9') ? t + name : NULL;
        if (eq) {
            *eq = 0;
            setenv(t, eq + 1, 1);
            fprintf(stderr, "[host] ps5: %s=%s\n", t, eq + 1);
            envs++;
        } else {
            args[n++] = t;
        }
    }
    args[n] = NULL;
    printf("[host] ps5: stdout to the log\n");
    fprintf(stderr, "[host] ps5: log %s (opened as %d and %d); %d variable(s), %d argument(s) from %s:",
            log_ok ? "on descriptors 1 and 2" : "NOT on 1 and 2", log_fds[0], log_fds[1], envs, n - 1, ARGS);
    for (int i = 1; i < n; i++) fprintf(stderr, " %s", args[i]);
    fputc('\n', stderr);
    memory_report("at start");
    if (getenv("MKW_PS5_PT_WATCH")) plat_thread_spawn(pt_watch, NULL, "pt watch", 0);   // after args.txt's settings
    int status = n >= 2 && !strcmp(args[1], "--ps5-pad-test")    ? pad_test(n >= 3 ? atoi(args[2]) : 20)
                 : n >= 2 && !strcmp(args[1], "--ps5-tone-test") ? tone_test(n >= 3 ? atoi(args[2]) : 3)
                 : n >= 3 && !strcmp(args[1], "--ps5-list-test") ? list_test(args[2])
                 : n >= 2 && !strcmp(args[1], "--ps5-signal-test") ? signal_test()
                 : n >= 2 && !strcmp(args[1], "--ps5-map-test") ? map_test()
                 : n >= 2 && !strcmp(args[1], "--ps5-block-test") ? block_test()
                                                                  : mkw_main(n, args);
    fprintf(stderr, "[host] ps5: mkw_main returned %d\n", status);
    return status;
}
