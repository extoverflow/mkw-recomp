# runtime/ps5_sdl

PS5 build only: on the include path first (`tools/build/build.py`, the PS5 target), so `<SDL3/SDL.h>` is the
subset below; the console has no SDL3. Beside `platform/`, not in it: a folder with subfolders holds only folders
(tools/docs/check.py, upstream 3042c59).

| Folder | What it is |
|---|---|
| `SDL3/` | The SDL3 subset `input/input.c` and `audio/audio_mixer.c` call, implemented by `platform/ps5_sdl.c` |
