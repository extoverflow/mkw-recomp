// PS5 implementation of platform.h, process, libraries, machine load and the desktop: linux_sys.c (8cbbfe1) where the
// payload SDK's FreeBSD libc has the same call, FreeBSD's way where it differs (no /proc, sysctl instead), and the
// contracts' "unknown" answers for what a console title does not have (desktop, clipboard, child processes, load
// counters). The code map: kept outside this repo.
#include <dlfcn.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <pthread_np.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

#include "platform.h"
#include "ps5_int.h"

uintptr_t plat_exe_base(void) {
    Dl_info d;
    return dladdr((void *)plat_exe_base, &d) ? (uintptr_t)d.dli_fbase : 0;
}

int plat_module_of(uintptr_t addr, char *name, size_t cap, uintptr_t *base) {
    Dl_info d;
    if (!dladdr((void *)addr, &d) || !d.dli_fname) return 0;
    const char *n = strrchr(d.dli_fname, '/');
    *base = (uintptr_t)d.dli_fbase;
    return snprintf(name, cap, "%s", n ? n + 1 : d.dli_fname) < (int)cap;
}

size_t plat_getenv(const char *name, char *out, size_t cap) {
    const char *v = getenv(name);
    size_t n = v ? strlen(v) : 0;
    if (!n || n >= cap) return 0;
    memcpy(out, v, n + 1);
    return n;
}

// /proc/self/exe on Linux; FreeBSD's sysctl kern.proc.pathname, which the PS5 does not answer (ENOENT, fs_diag
// 12/11/2026). In a title (its executable is /app0/eboot.bin) the path names a program in PS5_DATA_DIR instead: every
// caller uses the folder only, as the program's own (mkw.ini, exefs/, romfs/, profile/, spirv/, pipeline-cache.bin,
// texpre/, lang/, shared-fonts/: cfg.c, main.c portable_paths, gfx_shader.c, vk_pipeline.c, ...), and /app0 is
// read-only. A payload has neither: 0.
#define PS5_TITLE_EXE "/app0/eboot.bin"

// A URL scheme handler (GameBanana 1-Click; linux_sys.c:594 writes an xdg .desktop file): the PS5 has no desktop to
// register one with, so none is made (0, as linux_sys.c returns when it cannot).
int plat_url_scheme(const char *scheme, const char *label, const char *exe, const char *arg) {
    (void)scheme, (void)label, (void)exe, (void)arg;
    return 0;
}

int plat_exe_path(char *out, size_t cap) {
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};
    size_t n = cap;
    if (!sysctl(mib, 4, out, &n, NULL, 0) && n > 1 && n <= cap) {
        out[n - 1] = 0;
        return 1;
    }
    struct stat st;
    return !stat(PS5_TITLE_EXE, &st) && snprintf(out, cap, "%s/mkw", PS5_DATA_DIR) < (int)cap;
}

// Cached per thread, as on Linux (every recursive mutex asks at each lock).
uint32_t plat_thread_id(void) {
    static _Thread_local uint32_t id;
    if (!id) id = (uint32_t)pthread_getthreadid_np();
    return id;
}

// No zoneinfo link to read on the console: "the system gives none".
int plat_time_zone(char *out, size_t cap) {
    (void)out, (void)cap;
    return 0;
}

// The first set variable of names ("ru_RU.UTF-8") as a BCP-47 tag ("ru-RU") into tag; "" for C and POSIX.

static void locale_tag(const char *const *names, int n, char *tag, size_t cap) {
    const char *v = NULL;
    for (int i = 0; i < n && !(v && *v); i++) v = getenv(names[i]);
    size_t k = 0;
    if (v && strcmp(v, "C") && strncmp(v, "C.", 2) && strcmp(v, "POSIX"))
        for (; v[k] && v[k] != '.' && v[k] != '@' && k < cap - 1; k++) tag[k] = v[k] == '_' ? '-' : v[k];
    tag[k] = 0;
}

const char *plat_ui_language(void) {
    static char tag[32];
    static const char *const names[] = {"LC_ALL", "LC_MESSAGES", "LANG"};
    if (!tag[0]) locale_tag(names, 3, tag, sizeof tag);
    return tag;
}

const char *plat_user_locale(void) {
    static char tag[32];
    static const char *const names[] = {"LC_ALL", "LC_TIME", "LANG"};
    if (!tag[0]) locale_tag(names, 3, tag, sizeof tag);
    return tag;
}

// No system font files a title can name: the menu falls back to its own.
int plat_font_path(const char *lang, char *out, size_t cap) {
    (void)lang, (void)out, (void)cap;
    return 0;
}

// The kernel's name and release, and the SDK version the system reports (kern.sdk_version) when it answers.
int plat_os_version(char *out, size_t cap) {
    struct utsname u;
    uint32_t sdk = 0;
    size_t n = sizeof sdk;
    int have = !sysctlbyname("kern.sdk_version", &sdk, &n, NULL, 0) && n == sizeof sdk;
    if (uname(&u)) return snprintf(out, cap, "PlayStation 5") < (int)cap;
    if (have) return snprintf(out, cap, "PlayStation 5 (%s %s, SDK %08x)", u.sysname, u.release, sdk) < (int)cap;
    return snprintf(out, cap, "PlayStation 5 (%s %s)", u.sysname, u.release) < (int)cap;
}

// No /proc/self/fd to name stderr's file by.
int plat_stderr_path(char *out, size_t cap) {
    (void)out, (void)cap;
    return 0;
}

// ---- process

// ps5.c's stack report, when ps5.c is linked in (the payload tests of ps5_sys.c alone do not have it).
__attribute__((weak)) int ps5_stack_report(char *out, size_t cap, int each);

// A title asks the shell to close it (ps5_main.c); _exit would be reported as a crash. Weak: the payload tests have no
// ps5_main.c and end with _exit as on Linux.
__attribute__((weak)) void catchReturnFromMain(int status);

void plat_exit(int code) {
    char rep[256];
    if (ps5_stack_report && ps5_stack_report(rep, sizeof rep, 0) > 0) fputs(rep, stderr);
    fflush(stdout);
    fflush(stderr);
    if (catchReturnFromMain) catchReturnFromMain(code);
    _exit(code);
}

int plat_setenv(const char *name, const char *value) {
    return (value && *value ? setenv(name, value, 1) : unsetenv(name)) == 0;   // empty removes, as _putenv_s on Windows
}

uint32_t plat_process_id(void) { return (uint32_t)getpid(); }

int plat_process_alive(uint32_t pid) { return kill((pid_t)pid, 0) == 0 || errno == EPERM; }

uint64_t plat_process_cpu_ns(void) {
    struct timespec t;
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t)) return 0;
    return (uint64_t)t.tv_sec * 1000000000u + (uint64_t)t.tv_nsec;
}

// Linux reads the start time from /proc/self/stat; here it is taken when the program is loaded.
static uint64_t started_ns;
__attribute__((constructor)) static void note_start(void) { started_ns = plat_now_ns(); }

uint64_t plat_process_uptime_ns(void) { return started_ns ? plat_now_ns() - started_ns : 0; }

uint64_t plat_unix_time_ns(void) {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (uint64_t)t.tv_sec * 1000000000u + (uint64_t)t.tv_nsec;
}

void plat_local_time(struct tm *out) {
    time_t t = time(NULL);
    localtime_r(&t, out);
}

// The range of the Windows CRT's localtime_s and gmtime_s (1970 to 3000), which glibc exceeds: the game's time
// service reports times outside it as invalid on both hosts.
#define PLAT_TIME_MAX 32535215999ll

int plat_localtime(int64_t unix_s, struct tm *out) {
    time_t t = (time_t)unix_s;
    return unix_s >= 0 && unix_s <= PLAT_TIME_MAX && localtime_r(&t, out) != NULL;
}

int plat_gmtime(int64_t unix_s, struct tm *out) {
    time_t t = (time_t)unix_s;
    return unix_s >= 0 && unix_s <= PLAT_TIME_MAX && gmtime_r(&t, out) != NULL;
}

// The payload SDK's libc has no time zone data (no TZ, no /etc/localtime): its local time is UTC and localtime_r
// leaves tm_gmtoff and tm_zone untouched (fs_diag, 12/11/2026). So the offset is 0, matching plat_local_time. The
// console's own time zone setting (system service parameters) is a later step.
long plat_tz_west(void) { return 0; }

// A console title starts no child processes: -1, "could not start" (the probes that check fatals in a child report it).
int plat_run_self(const char *env_name, const char *env_value, char *out, size_t cap, uint32_t timeout_ms) {
    (void)env_name, (void)env_value, (void)timeout_ms;
    if (cap) *out = 0;
    return -1;
}

// getrandom on Linux; FreeBSD's arc4random_buf (the kernel's generator) cannot fail.
int plat_random(void *out, size_t n) {
    arc4random_buf(out, n);
    return 1;
}

// ---- local IPC (Discord's Rich Presence, linux_sys.c:306-353 on a Unix socket): no Discord runs on the console, so
// nobody listens: plat_ipc_open answers NULL, as on a desktop without Discord, and core/presence never sends.
PlatIpc *plat_ipc_open(const char *name) {
    (void)name;
    return NULL;
}

int plat_ipc_send(PlatIpc *c, const void *data, size_t n) {
    (void)c, (void)data, (void)n;
    return 0;
}

long plat_ipc_recv(PlatIpc *c, void *buf, size_t cap) {
    (void)c, (void)buf, (void)cap;
    return -1;
}

void plat_ipc_close(PlatIpc *c) { (void)c; }

// ---- desktop: none on the console
int plat_clipboard_set(const char *utf8) {
    (void)utf8;
    return 0;
}

int plat_open_folder(const char *path) {
    (void)path;
    return 0;
}

int plat_desktop_shortcut(const char *name, const char *exe, const char *dir, char *out, size_t cap) {
    (void)name, (void)exe, (void)dir;
    if (out && cap) *out = 0;
    return 0;
}


void *plat_lib_open(const char *name) { return dlopen(name, RTLD_NOW | RTLD_LOCAL); }
void *plat_lib_sym(void *lib, const char *name) { return lib ? dlsym(lib, name) : NULL; }

// ---- machine load: no /proc or sysfs counters on the console. Every figure is "unknown" (-1, or 0 where the
// contract says so) except the memory total (plat_mem_info).
struct PlatLoad {
    int unused;
};
static PlatLoad no_load;   // when calloc fails: the contract says never NULL

PlatLoad *plat_load_open(const uint8_t *gpu_luid) {
    (void)gpu_luid;
    PlatLoad *l = calloc(1, sizeof *l);
    return l ? l : &no_load;
}

void plat_load_close(PlatLoad *l) {
    if (l != &no_load) free(l);
}

void plat_cpu_load(PlatLoad *l, float *total_pct, float *proc_pct, float *temp_c) {
    (void)l;
    *total_pct = *proc_pct = *temp_c = -1;
}

void plat_gpu_load(PlatLoad *l, float *load_pct, float *vram_used_mb) {
    (void)l;
    *load_pct = *vram_used_mb = -1;
}

uint64_t plat_gpu_vram_mib(uint32_t vendor, uint32_t device, const uint8_t *luid) {
    (void)vendor, (void)device, (void)luid;
    return 0;
}

// hw.physmem and its kin do not answer on the PS5 (ENOENT, fs_diag 12/11/2026). The total is the direct memory a title
// may use (12288 MiB in Gate 0; 0 from a payload: unknown then).
int64_t sceKernelGetDirectMemorySize(void);

void plat_mem_info(float *used_mb, float *total_mb, float *proc_mb) {
    int64_t dmem = sceKernelGetDirectMemorySize();
    *used_mb = *total_mb = *proc_mb = -1;
    if (dmem > 0) *total_mb = (float)(dmem >> 20);
}

void plat_process_memory(uint64_t *resident, uint64_t *private_bytes) { *resident = *private_bytes = 0; }

// linux_sys.c:556 (upstream 3042c59) is malloc_trim(0) after a large release of small blocks. The PS5's heap is the SDK
// fork's platform heap, whose ps5_heap_trim gives back each arena's free top and empty segments (ps5_main.c mem_watch
// calls it every 10 s too); weak: an older SDK has none.
__attribute__((weak)) void ps5_heap_trim(void);
void plat_heap_trim(void) {
    if (ps5_heap_trim) ps5_heap_trim();
}

int64_t plat_threads_cpu_ns_named(const char *name) {
    (void)name;
    return -1;
}

// No FIFOs for a title (and no mkfifo in the modules it loads): core/base/ctl.c's MKW_CTL then logs that it is off.
// Weak: a payload's libc.a has the system call, which wins.
__attribute__((weak)) int mkfifo(const char *path, mode_t mode) {
    (void)path, (void)mode;
    errno = ENOTSUP;
    return -1;
}

// The console's libzstd.a (built with tracing) calls these weak hooks; nothing defines them, and the title converter
// refuses an import nobody exports. 0: no trace (zstd's lib/common/zstd_trace.h, ZSTD_TraceCtx is unsigned long long).
unsigned long long ZSTD_trace_compress_begin(const void *cctx) {
    (void)cctx;
    return 0;
}
void ZSTD_trace_compress_end(unsigned long long ctx, const void *trace) { (void)ctx, (void)trace; }
unsigned long long ZSTD_trace_decompress_begin(const void *dctx) {
    (void)dctx;
    return 0;
}
void ZSTD_trace_decompress_end(unsigned long long ctx, const void *trace) { (void)ctx, (void)trace; }

// setvbuf for the runtime (ps5_compat.h renames its calls): _IONBF on stdout or stderr becomes line buffering.
#undef setvbuf
int setvbuf(FILE *stream, char *buf, int mode, size_t size);   // stdio.h declared it under the macro's name
int ps5_setvbuf(FILE *stream, char *buf, int mode, size_t size) {
    if (mode == _IONBF && (stream == stdout || stream == stderr)) return setvbuf(stream, NULL, _IOLBF, 4096);
    return setvbuf(stream, buf, mode, size);
}

// strdup and strndup inside the console's libc allocate from libc's own small private heap, which the title's link
// does not redirect (PS5_Vulkan tools/radv-link.sh wraps malloc and its family into the platform heap in direct
// memory, not the libc functions that allocate). The RomFS index's 206,202 names ran it out ("RomFS: loose index
// memory", 13/11/2026, with the platform heap at 64 MiB and no fallbacks): defined here, they take malloc's heap, for
// the runtime and for the linked-in RADV alike; free takes them back the same way. Named ps5_* and bound to the libc
// names at the title's link (the port's link script: --defsym and a local version script), as the link recipe
// binds the platform layer's (PS5_Vulkan tools/radv-link.sh:96-116): a definition under libc's name would be exported,
// and the title converter publishes no exports.
char *ps5_strdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *d = malloc(n);
    return d ? memcpy(d, s, n) : NULL;
}

char *ps5_strndup(const char *s, size_t max) {
    size_t n = strnlen(s, max);
    char *d = malloc(n + 1);
    if (!d) return NULL;
    memcpy(d, s, n);
    d[n] = 0;
    return d;
}
