// SPDX-License-Identifier: GPL-3.0-or-later
// The part of SDL3's SDL_messagebox.h that core/boot/main.c calls (its AVX2 check, upstream 9895782, #262), with
// SDL3's names. The PS5 has no message box here: the call shows nothing and reports failure, as SDL does without a
// video driver; main.c writes the message to stderr (the title's log) before it. The PS5's Zen 2 has AVX2 anyway.
#pragma once
#include <stdbool.h>

#define SDL_MESSAGEBOX_ERROR 0x00000010u

typedef struct SDL_Window SDL_Window;
typedef unsigned int SDL_MessageBoxFlags;

static inline bool SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags flags, const char *title, const char *message,
                                            SDL_Window *window) {
    (void)flags, (void)title, (void)message, (void)window;
    return false;
}
