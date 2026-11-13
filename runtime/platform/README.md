# runtime/platform

| File | What it is |
|---|---|
| `platform.h` | The operating system interface the rest of the runtime uses |
| `linux.c`, `linux_int.h` | Linux: threads, preemption through signals, fibers; the header is what the `linux_*.c` parts share |
| `linux_sync.c` | Linux: clock, locks and condition variables on futex |
| `linux_fs.c` | Linux: files and file system (atomic write, rename without replace) |
| `linux_mem.c` | Linux: virtual memory, write watching with userfaultfd (Linux 6.7+), fault handler; `plat_mem_poke` writes through the protection via `/proc/self/mem` |
| `linux_sys.c` | Linux: process, local IPC (Unix sockets), desktop, libraries, machine load (CPU, temperature, GPU) and desktop shortcut |
| `win32.c`, `win32_int.h` | Windows: threads, preemption (SuspendThread), fibers, clock, locks; the header is what the `win32_*.c` parts share |
| `win32_fs.c` | Windows: files and file system (UTF-16 paths) |
| `win32_mem.c` | Windows: virtual memory and write watching (`MEM_WRITE_WATCH`) |
| `win32_sys.c` | Windows: process, local IPC (named pipes), desktop and libraries |
| `win32_load.c` | Windows: machine load (CPU, PDH for temperature and GPU) and desktop shortcut |
| `common.c`, `fp_x86.h` | Common parts, x86 floating point control |
| `ps5.c`, `ps5_int.h` | PS5: threads (stacks in direct memory, sized from a measurement), interruption by signal, fibers, affinity; the header is what the `ps5*.c` parts share |
| `ps5_main.c` | PS5: the title's entry (arguments and settings from `args.txt`, the log file, the splash, the exit through the shell; `--ps5-pad-test`, `--ps5-tone-test`) |
| `ps5_mem.c` | PS5: reservations, direct memory backed at the first touch per 16 KiB page, the 4 KiB guest state, faults and crashes |
| `ps5_frontend.c` | PS5: stand-ins for the parts of the SDL3 front end a console has no use for (keyboard, mouse, start screen) |
| `ps5_window.c` | PS5: the window back end (`gfx_wsys.h`) on the one display through RADV's `VK_KHR_display` |
| `ps5_sdl.c`, `ps5_sdl/SDL3/SDL.h` | PS5: the SDL3 subset `input/input.c` and `audio/audio_mixer.c` call, on scePad and sceAudioOut |
| `ps5_vk.c` | PS5: the Vulkan entry points the runtime calls by name, each asking the linked-in RADV for its own (written by the port's generator script from RADV's `vulkan_core.h`) |
| `ps5_sync.c` | PS5: clock, locks and condition variables (`linux_sync.c` on FreeBSD's `_umtx_op`) |
| `ps5_fs.c` | PS5: files and file system (`linux_fs.c`; no `renameat2`, the temporary directory under `/data`) |
| `ps5_sys.c` | PS5: process, libraries, time zone, randomness; no desktop, clipboard, child processes or load counters |
| `ps5_compat.h` | PS5: declarations the payload SDK's headers lack for functions the console has (`pthread_getname_np`); `build.py --target ps5` includes it first in every file |
| `mkw.rc`, `mkw.manifest` | `mkw.exe`'s manifest (UTF-8 code page, from totk-pc-recomp); `build.py --target windows` |
