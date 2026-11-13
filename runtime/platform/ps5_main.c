// PS5 build only: the title's entry. A title gets no command line and has no console, so the mode and its options come
// from ARGS (one per line or space-separated, as on a command line; NAME=value sets an environment variable; none: run
// the game), and standard output and standard error go to LOG (a file the web file manager reads). The shell's splash
// is dismissed, and the title ends by asking the shell to close it: exit() or returning from _start makes the console report a crash
// (PS5_VulkanTemplate ps5/src/platform.c:47-88, MIT, Copyright (C) 2026 Mihawk: the same calls). The runtime's own
// main() is mkw_main (ps5_compat.h renames it).
#undef main
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "platform.h"

#define PS5_DIR "/data/mkw-recomp"
#define ARGS PS5_DIR "/args.txt"
#define LOG PS5_DIR "/mkw.log"
#define MAX_ARGS 64

int mkw_main(int argc, char **argv);
int sceSystemServiceHideSplashScreen(void);
int sceSystemServiceLoadExec(const char *path, const char *const *argv);
int sceKernelUsleep(uint32_t microseconds);
static int log_fds[2];

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
    fprintf(stderr, "[host] exit status %d: asking the shell to close the title\n", status);
    fflush(NULL);
    int r = sceSystemServiceLoadExec("exit", NULL);
    fprintf(stderr, "[host] ps5: sceSystemServiceLoadExec(\"exit\") = %#x; waiting for the shell\n", (unsigned)r);
    for (;;) sceKernelUsleep(100000);
}

int main(int argc, char **argv) {
    (void)argc;
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
    int status = mkw_main(n, args);
    fprintf(stderr, "[host] ps5: mkw_main returned %d\n", status);
    return status;
}
