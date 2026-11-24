# PlayStation 5

mkw-recomp as a native PS5 homebrew title: the same recompiled game code and runtime as
the desktop builds, compiled for the console's Zen 2 CPU, with NVN2 on top of Mesa's RADV for the PS5. Like the
desktop builds it needs your own copy of the game, and the title you build contains it: never share it.

## Status (23/11/2026)

- It boots, plays the movies, goes through the menus and races, with the console's DualSense controllers and sound.
- Only Mario Kart World 1.8.0 has been tested on the console.
- A cup 0 Grand Prix (four races, 1080p) averages 56-57 fps at the PS5's fixed 60 (24/11/2026): the first race holds
  59-60; in the heavier stretches of the later races a slow frame slows the race a little (game speed 0.97-0.98x)
  instead of dropping to 30 fps. The slowest frames come where a course streams in new textures.
- Known problems: a rare one-frame flash where the 3D scene is blank, and a rare crash in the game's code
  (`f_300192aa74`). Free memory goes down race after race; long sessions are still being measured.

## What you need

- **A jailbroken PS5** running a homebrew payload loader, `kstuff` and
  [ShadowMountPlus](https://github.com/drakmor/ShadowMountPlus), which registers the titles it finds in
  `/data/homebrew/` and `/mnt/ext1/homebrew/` (the M.2 drive) on the home screen.
- **Your own Mario Kart World 1.8.0 dump** (`exefs/` and `romfs/`, decrypted, the update merged into the base RomFS),
  about 23 GB.
- **A Linux PC** with what the desktop build needs ([Building from source](../../README.md#building-from-source)),
  plus the PS5 toolchain below, and about 30 GB free.

## Building

The game code and the shaders are made exactly as for the desktop:

```sh
uv run python recomp/gen.py --exefs game/exefs --jobs 8
uv run tools/shaders/extract.py && uv run tools/shaders/translate.py
```

Then the PS5 objects, with three paths set (details in [tools/build/README.md](../../tools/build/README.md)):

```sh
PS5_PAYLOAD_SDK=<payload SDK> PS5_LIBS=<console libraries> LLVM_CONFIG=<llvm-config> \
    uv run tools/build/build.py --target ps5 --compile-only
```

- `PS5_PAYLOAD_SDK`: the payload SDK of [PS5_VulkanTemplate](https://github.com/mihawk-99/PS5_VulkanTemplate)
  (`prospero-clang` and the console's libc), whose clang follows `LLVM_CONFIG`.
- `PS5_LIBS`: the console builds of zstd, Opus and libvpx, and the Vulkan headers RADV was built with (from the
  [ps5-payload-dev](https://github.com/ps5-payload-dev) toolchain bundle).
- The objects land in `build/obj-ps5/` (`rt/` and `lib{main,subsdk0,subsdk1,sdk}.a`); `--compile-only` stops there.

The title is linked against RADV for the PS5: [PS5_Vulkan](https://github.com/mihawk-99/PS5_Vulkan) builds it from
[PS5_Mesa](https://github.com/mihawk-99/PS5_Mesa) (`tools/build-radv.sh release`) and links it into a title
(`tools/radv-link.sh`), the way PS5_VulkanTemplate's `ps5/tools/link-title.sh` does. Link with `--wrap=fprintf`
(the log's writer thread takes every line, RADV's too). The title folder is PS5_VulkanTemplate's with a `titleId`
of your own (`PPSA` and five digits no other installed title uses): `eboot.bin`, `sce_sys/param.json`,
`sce_sys/icon0.png` and `sce_module/libc.prx`.

The shaders for the console are the `.spv` and `.res.json` files of `build/spirv/` (about 23 000 files, 1 GB).

## Installing

On the console, with any file transfer (FTP, the web file manager):

| What | Where |
|---|---|
| The title folder | `/data/homebrew/<titleId>/` or `/mnt/ext1/homebrew/<titleId>/` (one of the two) |
| Your dump (`exefs/`, `romfs/`) | any folder, for example `/mnt/ext1/mkw/game/` |
| The shaders | any folder, for example `/mnt/ext1/mkw/spirv/` |
| Settings, log, saves | `/data/mkw-recomp/` (fixed) |

Then in `/data/mkw-recomp/`:

- `mkw.ini`: where the game is, as absolute paths:

  ```ini
  exefs = /mnt/ext1/mkw/game/exefs
  romfs = /mnt/ext1/mkw/game/romfs
  ```

- `args.txt`: one line of `KEY=value` settings, read at start. At least the shaders:

  ```
  MKW_SPIRV=/mnt/ext1/mkw/spirv MKW_PS5_LOG=min
  ```

`eboot.bin` and `sce_module/libc.prx` must be executable: files an FTP client left at mode 0644 are refused at launch
with "Can't start the game or app" (CE-107750-0); give the title folder mode 0755 or 0777. After replacing
`eboot.bin`, the first launch sometimes does not start the title; launching again works.

## Playing

Start the title from the home screen. The first session compiles the pipelines as it goes and stutters until
they are cached (`pipeline-cache.bin` and `pipeline-keys.bin` in `/data/mkw-recomp/`).

- The log is `/data/mkw-recomp/mkw.log` (the previous run's is `mkw-prev.log`); saves are in `profile/`.
- To close the title without the controller, create an empty `/data/mkw-recomp/quit`: the title ends within 2 s.
- Reading `mkw.log` while the title runs (for example from the web file manager) can stall the game for up to a
  second: read it after the title ends.

## Settings for the PS5

The PS5 device profile (`MKW_DEVICE_PROFILE=auto`) sets defaults for the keys `mkw.ini` and `args.txt` do not set,
among them a fixed 60 fps (`MKW_FPS=60`: smoother than `auto`, which drops single frames to 30), 1080-line screen
passes with the game's own 3D resolution, parallel recording on five threads, a GPU
memory cap of 4608 MiB, the pipeline cache saved every 10 minutes, and the shaders kept compressed in memory
(`runtime/core/base/cfg.c`, the `PS5[]` table). `MKW_DEVICE_PROFILE=desktop` turns it off.

| Setting | What it does |
|---|---|
| `MKW_SPIRV` | The folder of the translated shaders |
| `MKW_PS5_LOG=min` | Only the essential log lines; for play and measurements |
| `MKW_PS5_HEARTBEAT=0` | No `alive` file (test tools read it to know the title runs) |
| `MKW_PS5_DMEM_RESERVE_MIB` | Direct memory kept free for the game when the GPU asks for more (default 512) |
| `MKW_PS5_WW_HOT_AFTER` | How soon a page the game writes all the time stays writable (1-63, lower: fewer faults) |
| `MKW_BENCH=gp` | Benchmark: a Grand Prix with no player, timed, then the title ends (`bench:` lines in the log). Test builds only: `build.py --bench` |

## How it fits in the runtime

- `runtime/platform/ps5*.c`: the platform layer (threads, memory, files, clock and locks, window, the SDL3 subset for
  pads and sound); [runtime/platform/README.md](../../runtime/platform/README.md) lists each file.
- `runtime/ps5_sdl/`: the part of SDL3's API the input and audio code calls, so they build unchanged.
- Guest memory is backed with direct memory at first touch; the game's writes to memory the GPU reads are found with
  page protection, as on Linux without `userfaultfd`.
- RADV is linked into the title; `runtime/platform/ps5_vk.c` asks it for each Vulkan entry point by name.
