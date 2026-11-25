// What memory.c shares with memory_early.c (MKW_EARLY_READ): the trap map and its lock. Not for other files.
#ifndef MEMORY_INT_H
#define MEMORY_INT_H
#include <stdint.h>

#include "platform/platform.h"

extern PlatMutex frz_lock;
// frz_lock taken and dropped through these: frz_held says the calling thread holds it, so a fault under it (which
// thaw_fault would answer by waiting for frz_lock on this very thread) stops with a named error instead of a silent
// hang (run 25, t26-peek1).
extern _Thread_local int frz_held;
static inline void frz_take(void) { plat_mutex_lock(&frz_lock); frz_held++; }
static inline void frz_drop(void) { frz_held--; plat_mutex_unlock(&frz_lock); }
extern uint8_t *trapped;                  // per guest page: 1 a read trap (memory.c), 2 an early-read watch, 3 a base
                                          // trap (memory_btrap.c), 4 a spilled page (memory_spill.c)
extern int64_t mem_early_pages;           // frz_lock: pages watched now
int watched(uint64_t addr, uint64_t size);
int mem_trap_init(void);                  // the trap map + poke; 0 when poke is unavailable
int mem_trapped_init(void);               // the trap map alone (early-read watch on PS5/Linux)
uint64_t mem_early_release_locked(uint64_t a, int access);   // the fence to wait for once unlocked, 0 none
void mem_early_resolve_hold(uint64_t a, size_t n);
void mem_thaw_regions_locked(uint64_t a, uint64_t n);   // frz_lock held: no region over [a, a+n) stays frozen
void mem_base_fill_locked(uint64_t p);   // frz_lock held: a base-trapped page copied to its base, then writable
void mem_spill_fill_locked(uint64_t p, int why);   // frz_lock held: a spilled page (and the next ones) back from the
                                                   // file, read-write; why 1: an access, 2: a thaw
void mem_early_filled_locked(uint64_t a, int access);   // a trapped page was filled from the live shadow
// memory_early.c's state its diagnosis reads (memory_early_diag.c)
void mem_early_page_fences(uint64_t addr, uint64_t *lastf, uint64_t *prevf);
extern uint64_t (*mem_fence_done)(void), (*mem_fence_queued)(void);
extern uint64_t mem_early_frame;                                // the frame mem_early_log saw last
void mem_early_ev_note(int kind, uint64_t addr, uint64_t w, uint64_t done, uint64_t pc);   // MKW_EARLY_READ_DRY
void mem_early_ev_log(void);                                    // every frame
int mem_early_ev_counts(char *b, int n);                        // the 600-frame line's counts, then reset
void mem_early_wait(uint64_t fence);   // frz_lock held: a's page gets its protection back; access:
                                                         // a fault at a (the byte touched)
#endif
