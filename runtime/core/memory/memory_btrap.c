// Base traps (step 4): the shadow's merge base of a range the GPU may write is a
// copy of the shadow's bytes when the part is submitted, 1-4 MiB ranges that neither the CPU nor the GPU changes most
// of the time (wbc2), and copying them was ~1 ms a frame on the replay thread, the frame's critical path. Guest memory
// holds the same bytes there, so the whole pages of such a range are made read-only instead (trapped == 3) and stand
// for the base: before anything writes one (a write fault, a thaw for a file read or a write-back, a read trap over
// it, an early-read watch), the hook copies the page into the base's buffer (vk_mem_lazy.c), then it is writable.
// Under frz_lock, like the read traps; the hook takes only its own leaf lock. Linux only: it needs the trap map
// (mem_trap_init), which Windows does not have.
#include "host.h"
#include "memory_int.h"

#define PAGE 0x1000ull

static void (*base_hook)(uint64_t page);   // set once, before the first trap
static int64_t nbase;                      // frz_lock: pages trapped for a base now

void mem_base_hook(void (*fn)(uint64_t page)) { base_hook = fn; }

int64_t mem_base_pages(void) { return __atomic_load_n(&nbase, __ATOMIC_RELAXED); }

// frz_lock held: the page's base copy, then the page back to read-write.
// With native pages larger than 4 KiB (the PS5's 16 KiB, Linux's MKW_EARLY_16K) an early-read watch on another 4 KiB
// page of p's native page leaves p with no access, and the hook's read of p faulted with frz_lock held: thaw_fault then
// waited for frz_lock on the thread holding it (t26-peek1, 14/11/2026: mem_early_watch -> mem_base_fill_locked ->
// lazy_hook -> fault -> thaw_fault, the game's submit thread, every other thread queued on frz_lock behind it; the
// console's run 25 hung the same way). The native page is made readable for the copy, as releasing a watch there
// does (memory_early.c), and watched again after it while another of its 4 KiB pages still is.
static int early_on_native(uint64_t p, size_t ep) {
    if (ep <= PAGE) return 0;
    uint64_t np = p & ~(uint64_t)(ep - 1);
    for (uint64_t q = np; q < np + ep && q < RT_ADDRESS_LIMIT; q += PAGE)
        if (q != p && trapped[q / PAGE] == 2) return 1;
    return 0;
}

void mem_base_fill_locked(uint64_t p) {
    p &= ~(PAGE - 1);
    size_t ep = plat_mem_early_page_size();
    uint64_t np = p & ~(uint64_t)(ep - 1);
    int early = early_on_native(p, ep);
    // opened as a released watch leaves it (read-write, write-watch arming kept), for the copy only
    if (early && plat_mem_early_protect(np, ep, 0, PLAT_PROT_RW) != PLAT_EARLY_OK)
        rt_fatal("memory: opening the early-watched page %#llx for a base copy failed", (unsigned long long)np);
    if (base_hook) base_hook(p);
    if (!plat_mem_protect(p, PAGE, PLAT_PROT_RW))
        rt_fatal("memory: giving back the base-trapped page %#llx failed", (unsigned long long)p);
    trapped[p / PAGE] = 0;
    nbase--;
    // Another guest page in this native page still has a live watch.
    if (early && plat_mem_early_protect(np, ep, 1, 0) != PLAT_EARLY_OK)
        rt_fatal("memory: rearming the early-watched page %#llx after a base copy failed", (unsigned long long)np);
}

// The whole read-write pages of [lo, hi) with no other trap become base traps; bit k of bits says page lo + k*PAGE
// was trapped (lo page aligned, hi - lo up to 64 pages a word). 0: no trap map (nothing trapped, bits clear).
int mem_base_trap(uint64_t lo, uint64_t hi, uint64_t *bits) {
    uint64_t n = (hi - lo) / PAGE;
    for (uint64_t k = 0; k < (n + 63) / 64; k++) bits[k] = 0;
    if (!n || !watched(lo, hi - lo) || !mem_trap_init()) return 0;
    frz_take();
    mem_thaw_regions_locked(lo, hi - lo);   // a frozen region is read-only as a whole: its thaw would undo the traps
    for (uint64_t x = lo, y; x < hi; x = y) {
        int st = plat_mem_state(x, hi, &y);
        if (st != PLAT_PROT_RW) continue;
        uint64_t a = x;
        while (a < y) {   // runs of pages with no other trap, one protection call each
            while (a < y && trapped[a / PAGE]) a += PAGE;
            uint64_t b = a;
            while (b < y && !trapped[b / PAGE]) b += PAGE;
            if (a == b) continue;
            for (uint64_t q = a; q < b; q += PAGE) trapped[q / PAGE] = 3;   // before the protection: a fault finds it
            if (plat_mem_protect(a, b - a, PLAT_PROT_READ)) {
                for (uint64_t q = a; q < b; q += PAGE) bits[(q - lo) / PAGE / 64] |= 1ull << ((q - lo) / PAGE % 64);
                nbase += (int64_t)((b - a) / PAGE);
            } else {
                for (uint64_t q = a; q < b; q += PAGE) trapped[q / PAGE] = 0;
            }
            a = b;
        }
    }
    frz_drop();
    return 1;
}

// The pages of [lo, hi) still base-trapped go back to read-write without a copy: their base is no longer needed.
void mem_base_release(uint64_t lo, uint64_t hi) {
    if (!trapped || !__atomic_load_n(&nbase, __ATOMIC_RELAXED)) return;
    frz_take();
    for (uint64_t a = lo; a < hi;) {
        while (a < hi && trapped[a / PAGE] != 3) a += PAGE;
        uint64_t b = a;
        while (b < hi && trapped[b / PAGE] == 3) b += PAGE;
        if (a == b) continue;
        if (!plat_mem_protect(a, b - a, PLAT_PROT_RW))
            rt_fatal("memory: releasing base-trapped %#llx+%#llx failed", (unsigned long long)a,
                     (unsigned long long)(b - a));
        for (uint64_t q = a; q < b; q += PAGE) trapped[q / PAGE] = 0;
        nbase -= (int64_t)((b - a) / PAGE);
        a = b;
    }
    frz_drop();
}
