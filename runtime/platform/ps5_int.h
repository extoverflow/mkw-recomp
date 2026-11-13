// What the parts of the PS5 platform layer share (ps5.c, ps5_sync.c, ps5_fs.c, ps5_sys.c).
#pragma once

// The title's data folder: the log, args.txt, mkw.ini, the game's files, profile/,
// spirv/, caches. A title's own folder (/app0) is read-only.
#define PS5_DATA_DIR "/data/mkw-recomp"
#include <stddef.h>
#include <stdint.h>

void alt_stack(void);   // ps5.c: a signal stack for the calling thread
// ps5.c: the stacks' use (each one with each, then a summary line) into out; returns the length. Every stack is direct
// memory sized from a measurement on Linux, so this is how the console shows that the sizes hold.
int ps5_stack_report(char *out, size_t cap, int each);
uint64_t ps5_mem_sibling_lifts(void);   // ps5_mem.c: accesses opened past a stricter 4 KiB sibling (diagnosis)

// ps5_mem.c: a buffer the kernel is about to write (pread) made ready as a first write would make it (backed, marked
// written where watched): the kernel's own writes do not fault into the handler.
void ps5_mem_prepare_write(uint64_t a, uint64_t n);
// Whether the guest may write all of [a, a + n); why (may be NULL): the first kernel page that refuses writes, if any.
int ps5_mem_writable(uint64_t a, uint64_t n, char *why, size_t cap);

// ps5.c: the thread dump for a hang (PS5 only). Threads made by plat_thread_new register themselves; another (the
// title's main thread) calls ps5_dump_register. ps5_dump_threads writes every other registered thread's instruction
// pointer and the code addresses on its stack to fd (write(2), no stdio).
void ps5_dump_register(const char *name);
void ps5_dump_threads(int fd);
// The threads named name, count times every interval_ms (a profile of one thread), to fd.
void ps5_dump_sample(int fd, const char *name, int count, int interval_ms);
// ps5_mem.c: faults, backing and mprotect counts and times, as one line.
int ps5_mem_stats(char *out, size_t cap);
int ps5_mem_brief(char *out, size_t cap);
extern int ps5_log_fd;   // ps5_main.c: where raw write(2) lines reach the log
