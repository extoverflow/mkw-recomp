// Weak defaults of the platform calls the early-read watch uses (memory_early.c), for the platforms that do not give
// their own: the watch protects 4 KiB pages through plat_mem_protect as before, and the sibling-read emulation is not
// available. The PS5's are in platform/ps5_mem.c. From mkw-pit-stop (MIT, Copyright (c) 2026 Adrià; PS5 work by
// exploitizm) runtime/platform/common.c:174-182, 215-231 @ 0109457.
#include <stddef.h>
#include <stdint.h>

#include "platform/platform.h"

__attribute__((weak)) size_t plat_mem_early_page_size(void) { return 4096; }
__attribute__((weak)) PlatEarlyResult plat_mem_early_protect(uint64_t addr, size_t n, int on, int restore_prot) {
    if (plat_mem_protect(addr, n, on ? PLAT_PROT_NONE : restore_prot)) return PLAT_EARLY_OK;
    return on ? PLAT_EARLY_FAILED : PLAT_EARLY_UNRESTORED;
}
__attribute__((weak)) int plat_mem_early_open_read(uint64_t addr, size_t n) { return (void)addr, (void)n, 0; }
__attribute__((weak)) uint32_t plat_mem_early_gen(uint64_t addr) { return (void)addr, 0; }
__attribute__((weak)) void plat_mem_early_hold_hook(void (*fn)(uint64_t addr, size_t n)) { (void)fn; }
__attribute__((weak)) int plat_code_init(void) { return 0; }
__attribute__((weak)) int plat_fault_emulate_load(uint64_t fault_addr, int extra, int (*rd)(uint64_t, size_t, void *)) {
    return (void)fault_addr, (void)extra, (void)rd, 0;
}
__attribute__((weak)) int plat_fault_step(void (*fn)(uint64_t), uint64_t arg) { return (void)fn, (void)arg, 0; }
__attribute__((weak)) int plat_sample_self_name(char *out, size_t cap) { (void)out, (void)cap; return 0; }
