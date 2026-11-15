// PS5 build only: the SDL3 subset of platform/ps5_sdl/SDL3/SDL.h on the console's services, so input/input.c (and
// audio/audio_mixer.c) run unchanged. Gamepads: one per signed-in user, as the console pairs each controller with a
// user and scePadOpen opens a user's. input.c makes every gamepad call under its own lock, so this keeps no lock.
//
// The scePad calls, the 120-byte sample and its fields, the user scan and the two-motor rumble mode follow
// PS5_VulkanTemplate ps5/src/platform.c @ 91179df (MIT, Copyright (C) 2026 Mihawk): struct pad_sample :95-107,
// PAD_VIBRATION_COMPATIBLE :111, pad_player_open :153, pad_scan :194, pad_open :223, pad_poll :265,
// pad_player_vibrate :334; the button bits and PAD_INTERCEPTED from its platform.h :26-41, :73.
#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

#include "gfx/nvn/host/gfx_wsys.h"
#include "host.h"
#include "platform/platform.h"

int sceUserServiceInitialize(const void *params);
int sceUserServiceGetInitialUser(int32_t *user_id);
struct user_list {
    int32_t user_id[4];
};
int sceUserServiceGetLoginUserIdList(struct user_list *list);
int scePadInit(void);
int scePadOpen(int32_t user_id, int32_t port_type, int32_t index, const void *params);
int scePadGetHandle(int32_t user_id, int32_t port_type, int32_t index);
int scePadRead(int32_t handle, void *samples, int32_t capacity);
int scePadClose(int32_t handle);
int scePadSetVibrationMode(int32_t handle, int32_t mode);
int scePadSetVibration(int32_t handle, const void *vibration);

static char g_error[160] = "";

static bool fail(const char *what) {
    snprintf(g_error, sizeof g_error, "%s", what);
    return false;
}

const char *SDL_GetError(void) { return g_error; }
void SDL_free(void *mem) { free(mem); }
void SDL_SetEventEnabled(Uint32 type, bool enabled) { (void)type, (void)enabled; }   // no event queue here

// ---- gamepads
struct pad_sample {   // one reading as scePadRead returns it
    uint32_t buttons;
    uint8_t left_x, left_y, right_x, right_y, l2, r2;   // sticks 0..255, 128 at rest, y down-positive; triggers 0..255
    uint8_t reserved[66];
    int32_t connected;
    uint64_t timestamp_us;
    uint8_t extension[16];
    uint8_t connected_count;
    uint8_t remaining[15];
};
_Static_assert(sizeof(struct pad_sample) == 120, "the console's pad samples are 120 bytes");
_Static_assert(offsetof(struct pad_sample, connected) == 0x4c, "connection state sits at 0x4c");

enum {   // the console's button bits
    B_L3 = 0x2, B_R3 = 0x4, B_OPTIONS = 0x8, B_UP = 0x10, B_RIGHT = 0x20, B_DOWN = 0x40, B_LEFT = 0x80,
    B_L1 = 0x400, B_R1 = 0x800, B_TRIANGLE = 0x1000, B_CIRCLE = 0x2000, B_CROSS = 0x4000, B_SQUARE = 0x8000,
    B_TOUCH_PAD = 0x100000,
};
#define INTERCEPTED 0x80000000u   // the shell has the pad (home screen, a system dialog): not the title's input
#define VIBRATION_COMPATIBLE 2    // the two-motor rumble (the other mode is the DualSense's haptics)
#define PLAYERS 4
#define SCAN_NS 1000000000ull

struct SDL_Gamepad {
    int32_t user, handle;         // handle -1: no controller
    bool connected, opened;       // opened: SDL_OpenGamepad gave it out and SDL_CloseGamepad has not taken it back
    uint64_t reopen_at;           // users_scan: when a disconnected pad may be opened again (every 5 s at most)
    struct pad_sample last;       // the latest reading the title may use (neutral while the shell has the pad)
    uint64_t rumble_until_ns;     // 0: not rumbling
};

static SDL_Gamepad g_pads[PLAYERS];
static bool g_pad_service;
static uint64_t g_next_scan;

static void pad_rest(SDL_Gamepad *g) {
    memset(&g->last, 0, sizeof g->last);
    g->last.left_x = g->last.left_y = g->last.right_x = g->last.right_y = 128;
}

static void pad_read(SDL_Gamepad *g) {
    static struct pad_sample s[64];
    bool was = g->connected;
    int n = scePadRead(g->handle, s, 64);
    if (n < 0) g->connected = false;
    for (int i = 0; i < n && i < 64; i++) {
        g->connected = s[i].connected != 0;
        if (!g->connected || (s[i].buttons & INTERCEPTED)) pad_rest(g);
        else g->last = s[i];
    }
    if (g->connected != was)
        rt_log("input: ps5: pad of user %d %s (handle %d%s)", (int)g->user, g->connected ? "connected" : "disconnected",
               (int)g->handle, n < 0 ? ", read failed" : "");
}

static bool player_open(int p, int32_t user, int attempts) {
    SDL_Gamepad *g = &g_pads[p];
    int32_t h = -1;
    for (int a = 0; a < attempts && h < 0; a++) {   // a title can start before the pad service has the device
        h = scePadOpen(user, 0, 0, NULL);
        if (h < 0 && a + 1 < attempts) plat_sleep_ms(100);
    }
    if (h < 0) h = scePadGetHandle(user, 0, 0);   // one this process already holds for the user
    if (h < 0) return false;
    g->user = user, g->handle = h, g->connected = true, g->rumble_until_ns = 0;
    pad_rest(g);
    int mode = scePadSetVibrationMode(h, VIBRATION_COMPATIBLE);
    pad_read(g);
    rt_log("input: ps5: pad of user %d opened as player %d (handle %d, rumble mode %d, %s)", (int)user, p + 1, (int)h,
           mode, g->connected ? "connected" : "not connected");
    return true;
}

static void player_close(int p) {
    SDL_Gamepad *g = &g_pads[p];
    if (g->handle < 0) return;
    scePadClose(g->handle);
    g->handle = g->user = -1, g->connected = g->opened = false;
}

static bool player_open(int p, int32_t user, int attempts);

// The signed-in users again: a user who signed out lets their controller go, a new one's takes the first free slot.
// A signed-in user's pad that reads as disconnected is opened again: a handle stays with the controller it was opened
// on, and a new one (a Remote Play session started again, a controller paired again) is only reached by a new open
// (13/11/2026: after a Remote Play restart the game had no controller until the title restarted).
static void users_scan(void) {
    struct user_list list = {{-1, -1, -1, -1}};
    if (sceUserServiceGetLoginUserIdList(&list) < 0) return;
    for (int p = 0; p < PLAYERS; p++) {
        SDL_Gamepad *g = &g_pads[p];
        if (g->handle < 0 || g->connected || plat_now_ns() < g->reopen_at) continue;
        g->reopen_at = plat_now_ns() + 5 * SCAN_NS;
        int32_t user = g->user;
        bool opened = g->opened;
        scePadClose(g->handle);
        g->handle = -1;
        if (player_open(p, user, 1)) g->opened = opened;   // the game's open SDL_Gamepad keeps working
        else g->user = user, g->connected = false;          // tried again at the next scan
    }
    for (int p = 0; p < PLAYERS; p++) {
        if (g_pads[p].handle < 0) continue;
        bool in = false;
        for (int i = 0; i < 4; i++) in |= list.user_id[i] >= 0 && list.user_id[i] == g_pads[p].user;
        if (!in) player_close(p);
    }
    for (int i = 0; i < 4; i++) {
        int32_t u = list.user_id[i];
        bool known = u < 0;
        for (int p = 0; p < PLAYERS && !known; p++) known = g_pads[p].handle >= 0 && g_pads[p].user == u;
        for (int p = 0; p < PLAYERS && !known; p++)
            if (g_pads[p].handle < 0) {
                player_open(p, u, 1);
                break;
            }
    }
}

static bool gamepad_init(void) {
    if (g_pad_service) return true;
    for (int p = 0; p < PLAYERS; p++) g_pads[p].handle = g_pads[p].user = -1, pad_rest(&g_pads[p]);
    (void)sceUserServiceInitialize(NULL);
    int32_t user = -1;
    if (sceUserServiceGetInitialUser(&user) < 0 || scePadInit() < 0) return fail("no user or no pad service");
    g_pad_service = true;
    if (!player_open(0, user, 10)) rt_log("input: ps5: no pad for the user who started the title (%d)", (int)user);
    users_scan();
    g_next_scan = plat_now_ns() + SCAN_NS;
    return true;
}

// Instance ids are the slot + 1: they stay while the controller does.
SDL_JoystickID *SDL_GetGamepads(int *count) {
    if (g_pad_service && plat_now_ns() >= g_next_scan) {
        users_scan();
        g_next_scan = plat_now_ns() + SCAN_NS;
    }
    SDL_JoystickID *ids = calloc(PLAYERS + 1, sizeof *ids);
    int n = 0;
    for (int p = 0; ids && p < PLAYERS; p++)
        if (g_pads[p].handle >= 0 && g_pads[p].connected) ids[n++] = (SDL_JoystickID)(p + 1);
    if (count) *count = n;
    return ids;
}

SDL_Gamepad *SDL_OpenGamepad(SDL_JoystickID id) {
    if (id < 1 || id > PLAYERS || g_pads[id - 1].handle < 0) {
        fail("no such gamepad");
        return NULL;
    }
    g_pads[id - 1].opened = true;
    return &g_pads[id - 1];
}

void SDL_CloseGamepad(SDL_Gamepad *g) {
    if (g) g->opened = false;   // the controller stays the user's; only a sign-out lets it go
}

bool SDL_GamepadConnected(SDL_Gamepad *g) { return g && g->handle >= 0 && g->connected; }
SDL_JoystickID SDL_GetGamepadID(SDL_Gamepad *g) { return g ? (SDL_JoystickID)(g - g_pads + 1) : 0; }
const char *SDL_GetGamepadName(SDL_Gamepad *g) { return g ? "PS5 controller" : NULL; }
SDL_PropertiesID SDL_GetGamepadProperties(SDL_Gamepad *g) { return SDL_GetGamepadID(g); }

bool SDL_GetBooleanProperty(SDL_PropertiesID props, const char *name, bool default_value) {
    if (props && !strcmp(name, SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN)) return true;   // both motors, compatible mode
    return default_value;
}

SDL_GamepadButtonLabel SDL_GetGamepadButtonLabel(SDL_Gamepad *g, SDL_GamepadButton b) {
    (void)g;
    return b == SDL_GAMEPAD_BUTTON_SOUTH ? SDL_GAMEPAD_BUTTON_LABEL_CROSS
         : b == SDL_GAMEPAD_BUTTON_EAST  ? SDL_GAMEPAD_BUTTON_LABEL_CIRCLE
         : b == SDL_GAMEPAD_BUTTON_WEST  ? SDL_GAMEPAD_BUTTON_LABEL_SQUARE
         : b == SDL_GAMEPAD_BUTTON_NORTH ? SDL_GAMEPAD_BUTTON_LABEL_TRIANGLE
                                         : SDL_GAMEPAD_BUTTON_LABEL_UNKNOWN;
}

void SDL_UpdateGamepads(void) {
    uint64_t now = plat_now_ns();
    for (int p = 0; p < PLAYERS; p++) {
        SDL_Gamepad *g = &g_pads[p];
        if (g->handle < 0) continue;
        pad_read(g);
        if (g->rumble_until_ns && now >= g->rumble_until_ns) {   // SDL stops the motors after the duration
            static const uint8_t off[2];
            scePadSetVibration(g->handle, off);
            g->rumble_until_ns = 0;
        }
    }
}

// The console's bit for an SDL button by position; BACK is the touch pad's click (the Create button is not in the
// sample), GUIDE (the PS button) belongs to the shell.
static uint32_t button_bit(SDL_GamepadButton b) {
    switch (b) {
    case SDL_GAMEPAD_BUTTON_SOUTH: return B_CROSS;
    case SDL_GAMEPAD_BUTTON_EAST: return B_CIRCLE;
    case SDL_GAMEPAD_BUTTON_WEST: return B_SQUARE;
    case SDL_GAMEPAD_BUTTON_NORTH: return B_TRIANGLE;
    case SDL_GAMEPAD_BUTTON_BACK: return B_TOUCH_PAD;
    case SDL_GAMEPAD_BUTTON_START: return B_OPTIONS;
    case SDL_GAMEPAD_BUTTON_LEFT_STICK: return B_L3;
    case SDL_GAMEPAD_BUTTON_RIGHT_STICK: return B_R3;
    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: return B_L1;
    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: return B_R1;
    case SDL_GAMEPAD_BUTTON_DPAD_UP: return B_UP;
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN: return B_DOWN;
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT: return B_LEFT;
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: return B_RIGHT;
    case SDL_GAMEPAD_BUTTON_TOUCHPAD: return B_TOUCH_PAD;
    default: return 0;
    }
}

bool SDL_GetGamepadButton(SDL_Gamepad *g, SDL_GamepadButton b) {
    return SDL_GamepadConnected(g) && (g->last.buttons & button_bit(b)) != 0;
}

static Sint16 stick(uint8_t v) {   // 0..255, 128 at rest -> SDL's -32768..32767
    int s = ((int)v - 128) * 256;
    return (Sint16)(s > 32767 ? 32767 : s);
}

Sint16 SDL_GetGamepadAxis(SDL_Gamepad *g, SDL_GamepadAxis a) {
    if (!SDL_GamepadConnected(g)) return 0;
    switch (a) {
    case SDL_GAMEPAD_AXIS_LEFTX: return stick(g->last.left_x);
    case SDL_GAMEPAD_AXIS_LEFTY: return stick(g->last.left_y);   // down-positive, as SDL's
    case SDL_GAMEPAD_AXIS_RIGHTX: return stick(g->last.right_x);
    case SDL_GAMEPAD_AXIS_RIGHTY: return stick(g->last.right_y);
    case SDL_GAMEPAD_AXIS_LEFT_TRIGGER: return (Sint16)(g->last.l2 * 32767 / 255);
    case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER: return (Sint16)(g->last.r2 * 32767 / 255);
    default: return 0;
    }
}

bool SDL_RumbleGamepad(SDL_Gamepad *g, Uint16 low, Uint16 high, Uint32 ms) {
    if (!SDL_GamepadConnected(g)) return fail("gamepad not connected");
    const uint8_t motors[2] = {(uint8_t)(low >> 8), (uint8_t)(high >> 8)};   // the large, then the small motor
    if (scePadSetVibration(g->handle, motors) < 0) return fail("scePadSetVibration failed");
    g->rumble_until_ns = (low || high) && ms ? plat_now_ns() + (uint64_t)ms * 1000000u : 0;
    return true;
}

bool SDL_RumbleGamepadTriggers(SDL_Gamepad *g, Uint16 l, Uint16 r, Uint32 ms) {
    (void)g, (void)l, (void)r, (void)ms;
    return fail("trigger rumble is not implemented on the PS5");
}

// Motion (the sample's reserved bytes hold it) is not read yet: no sensors, input.c then logs motion as off.
bool SDL_GamepadHasSensor(SDL_Gamepad *g, SDL_SensorType t) {
    (void)g, (void)t;
    return false;
}
bool SDL_SetGamepadSensorEnabled(SDL_Gamepad *g, SDL_SensorType t, bool on) {
    (void)g, (void)t, (void)on;
    return fail("no gamepad sensors on the PS5 yet");
}
float SDL_GetGamepadSensorDataRate(SDL_Gamepad *g, SDL_SensorType t) {
    (void)g, (void)t;
    return 0;
}
bool SDL_GetGamepadSensorData(SDL_Gamepad *g, SDL_SensorType t, float *data, int n) {
    (void)g, (void)t, (void)data, (void)n;
    return fail("no gamepad sensors on the PS5 yet");
}

SDL_JoystickID SDL_AttachVirtualJoystick(const SDL_VirtualJoystickDesc *desc) {
    (void)desc;
    fail("virtual gamepads are not available on the PS5");
    return 0;
}

// ---- init: the gamepad here; audio in the audio part
static bool audio_init(void);
static void audio_quit(void);

bool SDL_InitSubSystem(SDL_InitFlags flags) {
    if ((flags & SDL_INIT_GAMEPAD) && !gamepad_init()) return false;
    if ((flags & SDL_INIT_AUDIO) && !audio_init()) return false;
    return true;
}

bool SDL_Init(SDL_InitFlags flags) { return SDL_InitSubSystem(flags); }

void SDL_QuitSubSystem(SDL_InitFlags flags) {
    if (flags & SDL_INIT_AUDIO) audio_quit();
}

// ---- audio: one playback stream on the system user's main port, 48 kHz 16-bit stereo in 256-frame grains, fed by a
// thread that sceAudioOutOutput paces (the template's audio_main/audio_start/audio_stop, platform.c :379-438). The
// stream's callback is called for one grain of float frames at a time; they are converted to 16 bits there.
int sceAudioOutInit(void);
int sceAudioOutOpen(int32_t user, int32_t type, int32_t index, uint32_t grain, uint32_t rate, uint32_t param);
int sceAudioOutOutput(int32_t port, const void *samples);
int sceAudioOutClose(int32_t port);

#define GRAIN 256
#define RATE 48000
#define ALREADY_INITIALISED ((int)0x8026000e)
#define DEVICE 1   // the one device's id

struct SDL_AudioStream {
    int32_t port;
    int ch;
    SDL_AudioStreamCallback cb;
    void *user;
    float pending[GRAIN * 2];     // what the callback put for the grain being filled
    int have;                     // floats in pending
    PlatThread *thread;
    volatile int running, playing;
    unsigned errors;
};

static int g_audio_refs;

static bool audio_init(void) {
    if (g_audio_refs++) return true;
    int r = sceAudioOutInit();
    if (r < 0 && r != ALREADY_INITIALISED) {
        g_audio_refs = 0;
        snprintf(g_error, sizeof g_error, "sceAudioOutInit %#x", (unsigned)r);
        return false;
    }
    return true;
}

static void audio_quit(void) {
    if (g_audio_refs > 0) g_audio_refs--;
}

bool SDL_GetAudioDeviceFormat(SDL_AudioDeviceID devid, SDL_AudioSpec *spec, int *sample_frames) {
    (void)devid;
    if (spec) *spec = (SDL_AudioSpec){SDL_AUDIO_F32, 2, RATE};   // stereo until 5.1 is sent as 8 channels (Step 5)
    if (sample_frames) *sample_frames = GRAIN;
    return true;
}

static void audio_main(void *arg) {
    SDL_AudioStream *s = arg;
    static int16_t grain[GRAIN * 2] __attribute__((aligned(64)));
    while (s->running) {
        s->have = 0;
        if (s->playing) s->cb(s->user, s, GRAIN * s->ch * (int)sizeof(float), GRAIN * s->ch * (int)sizeof(float));
        for (int i = 0; i < GRAIN * 2; i++) {
            float v = i < s->have ? s->pending[i] : 0.0f;   // short or paused: silence
            v = v > 1.0f ? 1.0f : v < -1.0f ? -1.0f : v;
            grain[i] = (int16_t)(v * 32767.0f);
        }
        if (sceAudioOutOutput(s->port, grain) < 0) {   // blocks for one grain: this paces the thread
            if (s->errors++ == 0) rt_log("audio: ps5: sceAudioOutOutput failed");
            plat_sleep_ms(5);
        }
    }
    (void)sceAudioOutOutput(s->port, NULL);   // drain the grain still queued
}

SDL_AudioStream *SDL_OpenAudioDeviceStream(SDL_AudioDeviceID devid, const SDL_AudioSpec *spec,
                                           SDL_AudioStreamCallback callback, void *userdata) {
    (void)devid;
    if (!spec || spec->format != SDL_AUDIO_F32 || spec->channels != 2 || spec->freq != RATE || !callback) {
        snprintf(g_error, sizeof g_error, "only 48 kHz float stereo with a callback on the PS5 (asked %d Hz, %d ch)",
                 spec ? spec->freq : 0, spec ? spec->channels : 0);
        return NULL;
    }
    SDL_AudioStream *s = calloc(1, sizeof *s);
    if (!s) return NULL;
    s->ch = 2, s->cb = callback, s->user = userdata;
    s->port = sceAudioOutOpen(0xff, 0, 0, GRAIN, RATE, 1);   // the system user's main port, 16-bit stereo
    if (s->port < 0) {
        snprintf(g_error, sizeof g_error, "sceAudioOutOpen %#x", (unsigned)s->port);
        free(s);
        return NULL;
    }
    s->running = 1;
    s->thread = plat_thread_new(audio_main, s, "audio out");
    if (!s->thread) {
        sceAudioOutClose(s->port);
        free(s);
        fail("no audio thread");
        return NULL;
    }
    rt_log("audio: ps5: AudioOut port %d, 48 kHz 16-bit stereo, %d-frame grains", (int)s->port, GRAIN);
    return s;
}

bool SDL_ResumeAudioStreamDevice(SDL_AudioStream *s) {   // a stream on a device starts paused, as SDL's
    if (!s) return fail("no stream");
    s->playing = 1;
    return true;
}

// Called from the callback, on the audio thread: the floats join the grain being filled; the rest is dropped (the
// callback is only ever asked for one grain).
bool SDL_PutAudioStreamData(SDL_AudioStream *s, const void *buf, int len) {
    if (!s || len < 0) return fail("bad stream data");
    int n = len / (int)sizeof(float), room = GRAIN * 2 - s->have;
    if (n > room) n = room;
    memcpy(s->pending + s->have, buf, (size_t)n * sizeof(float));
    s->have += n;
    return true;
}

SDL_AudioDeviceID SDL_GetAudioStreamDevice(SDL_AudioStream *s) { return s ? DEVICE : 0; }
const char *SDL_GetAudioDeviceName(SDL_AudioDeviceID devid) { return devid ? "PS5 AudioOut (main)" : NULL; }

void SDL_DestroyAudioStream(SDL_AudioStream *s) {
    if (!s) return;
    s->running = 0;
    plat_thread_join(s->thread);
    sceAudioOutClose(s->port);
    free(s);
}

// ---- display: the console's one display at the size the PS5 window reports, 60 Hz (VideoOut's mode on this console)
SDL_DisplayID SDL_GetPrimaryDisplay(void) { return 1; }

const SDL_DisplayMode *SDL_GetCurrentDisplayMode(SDL_DisplayID displayID) {
    static SDL_DisplayMode m;
    if (displayID != 1) return NULL;
    int w = 0, h = 0;
    wsys_client_size(&w, &h);
    m = (SDL_DisplayMode){.displayID = 1, .w = w, .h = h, .pixel_density = 1.0f, .refresh_rate = 60.0f,
                          .refresh_rate_numerator = 60, .refresh_rate_denominator = 1};
    return &m;
}
