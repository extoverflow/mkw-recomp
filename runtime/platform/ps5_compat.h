// PS5 build only (tools/build/build.py --target ps5 includes this first in every runtime file): declarations the
// payload SDK's headers lack for functions the console's libraries have. Nothing here when __PROSPERO__ is not defined.
// It includes no system header: one pulled into every file brings names a file may use for itself (pthread.h brings
// signal.h, and gfx/nvn/api/nvn_qorder.c has its own static signal()).
#pragma once
#ifdef __PROSPERO__
// libkernel exports glibc's pthread_getname_np (and not FreeBSD's pthread_get_name_np, which pthread_np.h declares);
// pthread_t is struct pthread * (sys/_pthreadtypes.h).
struct pthread;   // declared at file scope first, so the prototype's type is the same struct as pthread_t's
int pthread_getname_np(struct pthread *, char *, __SIZE_TYPE__);
// The PS5's CPU is a fixed Zen 2, which has every feature the runtime checks for (sha, sse4.1: core/base/sha256.c; avx2,
// fma, bmi2: core/boot/main.c), and the payload SDK has no compiler-rt for __builtin_cpu_supports' tables. A check for
// a feature Zen 2 lacks (AVX-512) would need its own answer here.
#define __builtin_cpu_init() ((void)0)
#define __builtin_cpu_supports(feature) 1
// The runtime's main() (core/boot/main.c) is called by the title's entry, platform/ps5_main.c, as mkw_main: that file
// reads the arguments from a file and sets up the log first.
#define main mkw_main
// RADV is linked into the title and defines Mesa's vk_sync_finish; the runtime's own (gfx/vk/device/vk_sync.c) takes
// another name here (the only global both define, by llvm-nm over both, 13/11/2026).
#define vk_sync_finish mkw_vk_sync_finish
#endif
