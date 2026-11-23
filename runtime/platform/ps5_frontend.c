// PS5 build only: stand-ins for the parts of the SDL3 front end the console has no use for or does not have yet
// (tools/build/build.py PS5_WITHOUT leaves the files out): no keyboard or mouse (input/input_sdl.c), no start screen
// (ui/launcher/launcher.cpp). The window is platform/ps5_window.c's; input/input.c and audio/audio_mixer.c run on
// platform/ps5_sdl.c's SDL3 subset.
#include <math.h>
#include <stdint.h>

#include "gfx.h"
#include "gfx_wsys.h"
#include "input.h"
#include "input_wsys.h"
#include "switch2.h"
#include "cfg.h"
#include "launcher.h"
#include "ui/launcher/libs_fetch.h"

// ---- keyboard and mouse (input_wsys.h; input_sdl.c on Linux): none on a console. input.c is built, on
// platform/ps5_sdl.c's gamepads.
int ikb_foreground(void) { return 1; }   // the title is always in front while it runs
int ikb_down(int key) {
    (void)key;
    return 0;
}
int ikb_escape(void) { return 0; }
int ikb_captured(void) { return 0; }
void ikb_mouse_take(int32_t *dx, int32_t *dy) { *dx = *dy = 0; }
void ikb_cursor_update(void) {}
void ikb_menu_closed(void) {}
// Switch 2 controllers over Bluetooth LE (input/switch2.c, with linux_switch2.c's or switch2_win.c's transport): no
// transport on the console, so MKW_SWITCH2 has no effect here; the controllers the console pairs come as its own pads.
void switch2_start(void) {
    if (cfg_int("MKW_SWITCH2")) rt_log("input: Switch 2 controllers over Bluetooth are not available on the PS5");
}

// input.h's keyboard queries that input_sdl.c answers on Linux: no key is ever down
uint32_t input_key_char(int key) {
    (void)key;
    return 0;
}
int input_flight_key(void) { return 0; }
int input_hack_key(int which) {
    (void)which;
    return 0;
}

// input_power.c (upstream 3042c59) asks the Switch 2 controllers' battery first: none here (0, as switch2.c for a pad
// that is not one of its own)
int switch2_battery(uint32_t sdl_id, int *mv) {
    (void)sdl_id;
    if (mv) *mv = 0;
    return 0;
}

// ---- racing wheels (input/wheel.c, upstream 3042c59): SDL's joystick API maps wheels as gamepads; the console's pads
// come from scePad (platform/ps5_sdl.c), and no wheel is opened there
void input_wheel_hint(void) {}
void input_wheel_map(void) {}

// ---- optional libraries (ui/launcher/libs_fetch.cpp, upstream 96d9eda: DLSS, FSR 3.1 downloaded by libs.py): none on
// the PS5, whose RADV has neither; the menu sees no fetch running and none finished
LibsState libs_state(void) {
    LibsState s = {0};
    return s;
}
void libs_fetch_start(const char *features) { (void)features; }

// ---- --probe-pads (probes/pads_probe.c, upstream 96d9eda): it attaches virtual SDL gamepads, which the PS5 SDL subset
// does not have (its pads are scePad's)
int probe_pads(void) {
    rt_log("probe-pads: not on the PS5 (virtual SDL gamepads)");
    return 1;
}

// ---- window: platform/ps5_window.c

// ---- start screen (launcher.cpp): none; straight into the mode asked for
int launcher_run(void) { return LAUNCHER_PLAY; }
// The paths main.c hands the launcher's pages (upstream 8dd7801: the Profiles and Mods pages, a GameBanana 1-Click link):
// no launcher on the PS5, so they are not kept.
void launcher_set_romfs(const char *romfs) { (void)romfs; }
void launcher_set_profile(const char *profile) { (void)profile; }
void launcher_set_exefs(const char *exefs) { (void)exefs; }
void launcher_gb_install(const char *url) { (void)url; }
