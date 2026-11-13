// PS5 build only, until Step 3 gives the console its own window (VK_KHR_display), pad and sound: stand-ins for what
// the SDL3 front end provides on Linux and Windows (input/input.c, audio/audio_mixer.c, gfx/nvn/host/gfx_window_sdl.c,
// ui/launcher/launcher.cpp; tools/build/build.py PS5_WITHOUT leaves those out). Nothing pressed, nothing heard, no
// window: enough for the runtime's self test and its probes that need neither (Step 2's pass).
#include <math.h>
#include <stdint.h>

#include "audren.h"
#include "gfx.h"
#include "gfx_wsys.h"
#include "input.h"
#include "launcher.h"

// ---- input (input.c): no device
void input_poll(uint64_t *buttons, int32_t *lx, int32_t *ly, int32_t *rx, int32_t *ry) {
    *buttons = 0, *lx = *ly = *rx = *ry = 0;
}
void input_poll_player(int player, uint64_t *buttons, int32_t *lx, int32_t *ly, int32_t *rx, int32_t *ry) {
    (void)player;
    input_poll(buttons, lx, ly, rx, ry);
}
void input_poll_ui(uint64_t *buttons, int32_t *lx, int32_t *ly, int32_t *rx, int32_t *ry) {
    input_poll(buttons, lx, ly, rx, ry);
}
int input_pad_read(uint64_t *buttons, int32_t *lx, int32_t *ly) {
    *buttons = 0, *lx = *ly = 0;
    return 0;
}
void input_block(int on) { (void)on; }
int input_capture(int reset, int *id) {
    (void)reset, (void)id;
    return 0;
}
int input_cursor_released(void) { return 0; }
int input_flight_key(void) { return 0; }
int input_hack_key(int which) {
    (void)which;
    return 0;
}
uint32_t input_key_char(int key) {
    (void)key;
    return 0;
}
int input_last_device(void) { return 0; }
void input_map_snapshot(struct InputMap *m) { (void)m; }
void input_menu(int open) { (void)open; }
void input_motion_axes(const float sdl_acc[3], const float sdl_gyro[3], double acc_g[3], double gyro[3]) {
    (void)sdl_acc, (void)sdl_gyro;
    for (int i = 0; i < 3; i++) acc_g[i] = gyro[i] = 0;
}
int input_motion_enabled(void) { return 0; }
void input_motion_start(void) {}
int input_pad_label(int pad) {
    (void)pad;
    return 0;
}
int input_pad_layout(void) { return 0; }
uint32_t input_player_mask(void) { return 1; }   // bit 0 always, as input.h says
int input_rumble(int player, uint16_t low, uint16_t high, uint32_t ms) {
    (void)player, (void)low, (void)high, (void)ms;
    return 0;
}
int input_rumble_triggers_enabled(void) { return 0; }
// The same formula as input.c's (a pure function the guest's vibration requests go through).
uint16_t rumble_motor(float a, float curve, float strength) {
    if (!(a > 0) || !(strength > 0)) return 0;
    float v = powf(a > 1 ? 1 : a, curve) * strength * 65535.0f + 0.5f;
    return v >= 65535.0f ? 65535 : (uint16_t)v;
}

// ---- sound (audio_mixer.c): the renderer runs, its output goes nowhere
const char audio_device_name[] = "AudioTvOutput";
int audio_output_channels(void) { return 2; }
float audio_device_volume(void) { return 1.0f; }
void audio_device_set_volume(float v) { (void)v; }
void audio_mixer_open(AudioRenderer *r) { (void)r; }
void audio_mixer_close(AudioRenderer *r) { (void)r; }
void audio_mixer_render(AudioRenderer *r, double seconds) { (void)r, (void)seconds; }
void audio_mixer_set_running(AudioRenderer *r, int running) { (void)r, (void)running; }
void audio_mixer_effect_result(AudioRenderer *r, int id, void *output, unsigned size) {
    (void)r, (void)id, (void)output, (void)size;
}
uint32_t audio_mixer_sink_written(AudioRenderer *r, int id) {
    (void)r, (void)id;
    return 0;
}
void audio_mixer_update_effects(AudioRenderer *r) { (void)r; }
void audio_mixer_update_routing(AudioRenderer *r, const uint8_t *splitters, uint32_t splitter_bytes,
                                const uint8_t *mixes, uint32_t mix_bytes, int mix_count) {
    (void)r, (void)splitters, (void)splitter_bytes, (void)mixes, (void)mix_bytes, (void)mix_count;
}

// ---- window (gfx_window_sdl.c): none yet
int wsys_open(void) { return 0; }
VkSurfaceKHR wsys_surface(VkInstance inst) {
    (void)inst;
    return VK_NULL_HANDLE;
}
void wsys_client_size(int *w, int *h) { *w = 1920, *h = 1080; }
int wsys_hdr(float *sdr_white, float *headroom) {
    (void)sdr_white, (void)headroom;
    return 0;
}
int wsys_minimized(void) { return 1; }
int wsys_probe_click(int where) {
    (void)where;
    return 0;
}
void wsys_probe_escape(void) {}
void wsys_probe_focus(int in) { (void)in; }
void wsys_resize(int w, int h) { (void)w, (void)h; }
void gfx_window_apply_settings(void) {}
int gfx_window_lines(void) { return 1080; }
int gfx_window_monitor_size(int *w, int *h) {
    *w = 1920, *h = 1080;
    return 1;
}
void gfx_window_request_close(void) {}
int gfx_window_shape(int *w, int *h) {
    *w = 1920, *h = 1080;
    return 1;
}
int gfx_window_vk_extensions(const char **out, int max) {
    (void)out, (void)max;
    return 0;
}
int gfx_window_vk_presentable(VkInstance inst, VkPhysicalDevice pd, uint32_t family) {
    (void)inst, (void)pd, (void)family;
    return 0;
}

// ---- start screen (launcher.cpp): none; straight into the mode asked for
int launcher_run(void) { return LAUNCHER_PLAY; }
