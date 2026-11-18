// PS5 implementation of platform.h: threads, thread interruption, fibers, affinity and priorities. linux.c
// (8cbbfe1) on the payload SDK's FreeBSD libthr, with one change of substance: stacks. In a PS5 title every anonymous
// mapping, even an inaccessible one, counts against the ~442 MiB flexible-memory budget (Gate 0), and the runtime asks
// for 256 MiB per guest thread and fiber (HOST_STACK) and 16 MiB per runtime thread. So stacks are direct memory,
// committed, sized to what the threads use on Linux with a wide margin (measured 2026-11-12: guest stacks at most
// 84 KiB deep, runtime threads 212 KiB), below each an unmapped guard. Code map and divergences:
// kept outside this repo.
#include <errno.h>
#include <stdarg.h>
#include <pthread.h>
#include <pthread_np.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/cpuset.h>
#include <sys/mman.h>
#include <time.h>
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

// ---- stacks
#define PS5_PAGE 0x4000u                 // the kernel's page (Gate 0)
#define STACK_GUARD (64u << 10)          // reserved, never mapped: an overflow faults instead of writing a neighbour
#define STACK_GUEST_MIN (64u << 20)      // a request this large is HOST_STACK (256 MiB): a guest thread or fiber
#define STACK_GUEST_KIB 2048u            // MKW_PS5_STACK_GUEST_KIB overrides: 24x the deepest guest stack measured
#define STACK_KIB 1024u                  // MKW_PS5_STACK_KIB overrides: 5x the deepest runtime stack measured
#define STACK_FILL 0x21355350214b5453ull // "STK!PS5!": what an untouched word of a stack still holds (depth report)
#define STACK_MAX 4096                   // stacks tracked for the report and the free list
#define STACK_REUSE_NS 100000000u        // a detached thread's stack is reused this long after its thread ended

typedef struct Ps5Stack {
    uint8_t *base;      // the reservation: guard, then the stack
    size_t size;        // the stack's bytes, above the guard
    int64_t phys;       // the direct memory behind it; -1: anonymous memory (no direct memory here: a payload)
    int kind;           // 0 runtime thread, 1 guest thread or fiber
    int in_use;
    pthread_t owner;    // the detached thread that last ran on it, until it is gone
    uint64_t freed_ns;  // when it was put back; 0 while in use
} Ps5Stack;

static Ps5Stack stacks[STACK_MAX];
static int nstacks, stack_direct = -1;   // -1 untried, 1 direct memory works, 0 it does not (anonymous instead)
static PlatMutex stack_lock = PLAT_MUTEX_INIT;

static size_t env_kib(const char *name, size_t def) {
    const char *v = getenv(name);
    long k = v ? atol(v) : 0;
    return (k > 0 ? (size_t)k : def) << 10;
}

// The bytes to commit for a request of want: guest-sized requests get the guest size, the others the runtime size but
// never more than they asked for (a small helper thread's 64 KiB stays 64 KiB). Whole kernel pages.
static size_t stack_bytes(size_t want, int *kind) {
    *kind = want >= STACK_GUEST_MIN;
    size_t n = *kind ? env_kib("MKW_PS5_STACK_GUEST_KIB", STACK_GUEST_KIB) : env_kib("MKW_PS5_STACK_KIB", STACK_KIB);
    if (!*kind && want && want < n) n = want;
    if (n < 4 * PS5_PAGE) n = 4 * PS5_PAGE;
    return (n + PS5_PAGE - 1) & ~(size_t)(PS5_PAGE - 1);
}

// A new stack: an address range reserved (guard + stack, sceKernelReserveVirtualRange: not flexible memory) and direct
// memory mapped over its top part; where there is no direct memory to have (a payload: 0 MiB), anonymous memory with
// the guard made inaccessible. The words are filled with STACK_FILL for the depth report.
static int stack_map(Ps5Stack *s, size_t size) {
    size_t total = size + STACK_GUARD;
    void *base = NULL;
    s->phys = -1;
    if (stack_direct != 0 && !sceKernelReserveVirtualRange(&base, total, 0, PS5_PAGE)) {
        int64_t phys = 0;
        void *top = (uint8_t *)base + STACK_GUARD;
        if (!sceKernelAllocateDirectMemory(0, sceKernelGetDirectMemorySize(), size, PS5_PAGE, 12, &phys)) {
            if (!sceKernelMapDirectMemory(&top, size, PROT_READ | PROT_WRITE, MAP_FIXED, phys, PS5_PAGE) &&
                top == (uint8_t *)base + STACK_GUARD)
                s->phys = phys;
            else
                sceKernelReleaseDirectMemory(phys, size);
        }
        if (s->phys < 0) munmap(base, total), base = NULL;
        if (stack_direct < 0) stack_direct = s->phys >= 0;
    }
    if (!base) {
        base = mmap(NULL, total, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (base == MAP_FAILED) return 0;
        mprotect(base, STACK_GUARD, PROT_NONE);
    }
    s->base = base, s->size = size;
    uint64_t *w = (uint64_t *)(s->base + STACK_GUARD);
    for (size_t i = 0; i < size / 8; i++) w[i] = STACK_FILL;
    return 1;
}

// A stack for a thread or fiber that asked for want bytes: a free one of the same size, else a new one. NULL when the
// memory is gone.
static size_t stack_depth(const Ps5Stack *s);

static Ps5Stack *stack_get(size_t want) {
    int kind;
    size_t size = stack_bytes(want, &kind);
    uint64_t now = plat_now_ns();
    plat_mutex_lock(&stack_lock);
    Ps5Stack *s = NULL;
    for (int i = 0; i < nstacks && !s; i++) {
        Ps5Stack *c = &stacks[i];
        if (c->in_use || c->size != size || now - c->freed_ns < STACK_REUSE_NS) continue;
        if (c->owner && pthread_kill(c->owner, 0) != ESRCH) continue;   // its thread is still finishing on it
        s = c;
    }
    if (!s && nstacks < STACK_MAX) {
        s = &stacks[nstacks];
        if (stack_map(s, size)) nstacks++;
        else s = NULL;
    }
    if (s) {
        if (s->freed_ns) {   // used before: refilled where it was used, so the report measures each user afresh
            size_t used = stack_depth(s);
            uint64_t *w = (uint64_t *)(s->base + STACK_GUARD + s->size - used);
            for (size_t i = 0; i < used / 8; i++) w[i] = STACK_FILL;
        }
        s->in_use = 1, s->kind = kind, s->owner = 0, s->freed_ns = 0;
    }
    plat_mutex_unlock(&stack_lock);
    return s;
}

// Back to the free list. owner: the thread still running on it as it ends (detached), else 0.
static void stack_put(Ps5Stack *s, pthread_t owner) {
    plat_mutex_lock(&stack_lock);
    s->owner = owner, s->freed_ns = plat_now_ns(), s->in_use = 0;
    plat_mutex_unlock(&stack_lock);
}

static size_t stack_depth(const Ps5Stack *s) {
    const uint64_t *w = (const uint64_t *)(s->base + STACK_GUARD);
    size_t i = 0;
    while (i < s->size / 8 && w[i] == STACK_FILL) i++;
    return s->size - i * 8;
}

int ps5_stack_report(char *out, size_t cap, int each) {
    size_t deepest[2] = {0, 0}, size[2] = {0, 0};
    int n[2] = {0, 0}, k = 0, anon = 0;
    plat_mutex_lock(&stack_lock);
    for (int i = 0; i < nstacks; i++) {
        size_t d = stack_depth(&stacks[i]);
        int kind = stacks[i].kind;
        n[kind]++, size[kind] = stacks[i].size, anon += stacks[i].phys < 0;
        if (d > deepest[kind]) deepest[kind] = d;
        if (each && (size_t)k < cap)
            k += snprintf(out + k, cap - (size_t)k, "stack %d: %s, %zu of %zu KiB used, %s, at %p\n", i,
                          kind ? "guest" : "runtime", d >> 10, stacks[i].size >> 10,
                          stacks[i].phys >= 0 ? "direct memory" : "anonymous", (void *)(stacks[i].base + STACK_GUARD));
    }
    plat_mutex_unlock(&stack_lock);
    if ((size_t)k < cap)
        k += snprintf(out + k, cap - (size_t)k,
                      "stacks: %d guest, deepest %zu of %zu KiB; %d runtime, deepest %zu of %zu KiB; %d in direct "
                      "memory, %d anonymous\n", n[1], deepest[1] >> 10, size[1] >> 10, n[0], deepest[0] >> 10,
                      size[0] >> 10, n[0] + n[1] - anon, anon);
    return k;
}

// ---- threads

struct PlatThread {
    pthread_t t;
    PlatThreadFn fn;
    void *arg;
    Ps5Stack *stack;
    volatile int detached;   // set by plat_thread_detach: the thread puts its own stack back as it ends
    int prio;
    char name[32];
};

static __thread int my_prio;

// A signal stack for the calling thread, so the fault and crash handlers still run when its own stack is exhausted
// (linux.c's alt_stack). 64 KiB of anonymous memory each: small enough for the flexible budget.
void alt_stack(void) {
    stack_t cur;
    if (!sigaltstack(NULL, &cur) && !(cur.ss_flags & SS_DISABLE)) return;
    size_t size = 64 << 10;
    void *p = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (p == MAP_FAILED) return;
    stack_t ss = {.ss_sp = p, .ss_size = size};
    sigaltstack(&ss, NULL);
}

// ---- thread dump (PS5 only, for a hang: Linux has tools/run/stacks.py over ptrace, a title has no debugger). The
// runtime's threads register themselves; ps5_dump_threads stops each with SIGUSR2 and logs its instruction pointer and
// the stack words that point into the executable's code (return addresses, mostly), as offsets from its start, for
// llvm-symbolizer on the linked ELF. A scan, not an unwind: the code has no frame pointers.
#define DUMP_MAX 512
#define DUMP_WORDS 24
#define DUMP_SIG SIGUSR2
typedef struct {
    pthread_t t;
    uint64_t lo, hi;   // the thread's stack
    char name[32];
    int live;
} DumpThread;
static DumpThread dump_threads[DUMP_MAX];
static PlatMutex dump_lock = PLAT_MUTEX_INIT;
static struct {
    volatile int done;
    uint64_t lo, hi, rip, rsp, rdi, n, w[DUMP_WORDS];
} dump_res;
extern const char __ehdr_start[];   // the executable's start: .text begins there (lld)
// The code's end: the port's linker script defines it at the title's link. Weak: the payload tests have no such
// symbol, and their dump then takes no stack words. (The ELF header is not mapped in a title: reading its program
// headers at __ehdr_start faulted, 13/11/2026.)
extern const char mkw_text_end[] __attribute__((weak));
static uint64_t text_lo, text_hi;

static void text_range(void) {
    text_lo = (uint64_t)(uintptr_t)__ehdr_start;
    text_hi = mkw_text_end ? (uint64_t)(uintptr_t)mkw_text_end : text_lo;
}

// Records where the thread is, from its context only: no memory is read here (a page the reported stack bounds include
// can be reserved but unmapped, and a fault in this handler ends the title: the first two dumps, 13/11/2026).
static void dump_handler(int sig, siginfo_t *si, void *uc) {
    (void)sig, (void)si;
    ucontext_t *u = uc;
    dump_res.rip = (uint64_t)u->uc_mcontext.mc_rip, dump_res.rsp = (uint64_t)u->uc_mcontext.mc_rsp;
    dump_res.rdi = (uint64_t)u->uc_mcontext.mc_rdi;   // in a wait system call: the object waited on (_umtx_op's first)
    __atomic_store_n(&dump_res.done, 1, __ATOMIC_RELEASE);
}

// The code addresses among the words of [rsp, rsp + 64 KiB) inside the stack's bounds, read by the dumping thread
// through a pipe: write(2) from an unreadable page fails with EFAULT instead of faulting.
static void dump_scan(uint64_t rsp, uint64_t lo_stack, uint64_t hi_stack) {
    static int pfd[2] = {-1, -1};
    dump_res.n = 0;
    if (rsp < lo_stack || rsp >= hi_stack || (pfd[0] < 0 && pipe(pfd))) return;
    uint64_t end = hi_stack < rsp + (64u << 10) ? hi_stack : rsp + (64u << 10);
    static uint64_t buf[512];
    for (uint64_t a = rsp & ~7ull; a < end && dump_res.n < DUMP_WORDS;) {
        uint64_t chunk = ((a | 0xfff) + 1) - a;   // to the next 4 KiB boundary
        if (chunk > end - a) chunk = end - a;
        ssize_t w = write(pfd[1], (const void *)(uintptr_t)a, (size_t)chunk);
        if (w <= 0) break;
        ssize_t r = read(pfd[0], buf, (size_t)w);
        for (ssize_t i = 0; i + 8 <= r && dump_res.n < DUMP_WORDS; i += 8) {
            uint64_t v = buf[i / 8];
            if (v >= text_lo && v < text_hi) dump_res.w[dump_res.n++] = v;
        }
        a += (uint64_t)w;
    }
}

void ps5_dump_register(const char *name) {
    pthread_attr_t a;
    void *addr = NULL;
    size_t size = 0;
    pthread_attr_init(&a);
    if (!pthread_attr_get_np(pthread_self(), &a)) pthread_attr_getstack(&a, &addr, &size);
    pthread_attr_destroy(&a);
    plat_mutex_lock(&dump_lock);
    for (int i = 0; i < DUMP_MAX; i++)
        if (!dump_threads[i].live) {
            dump_threads[i] = (DumpThread){pthread_self(), (uint64_t)(uintptr_t)addr, (uint64_t)(uintptr_t)addr + size,
                                           "", 1};
            snprintf(dump_threads[i].name, sizeof dump_threads[i].name, "%s", name ? name : "?");
            break;
        }
    plat_mutex_unlock(&dump_lock);
}

static void dump_unregister(void) {
    plat_mutex_lock(&dump_lock);
    for (int i = 0; i < DUMP_MAX; i++)
        if (dump_threads[i].live && pthread_equal(dump_threads[i].t, pthread_self())) dump_threads[i].live = 0;
    plat_mutex_unlock(&dump_lock);
}

// Written with write(2) to fd, not stdio: a stuck thread may hold the log stream's lock (13/11/2026: the first dump
// through fprintf blocked, and with it every log line).
static void dump_line(int fd, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
static void dump_line(int fd, const char *fmt, ...) {
    char line[1024];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(line, sizeof line, fmt, ap);
    va_end(ap);
    if (n > 0) (void)!write(fd, line, (size_t)n < sizeof line ? (size_t)n : sizeof line - 1);
}

// The threads named name, count times, every interval_ms: a profile of where that thread spends its time (each line
// as ps5_dump_threads writes it). The registry is read under dump_lock each round, so threads may come and go.
static int dump_one(int fd, DumpThread *d, int i, uint64_t base);

void ps5_dump_sample(int fd, const char *name, int count, int interval_ms) {
    ps5_dump_threads(-1);   // installs the handler and the code range without writing anything
    uint64_t base = (uint64_t)(uintptr_t)__ehdr_start;
    for (int k = 0; k < count; k++) {
        plat_mutex_lock(&dump_lock);
        for (int i = 0; i < DUMP_MAX; i++)
            if (dump_threads[i].live && !strcmp(dump_threads[i].name, name)) dump_one(fd, &dump_threads[i], i, base);
        plat_mutex_unlock(&dump_lock);
        plat_sleep_ms((uint32_t)interval_ms);
    }
    dump_line(fd, "sample: %d rounds of '%s' every %d ms\n", count, name, interval_ms);
}

static int dump_one(int fd, DumpThread *d, int i, uint64_t base) {
    dump_res.done = 0, dump_res.lo = d->lo, dump_res.hi = d->hi;
    if (pthread_kill(d->t, DUMP_SIG)) return 0;
    uint64_t t0 = plat_now_ns();
    while (!__atomic_load_n(&dump_res.done, __ATOMIC_ACQUIRE) && plat_now_ns() - t0 < 300000000ull) sched_yield();
    if (!dump_res.done) {
        dump_line(fd, "thread %d '%s': no answer in 300 ms\n", i, d->name);
        return 0;
    }
    dump_scan(dump_res.rsp, d->lo, d->hi);
    char words[DUMP_WORDS * 20] = "";
    int k = 0;
    for (uint64_t w = 0; w < dump_res.n && k < (int)sizeof words; w++)
        k += snprintf(words + k, sizeof words - (size_t)k, " %#llx", (unsigned long long)(dump_res.w[w] - base));
    dump_line(fd, "thread %d '%s' rip %#llx rsp %#llx rdi %#llx; stack:%s\n", i, d->name,
              (unsigned long long)(dump_res.rip - base), (unsigned long long)dump_res.rsp,
              (unsigned long long)dump_res.rdi, words);
    return 1;
}

void ps5_dump_threads(int fd) {
    static int installed;
    if (!installed) {
        struct sigaction sa = {.sa_sigaction = dump_handler, .sa_flags = SA_SIGINFO | SA_RESTART | SA_ONSTACK};
        sigemptyset(&sa.sa_mask);
        int r = sigaction(DUMP_SIG, &sa, NULL);
        text_range();
        if (fd >= 0)
            dump_line(fd, "dump: handler %d, code %#llx-%#llx\n", r, (unsigned long long)text_lo,
                      (unsigned long long)text_hi);
        installed = 1;
    }
    uint64_t base = (uint64_t)(uintptr_t)__ehdr_start;
    plat_mutex_lock(&dump_lock);
    int shown = 0;
    for (int i = 0; i < DUMP_MAX && fd >= 0; i++) {
        DumpThread *d = &dump_threads[i];
        if (!d->live || pthread_equal(d->t, pthread_self())) continue;
        shown += dump_one(fd, d, i, base);
    }
    plat_mutex_unlock(&dump_lock);
    if (fd >= 0)
        dump_line(fd, "dump: %d threads (offsets from the executable's start, %#llx)\n", shown, (unsigned long long)base);
}

static void *thread_main(void *p) {
    PlatThread *t = p;
    alt_stack();
    if (t->name[0]) pthread_set_name_np(pthread_self(), t->name);
    ps5_dump_register(t->name[0] ? t->name : "thread");
    t->fn(t->arg);
    dump_unregister();
    // Detached: nobody joins, so the thread hands its stack back itself; stack_get reuses it only once this thread is
    // gone (pthread_kill reports ESRCH) and STACK_REUSE_NS has passed.
    if (__atomic_load_n(&t->detached, __ATOMIC_ACQUIRE)) stack_put(t->stack, pthread_self());
    return NULL;
}

PlatThread *plat_thread_new(PlatThreadFn fn, void *arg, const char *name) {
    return plat_thread_new_ex(fn, arg, name, 0);
}

PlatThread *plat_thread_new_ex(PlatThreadFn fn, void *arg, const char *name, size_t stack) {
    PlatThread *t = calloc(1, sizeof *t);
    if (!t) return NULL;
    t->fn = fn;
    t->arg = arg;
    if (name) snprintf(t->name, sizeof t->name, "%s", name);
    t->stack = stack_get(stack ? stack : PLAT_STACK_DEFAULT);
    if (!t->stack) {
        free(t);
        return NULL;
    }
    pthread_attr_t a;
    pthread_attr_init(&a);
    pthread_attr_setstack(&a, t->stack->base + STACK_GUARD, t->stack->size);
    int e = pthread_create(&t->t, &a, thread_main, t);
    pthread_attr_destroy(&a);
    if (e) {
        stack_put(t->stack, 0);
        free(t);
        return NULL;
    }
    return t;
}

int plat_thread_spawn(PlatThreadFn fn, void *arg, const char *name, size_t stack) {
    PlatThread *t = plat_thread_new_ex(fn, arg, name, stack);
    if (t) plat_thread_detach(t);
    return t != NULL;
}

void plat_thread_join(PlatThread *t) {
    pthread_join(t->t, NULL);
    stack_put(t->stack, 0);
    free(t);
}

int plat_thread_join_ns(PlatThread *t, uint64_t ns) {
    struct timespec at;
    clock_gettime(CLOCK_REALTIME, &at);   // pthread_timedjoin_np takes a CLOCK_REALTIME deadline, as on Linux
    uint64_t end = (uint64_t)at.tv_sec * 1000000000u + (uint64_t)at.tv_nsec + ns;
    at.tv_sec = (time_t)(end / 1000000000u), at.tv_nsec = (long)(end % 1000000000u);
    if (pthread_timedjoin_np(t->t, NULL, &at)) return 0;
    stack_put(t->stack, 0);
    free(t);
    return 1;
}

// The thread keeps using t (fn, arg, its stack), so the struct stays allocated, as on Linux.
void plat_thread_detach(PlatThread *t) {
    __atomic_store_n(&t->detached, 1, __ATOMIC_RELEASE);
    pthread_detach(t->t);
}

// ---- processors. No /sys topology on the console: the processors the process may use come from its cpuset, and the
// two logical processors of a core are taken to be 2k and 2k+1 (Zen 2's numbering). The kernel takes a set of at most
// 16 bytes: the SDK's cpuset_t is 32, which it refuses with ERANGE (thread_test, 13/11/2026), so every call passes
// PS5_CPUSET_BYTES.
#define PS5_CPUSET_BYTES 16
static int cpuset_mask(uint64_t *mask) {
    cpuset_t set;
    CPU_ZERO(&set);
    if (cpuset_getaffinity(CPU_LEVEL_WHICH, CPU_WHICH_PID, -1, PS5_CPUSET_BYTES, &set)) return 0;
    *mask = 0;
    for (int i = 0; i < 64; i++)
        if (CPU_ISSET(i, &set)) *mask |= 1ull << i;
    return 1;
}

int plat_cpu_cores(uint64_t *all, uint64_t *first) {
    *all = *first = 0;
    if (!cpuset_mask(all)) return 0;
    int cores = 0;
    for (int i = 0; i < 64; i += 2)
        if (*all >> i & 3) *first |= 1ull << (*all >> i & 1 ? i : i + 1), cores++;
    return cores;
}

// linux.c:130 matches a processor's core by /sys core_id; here by plat_cpu_cores' rule (2k and 2k+1 share a core).
uint64_t plat_cpu_core_of(int cpu) {
    if (cpu < 0 || cpu > 63) return 0;
    return 3ull << (cpu & ~1);
}

// linux.c:141 picks the processors with the highest cpufreq maximum (a hybrid CPU's performance cores). The PS5's eight
// Zen 2 cores are alike: every processor the process may use; 0 when the cpuset cannot be read.
uint64_t plat_cpu_fast(void) {
    uint64_t m = 0;
    return cpuset_mask(&m) ? m : 0;
}

// linux.c:184 (upstream 3042c59) puts a background thread on every processor the process had at start, so a hybrid
// CPU's efficiency cores take it while the frame's threads keep the fast ones; Windows does nothing. The PS5's cores are
// alike (plat_cpu_fast): nothing to move.
void plat_thread_background(void) {}

int plat_process_affinity(uint64_t mask) {
    cpuset_t set;
    CPU_ZERO(&set);
    for (int i = 0; i < 64; i++)
        if (mask >> i & 1) CPU_SET(i, &set);
    return cpuset_setaffinity(CPU_LEVEL_WHICH, CPU_WHICH_PID, -1, PS5_CPUSET_BYTES, &set) == 0;
}

uint64_t plat_thread_affinity(PlatThread *t, uint64_t mask) {
    pthread_t th = t ? t->t : pthread_self();
    cpuset_t old, set;
    CPU_ZERO(&old);
    if (pthread_getaffinity_np(th, PS5_CPUSET_BYTES, &old)) return 0;
    CPU_ZERO(&set);
    for (int i = 0; i < 64; i++)
        if (mask >> i & 1) CPU_SET(i, &set);
    if (pthread_setaffinity_np(th, PS5_CPUSET_BYTES, &set)) return 0;
    uint64_t prev = 0;
    for (int i = 0; i < 64; i++)
        if (CPU_ISSET(i, &old)) prev |= 1ull << i;
    return prev;
}

uint32_t plat_cpu_count(void) {
    uint64_t m = 0;
    if (cpuset_mask(&m) && m) return (uint32_t)__builtin_popcountll(m);
    long n = sysconf(_SC_NPROCESSORS_ONLN);
    return n > 0 ? (uint32_t)n : 1;
}

uint64_t plat_self_cpu_ns(void) {
    struct timespec t;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t);
    return (uint64_t)t.tv_sec * 1000000000ull + (uint64_t)t.tv_nsec;
}

// Another thread's CPU time: /proc on Linux; the SDK has no clock_getcpuclockid2. Unknown.
uint64_t plat_thread_cpu_ns(uint32_t tid) {
    (void)tid;
    return 0;
}

// ---- thread interruption: linux.c's signal handshake. SIGUSR1, the signal Gate 0 measured (10,000 stops, mean 12 us);
// the registers through FreeBSD's mcontext (the SDK fork's headers have the PS5's layout, Gate 0).
enum { INTR_IDLE, INTR_REQ, INTR_STOPPED, INTR_GO };
#define INTR_WAIT_NS 200000u
#define INTR_SIG SIGUSR1

struct PlatIntr {
    pthread_t pt;
    volatile int32_t state;
    uint64_t rip, rsp, to;
};

static __thread PlatIntr *intr_me;
static int intr_sig;

static void intr_handler(int sig, siginfo_t *si, void *uc) {
    (void)sig, (void)si;
    PlatIntr *t = intr_me;
    ucontext_t *u = uc;
    if (!t) return;
    t->rip = (uint64_t)u->uc_mcontext.mc_rip;
    t->rsp = (uint64_t)u->uc_mcontext.mc_rsp;
    int32_t req = INTR_REQ;
    if (!__atomic_compare_exchange_n(&t->state, &req, INTR_STOPPED, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) return;
    while (__atomic_load_n(&t->state, __ATOMIC_ACQUIRE) != INTR_GO) plat_pause();
    if (t->to) u->uc_mcontext.mc_rip = (__register_t)t->to;
    __atomic_store_n(&t->state, INTR_IDLE, __ATOMIC_RELEASE);
}

PlatIntr *plat_intr_self(void) {
    static PlatMutex install = PLAT_MUTEX_INIT;
    if (!__atomic_load_n(&intr_sig, __ATOMIC_ACQUIRE)) {
        plat_mutex_lock(&install);
        if (!intr_sig) {
            struct sigaction sa = {.sa_sigaction = intr_handler, .sa_flags = SA_SIGINFO | SA_RESTART | SA_ONSTACK};
            sigemptyset(&sa.sa_mask);
            if (!sigaction(INTR_SIG, &sa, NULL)) __atomic_store_n(&intr_sig, INTR_SIG, __ATOMIC_RELEASE);
        }
        plat_mutex_unlock(&install);
    }
    if (!intr_sig) return NULL;
    PlatIntr *t = calloc(1, sizeof *t);
    if (!t) return NULL;
    t->pt = pthread_self();
    intr_me = t;
    return t;
}

void plat_intr_free(PlatIntr *t) {
    if (!t) return;
    if (intr_me == t) intr_me = NULL;
    free(t);
}

int plat_intr_stop(PlatIntr *t, uint64_t *rip, uint64_t *rsp) {
    int32_t idle = INTR_IDLE;
    if (!__atomic_compare_exchange_n(&t->state, &idle, INTR_REQ, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) return 0;
    if (pthread_kill(t->pt, intr_sig)) {
        __atomic_store_n(&t->state, INTR_IDLE, __ATOMIC_RELEASE);
        return 0;
    }
    uint64_t end = plat_now_ns() + INTR_WAIT_NS;
    while (__atomic_load_n(&t->state, __ATOMIC_ACQUIRE) != INTR_STOPPED) {
        if (plat_now_ns() < end) {
            plat_pause();
            continue;
        }
        int32_t req = INTR_REQ;
        if (__atomic_compare_exchange_n(&t->state, &req, INTR_IDLE, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) return 0;
    }
    *rip = t->rip;
    *rsp = t->rsp;
    return 1;
}

int plat_intr_resume(PlatIntr *t, uint64_t to) {
    t->to = to;
    __atomic_store_n(&t->state, INTR_GO, __ATOMIC_RELEASE);
    return 1;
}

// ---- fibers: linux.c's own stack switch, x86-64 only (the PS5's CPU). The stacks come from stack_get like a thread's.
struct PlatFiber {
    void *sp;
    Ps5Stack *stack;   // NULL for a thread's own context
};

void plat_fiber_swap(void **save, void *load);
void plat_fiber_entry(void);
__asm__(".text\n"
        ".globl plat_fiber_swap\n"
        ".type plat_fiber_swap, @function\n"
        ".p2align 4\n"
        "plat_fiber_swap:\n"
        "    .cfi_startproc\n"
        "    pushq %rbp\n"
        "    pushq %rbx\n"
        "    pushq %r12\n"
        "    pushq %r13\n"
        "    pushq %r14\n"
        "    pushq %r15\n"
        "    subq $16, %rsp\n"
        "    stmxcsr 8(%rsp)\n"
        "    fnstcw (%rsp)\n"
        "    movq %rsp, (%rdi)\n"
        "    movq %rsi, %rsp\n"
        "    fldcw (%rsp)\n"
        "    ldmxcsr 8(%rsp)\n"
        "    addq $16, %rsp\n"
        "    popq %r15\n"
        "    popq %r14\n"
        "    popq %r13\n"
        "    popq %r12\n"
        "    popq %rbx\n"
        "    popq %rbp\n"
        "    ret\n"
        "    .cfi_endproc\n"
        ".size plat_fiber_swap, .-plat_fiber_swap\n"
        ".globl plat_fiber_entry\n"
        ".type plat_fiber_entry, @function\n"
        ".p2align 4\n"
        "plat_fiber_entry:\n"
        "    movq %r13, %rdi\n"
        "    andq $-16, %rsp\n"   // the ret here left it 8 off; SysV wants 16 before the call (NX2navi e9edd6a)
        "    callq *%r12\n"
        "    ud2\n"
        ".size plat_fiber_entry, .-plat_fiber_entry\n");

static __thread PlatFiber *fiber_cur;
static __thread PlatFiber fiber_own;

PlatFiber *plat_fiber_self(void) {
    fiber_own = (PlatFiber){0};
    fiber_cur = &fiber_own;
    return &fiber_own;
}

void plat_fiber_unself(void) { fiber_cur = NULL; }

PlatFiber *plat_fiber_new(size_t stack, void (*fn)(void *arg), void *arg) {
    PlatFiber *f = calloc(1, sizeof *f);
    if (!f) return NULL;
    f->stack = stack_get(stack);
    if (!f->stack) {
        free(f);
        return NULL;
    }
    uint64_t *top = (uint64_t *)(((uintptr_t)f->stack->base + STACK_GUARD + f->stack->size) & ~(uintptr_t)15);
    uint64_t *sp = top - 10;   // fpu state (2 words), r15, r14, r13, r12, rbx, rbp, return address, padding
    sp[0] = 0x037f;            // x87 control word: the default
    sp[1] = 0x1f80;            // MXCSR: the default
    sp[2] = sp[3] = 0;         // r15, r14
    sp[4] = (uint64_t)(uintptr_t)arg;   // r13
    sp[5] = (uint64_t)(uintptr_t)fn;    // r12
    sp[6] = sp[7] = 0;         // rbx, rbp
    sp[8] = (uint64_t)(uintptr_t)plat_fiber_entry;
    sp[9] = 0;
    f->sp = sp;
    return f;
}

void plat_fiber_switch(PlatFiber *to) {
    PlatFiber *from = fiber_cur;
    fiber_cur = to;   // on this thread; the switch back into from sets it on whichever thread that is
    plat_fiber_swap(&from->sp, to->sp);
}

void plat_fiber_free(PlatFiber *f) {
    if (!f) return;
    if (f->stack) stack_put(f->stack, 0);   // nobody runs it: free at once
    free(f);
}

// ---- priorities: remembered, not applied. Linux applies them only with the right to raise a priority again; the PS5's
// own thread priorities (scePthreadSetprio) are a later step.
void plat_thread_priority(int prio) { my_prio = prio; }

void plat_thread_set_priority(PlatThread *t, int prio) { t->prio = prio; }

int plat_thread_priority_get(void) { return my_prio; }

void plat_yield(void) { sched_yield(); }

// ---- GPU memory log (MKW_PS5_GPUMEM_LOG=IP:PORT; a diagnosis, off by default). The silent exits (CE-108255-1) are GPU page
// faults at host addresses past the guest's (run 129b: "GPU Protection fault ... client:TCP(8) access:Read, reason:
// Unmapped page access, addr(VA): 0x40d280c000", GPU_FAULT_PAGE_FAULT_ASYNC; PS_LIMIT is 0x4000000000): memory RADV
// gave back while queued GPU work still read it. RADV's PS5 winsys maps and frees its memory through
// radv_ps5_platform.c (PS5_Mesa 0f86253 src/amd/vulkan/winsys/ps5/radv_ps5_platform.c:292 radv_ps5_memory_alloc,
// :433 radv_ps5_memory_alloc_replayable, :730 radv_ps5_vrange_release, :746 radv_ps5_memory_free, :910
// radv_ps5_memory_unmap_at; struct radv_ps5_memory radv_ps5_platform.h:48-60 begins with the mapping, which is also
// the GPU address, and its size). The title is linked with --wrap for each (the port's link script); here each
// call is sent to the listener as it happens (gm_line): the wall clock (UTC, as the
// kernel log recorder stamps its lines), the kind (A alloc, F free, V vrange release, U unmap), address, size, and the
// first return addresses on the stack inside the title (offsets from __ehdr_start, for llvm-symbolizer); sparse binds below.
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
struct radv_ps5_memory;
typedef struct { uint8_t *cpu; uint64_t bytes; } GmHead;   // radv_ps5_platform.h:48-51
bool __real_radv_ps5_memory_alloc(uint64_t bytes, uint64_t alignment, bool window32, struct radv_ps5_memory *out);
bool __real_radv_ps5_memory_alloc_replayable(uint64_t bytes, uint64_t alignment, bool window32, uint64_t replay_va,
                                             struct radv_ps5_memory *out);
void __real_radv_ps5_memory_free(struct radv_ps5_memory *memory);
void __real_radv_ps5_vrange_release(struct radv_ps5_memory *range);
void __real_radv_ps5_memory_unmap_at(void *address, uint64_t bytes, bool reserve);
bool __wrap_radv_ps5_memory_alloc(uint64_t bytes, uint64_t alignment, bool window32, struct radv_ps5_memory *out);
bool __wrap_radv_ps5_memory_alloc_replayable(uint64_t bytes, uint64_t alignment, bool window32, uint64_t replay_va,
                                             struct radv_ps5_memory *out);
void __wrap_radv_ps5_memory_free(struct radv_ps5_memory *memory);
void __wrap_radv_ps5_vrange_release(struct radv_ps5_memory *range);
void __wrap_radv_ps5_memory_unmap_at(void *address, uint64_t bytes, bool reserve);

static int gm_fd = -2;   // -2 not decided, -1 off: a UDP socket to GM_TO
static struct sockaddr_in gm_to;
static uint64_t gm_n;
static _Thread_local const uint64_t *gm_why;   // ps5_gm_dying: the object vk_mem_upload.c's collect_locked destroys now
static _Thread_local uint64_t gm_gate[6];      // what it waited for (seq, part, submission) and the done values then

// MKW_PS5_GPUMEM_LOG=IP:PORT: one UDP datagram a line to a listener (a UDP listener on the LAN). Not a file:
// a write(2) to /data a call stalled the title (run 130: 0.1x speed, allocations ~950 ms each; cf. Step 4 run 12's
// unbuffered stdio on /data), and a datagram already sent survives the fault that ends the title. The frees of empty
// memory (address 0: the winsys's per-submission dedicated slot, radv_ps5_cs.c:395) are not sent.
static void gm_open(void) {
    if (gm_fd == -2) {
        const char *e = getenv("MKW_PS5_GPUMEM_LOG");
        int fd = -1;
        char ip[32] = "";
        int port = 0;
        if (e && sscanf(e, "%31[0-9.]:%d", ip, &port) == 2 && port > 0) {
            memset(&gm_to, 0, sizeof gm_to);
            gm_to.sin_family = AF_INET;
            gm_to.sin_port = htons((uint16_t)port);
            if (inet_pton(AF_INET, ip, &gm_to.sin_addr) == 1) fd = socket(AF_INET, SOCK_DGRAM, 0);
        }
        int was = -2;
        if (!__atomic_compare_exchange_n(&gm_fd, &was, fd, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE) && fd >= 0) close(fd);
        else if (fd >= 0) fprintf(stderr, "[host] ps5: RADV's memory allocations and frees go to %s by UDP (MKW_PS5_GPUMEM_LOG)\n", e);
    }
}

// vk_mem_upload.c (weak there): whether the log is on; the deferring thread's return addresses (offsets, as gm_line's);
// the object being destroyed by collect_locked, whose why[] and gate go on the lines of the frees it causes (NULL after).
int ps5_gm_on(void) {
    gm_open();
    return __atomic_load_n(&gm_fd, __ATOMIC_ACQUIRE) >= 0;
}
__attribute__((noinline)) int ps5_gm_stack(uint64_t *w, int n) {
    if (!text_hi) text_range();
    const uint64_t *sp = (const uint64_t *)__builtin_frame_address(0);
    int got = 0;
    for (int i = 0; i < 400 && got < n; i++)
        if (sp[i] >= text_lo && sp[i] < text_hi) w[got++] = sp[i] - text_lo;
    return got;
}
void ps5_gm_dying(const uint64_t *why, uint64_t seq, uint64_t tl, uint64_t fence, uint64_t done_seq, uint64_t tl_done,
                  uint64_t fdone) {
    gm_why = why;
    gm_gate[0] = seq, gm_gate[1] = tl, gm_gate[2] = fence, gm_gate[3] = done_seq, gm_gate[4] = tl_done, gm_gate[5] = fdone;
}

static __attribute__((noinline)) void gm_line(char kind, uint64_t va, uint64_t bytes) {
    if (!va) return;
    gm_open();
    int fd = __atomic_load_n(&gm_fd, __ATOMIC_ACQUIRE);
    if (fd < 0) return;
    if (!text_hi) text_range();
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm tm;
    gmtime_r(&ts.tv_sec, &tm);
    char b[640];
    int k = snprintf(b, sizeof b, "%02d:%02d:%02d.%03ld %c %#llx %#llx %llu", tm.tm_hour, tm.tm_min, tm.tm_sec,
                     ts.tv_nsec / 1000000, kind, (unsigned long long)va, (unsigned long long)bytes,
                     (unsigned long long)__atomic_add_fetch(&gm_n, 1, __ATOMIC_RELAXED));
    const uint64_t *sp = (const uint64_t *)__builtin_frame_address(0);
    for (int i = 0, got = 0; i < 160 && got < 6 && k < (int)sizeof b - 24; i++) {   // this thread's own stack
        uint64_t v = sp[i];
        if (v >= text_lo && v < text_hi) k += snprintf(b + k, sizeof b - (size_t)k, " %#llx", (unsigned long long)(v - text_lo)), got++;
    }
    // A free caused by a deferred destruction: "| D kind path tl_noted seq/tl/fence done done_seq/tl/fdone" and the
    // deferring stack (the port's log tool symbolises it too).
    if (gm_why) {
        k += snprintf(b + k, sizeof b - (size_t)k, " | D %llu %llu %llu %llu/%llu/%llu done %llu/%llu/%llu",
                      (unsigned long long)(gm_why[0] & 0xff), (unsigned long long)(gm_why[0] >> 8),
                      (unsigned long long)gm_why[1], (unsigned long long)gm_gate[0], (unsigned long long)gm_gate[1],
                      (unsigned long long)gm_gate[2], (unsigned long long)gm_gate[3], (unsigned long long)gm_gate[4],
                      (unsigned long long)gm_gate[5]);
        for (int i = 2; i < 10 && gm_why[i] && k < (int)sizeof b - 24; i++)
            k += snprintf(b + k, sizeof b - (size_t)k, " %#llx", (unsigned long long)gm_why[i]);
    }
    (void)!sendto(fd, b, (size_t)k, 0, (const struct sockaddr *)&gm_to, sizeof gm_to);
}

bool __wrap_radv_ps5_memory_alloc(uint64_t bytes, uint64_t alignment, bool window32, struct radv_ps5_memory *out) {
    bool ok = __real_radv_ps5_memory_alloc(bytes, alignment, window32, out);
    if (ok) gm_line('A', (uint64_t)(uintptr_t)((GmHead *)out)->cpu, ((GmHead *)out)->bytes);
    return ok;
}
bool __wrap_radv_ps5_memory_alloc_replayable(uint64_t bytes, uint64_t alignment, bool window32, uint64_t replay_va,
                                             struct radv_ps5_memory *out) {
    bool ok = __real_radv_ps5_memory_alloc_replayable(bytes, alignment, window32, replay_va, out);
    if (ok) gm_line('A', (uint64_t)(uintptr_t)((GmHead *)out)->cpu, ((GmHead *)out)->bytes);
    return ok;
}
void __wrap_radv_ps5_memory_free(struct radv_ps5_memory *memory) {
    gm_line('F', (uint64_t)(uintptr_t)((GmHead *)memory)->cpu, ((GmHead *)memory)->bytes);
    __real_radv_ps5_memory_free(memory);
}
void __wrap_radv_ps5_vrange_release(struct radv_ps5_memory *range) {
    gm_line('V', (uint64_t)(uintptr_t)((GmHead *)range)->cpu, ((GmHead *)range)->bytes);
    __real_radv_ps5_vrange_release(range);
}
void __wrap_radv_ps5_memory_unmap_at(void *address, uint64_t bytes, bool reserve) {
    gm_line('U', (uint64_t)(uintptr_t)address, bytes);
    __real_radv_ps5_memory_unmap_at(address, bytes, reserve);
}
// Sparse ranges: binding and unbinding pages of a reserved range (radv_ps5_platform.c:633 radv_ps5_vrange_unbind, :648
// radv_ps5_vrange_bind): an unbind maps nothing there again, as a free does. B bind, X unbind.
bool __real_radv_ps5_vrange_bind(void *at, uint64_t bytes, const struct radv_ps5_memory *memory, uint64_t offset);
bool __real_radv_ps5_vrange_unbind(void *at, uint64_t bytes);
bool __wrap_radv_ps5_vrange_bind(void *at, uint64_t bytes, const struct radv_ps5_memory *memory, uint64_t offset);
bool __wrap_radv_ps5_vrange_unbind(void *at, uint64_t bytes);
bool __wrap_radv_ps5_vrange_bind(void *at, uint64_t bytes, const struct radv_ps5_memory *memory, uint64_t offset) {
    bool ok = __real_radv_ps5_vrange_bind(at, bytes, memory, offset);
    gm_line(ok ? 'B' : 'b', (uint64_t)(uintptr_t)at, bytes);
    return ok;
}
bool __wrap_radv_ps5_vrange_unbind(void *at, uint64_t bytes) {
    gm_line('X', (uint64_t)(uintptr_t)at, bytes);
    return __real_radv_ps5_vrange_unbind(at, bytes);
}
