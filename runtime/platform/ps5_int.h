// What the parts of the PS5 platform layer share (ps5.c, ps5_sync.c, ps5_fs.c, ps5_sys.c).
#pragma once
#include <stddef.h>

void alt_stack(void);   // ps5.c: a signal stack for the calling thread
// ps5.c: the stacks' use (each one with each, then a summary line) into out; returns the length. Every stack is direct
// memory sized from a measurement on Linux, so this is how the console shows that the sizes hold.
int ps5_stack_report(char *out, size_t cap, int each);
uint64_t ps5_mem_sibling_lifts(void);   // ps5_mem.c: accesses opened past a stricter 4 KiB sibling (diagnosis)
