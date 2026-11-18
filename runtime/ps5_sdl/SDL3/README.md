# runtime/ps5_sdl/SDL3

| File | What it is |
|---|---|
| `SDL.h` | The part of SDL3's API that `input/input.c` (gamepads) and `audio/audio_mixer.c` (one audio stream) call, with SDL3's names and enumeration order; `platform/ps5_sdl.c` implements it on scePad and sceAudioOut. A call outside it does not compile |
| `SDL_messagebox.h` | `SDL_ShowSimpleMessageBox` for `core/boot/main.c`'s AVX2 check (upstream 9895782): shows nothing on the PS5 and returns false; the message goes to the log first |
