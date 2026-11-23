// From mkw-pit-stop (MIT, Copyright (c) 2026 Adrià; PS5 work by exploitizm) runtime/core/diag/bench.h @ 0a0259d.
// Benchmark mode (bench.c): MKW_BENCH=tt|gp boots into a time trial or a Grand Prix, the game's own assists drive it,
// and the run ends at the finish with frame-time statistics of the races in the log.
// Test builds only: compiled in with MKW_WITH_BENCH (tools/build/build.py --bench). Without it the hooks below are
// empty inline functions, bench.c is empty, and MKW_BENCH is not a setting: a release carries none of it.
#pragma once
#include <stdint.h>

#ifdef MKW_WITH_BENCH
int bench_on(void);                            // MKW_BENCH is set
int bench_save(uint8_t *save, uint64_t size);  // fs_host.c: Play/account.sav's working copy at open; 1 when changed
void bench_save_write(const char *host_path);  // fs_host.c: a save file opened for writing (the finish's save)
void bench_romfs_opened(const char *path);     // fs_host.c: a romfs file opened (the cup's ending movie)
void bench_frame(uint64_t frame);              // perf.c: each present
#else
static inline int bench_on(void) { return 0; }
static inline int bench_save(uint8_t *save, uint64_t size) { (void)save, (void)size; return 0; }
static inline void bench_save_write(const char *host_path) { (void)host_path; }
static inline void bench_romfs_opened(const char *path) { (void)path; }
static inline void bench_frame(uint64_t frame) { (void)frame; }
#endif
