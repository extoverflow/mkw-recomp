// PS5 build only: the title's entry. A title gets no command line and has no console, so the mode and its options come
// from ARGS (one per line or space-separated, as on a command line; NAME=value sets an environment variable; none: run
// the game), and standard output and standard error go to LOG (a file the web file manager reads). The shell's splash
// is dismissed, and the title ends by asking the shell to close it: exit() or returning from _start makes the console report a crash
// (PS5_VulkanTemplate ps5/src/platform.c:48-56 and 82-89, MIT, Copyright (C) 2026 Mihawk: the same calls). The runtime's own
// main() is mkw_main (ps5_compat.h renames it).
#undef main
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
int sceKernelAvailableFlexibleMemorySize(size_t *size);
int sceKernelAvailableDirectMemorySize(int64_t start, int64_t end, size_t align, int64_t *phys, size_t *size);
int64_t sceKernelGetDirectMemorySize(void);

static void memory_report(const char *when) {
    struct ps5_heap_stats h = {0};
    ps5_heap_stats(&h);
    size_t flex = 0, dfree = 0;
    int64_t dphys = 0;
    int fr = sceKernelAvailableFlexibleMemorySize(&flex);
    (void)sceKernelAvailableDirectMemorySize(0, sceKernelGetDirectMemorySize(), 0, &dphys, &dfree);
    fprintf(stderr,
            "[host] ps5: memory %s: heap range %#llx+%#llx, mapped %.1f MiB (peak %.1f), in use %.1f MiB, %u segments, "
            "%u arenas, %llu libc fallbacks; flexible free %.1f MiB (%d); direct memory: largest free %.1f MiB at %.1f s\n",
            when, (unsigned long long)h.range_base, (unsigned long long)h.range_bytes, h.mapped_bytes / 1048576.0,
            h.peak_bytes / 1048576.0, h.in_use_bytes / 1048576.0, h.segments, h.arenas, h.libc_fallbacks,
            flex / 1048576.0, fr, dfree / 1048576.0, plat_process_uptime_ns() / 1e9);
}

// Descriptors 1 and 2, and so the streams stdout and stderr, go to LOG. A title may not dup2 (the console refuses it
// with EPERM: ref/PS5_PayloadSDK/platform/src/klog.c:86-87; title run 2 saw it too), so 1 and 2 are closed and the
// log opened twice: open takes the lowest free descriptor, and 0 stays open. Raw write(2, ...) calls (the signal
// handlers') then reach the log too, and the system libc's own stdout and stderr follow without being touched
// (reassigning the stdout pointer does not move printf: title run 2). Both are opened for appending, so their lines
// interleave. Returns whether the log took descriptors 1 and 2; if not, the streams are reopened onto it.
static int log_open(void) {
    close(1);
    close(2);
    int out = open(LOG, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0644);
    int err = open(LOG, O_WRONLY | O_APPEND);
    log_fds[0] = out, log_fds[1] = err;
    int ok = out == 1 && err == 2;
    if (!ok) {   // the streams at least (title run 3: freopen moved both)
        (void)!freopen(LOG, "a", stdout);
        (void)!freopen(LOG, "a", stderr);
    }
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    return ok;
}

// Asks the shell to close the title and waits for it (the template's catchReturnFromMain). The C runtime calls it after
// main returns; plat_exit (ps5_sys.c) calls it too.
void catchReturnFromMain(int status) {
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
static void dump_watch(void *arg) {
    (void)arg;
    for (int n = 1, tick = 0;; ) {
        plat_sleep_ms(2000);
        if (++tick % 30 == 0) memory_report("every 60 s");   // the memory budget while the game runs (Step 4)
        if (access(DUMP_TRIGGER, F_OK)) continue;
        unlink(DUMP_TRIGGER);
        int fd = open(DUMP_OUT, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) continue;
        char head[96];
        int k = snprintf(head, sizeof head, "== dump %d at %.3f s\n", n++, plat_process_uptime_ns() / 1e9);
        (void)!write(fd, head, (size_t)k);
        ps5_dump_threads(fd);
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

int main(int argc, char **argv) {
    (void)argc;
    ps5_dump_register("main");
    plat_thread_spawn(dump_watch, NULL, "dump watch", 0);
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
    int status = n >= 2 && !strcmp(args[1], "--ps5-pad-test")    ? pad_test(n >= 3 ? atoi(args[2]) : 20)
                 : n >= 2 && !strcmp(args[1], "--ps5-tone-test") ? tone_test(n >= 3 ? atoi(args[2]) : 3)
                 : n >= 3 && !strcmp(args[1], "--ps5-list-test") ? list_test(args[2])
                 : n >= 2 && !strcmp(args[1], "--ps5-signal-test") ? signal_test()
                                                                  : mkw_main(n, args);
    fprintf(stderr, "[host] ps5: mkw_main returned %d\n", status);
    return status;
}
