// PS5 build only (tools/build/build.py puts runtime/ps5_sdl first on the include path): the part of SDL3's
// API that input/input.c and audio/audio_mixer.c call, so they build unchanged on the console, where there is no SDL3.
// platform/ps5_sdl.c implements it on scePad and sceAudioOut. Names, signatures and the order of the enumerations
// follow SDL3's public API (SDL_gamepad.h, SDL_joystick.h, SDL_sensor.h, SDL_audio.h, SDL_init.h); everything else of
// SDL3 is absent on purpose: a call outside this subset is a compile error, not a silent stub.
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "SDL_messagebox.h"   // as SDL3's SDL.h includes it (plugin/plugin.c, upstream ed04f94)

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t Uint8;
typedef uint16_t Uint16;
typedef uint32_t Uint32;
typedef uint64_t Uint64;
typedef int16_t Sint16;
typedef int32_t Sint32;

// ---- SDL_init.h, SDL_error.h, SDL_stdinc.h
typedef Uint32 SDL_InitFlags;
#define SDL_INIT_AUDIO 0x00000010u
#define SDL_INIT_GAMEPAD 0x00002000u
bool SDL_Init(SDL_InitFlags flags);
bool SDL_InitSubSystem(SDL_InitFlags flags);
void SDL_QuitSubSystem(SDL_InitFlags flags);
const char *SDL_GetError(void);
void SDL_free(void *mem);
// An interface struct's version is its size (SDL_stdinc.h SDL_INIT_INTERFACE).
#define SDL_INIT_INTERFACE(iface) (memset((iface), 0, sizeof(*(iface))), (iface)->version = (Uint32)sizeof(*(iface)))

// ---- SDL_events.h: switching an event type off, and a user event pushed to wake a loop that waits for input
// (plugin/mod_page_ui.cpp, upstream 96d9eda). The console has no event loop (its menus draw every frame): SDL_PushEvent
// drops the event and returns false, as SDL3 does for one a filter drops.
typedef Uint32 SDL_EventType;
#define SDL_EVENT_GAMEPAD_SENSOR_UPDATE 0x659u
#define SDL_EVENT_USER 0x8000u
void SDL_SetEventEnabled(Uint32 type, bool enabled);
typedef struct SDL_UserEvent {
    Uint32 type, reserved;
    Uint64 timestamp;
    Uint32 windowID;
    Sint32 code;
    void *data1, *data2;
} SDL_UserEvent;
typedef union SDL_Event {
    Uint32 type;
    SDL_UserEvent user;
    Uint8 padding[128];
} SDL_Event;
bool SDL_PushEvent(SDL_Event *event);
#define SDL_zero(x) memset(&(x), 0, sizeof(x))

// ---- SDL_properties.h
typedef Uint32 SDL_PropertiesID;
bool SDL_GetBooleanProperty(SDL_PropertiesID props, const char *name, bool default_value);

// ---- SDL_joystick.h, SDL_sensor.h
typedef Uint32 SDL_JoystickID;
typedef enum SDL_JoystickType { SDL_JOYSTICK_TYPE_UNKNOWN, SDL_JOYSTICK_TYPE_GAMEPAD } SDL_JoystickType;
typedef struct SDL_VirtualJoystickDesc {
    Uint32 version;
    Uint16 type;
    Uint16 naxes, nbuttons;
    const char *name;
} SDL_VirtualJoystickDesc;
SDL_JoystickID SDL_AttachVirtualJoystick(const SDL_VirtualJoystickDesc *desc);
typedef enum SDL_SensorType {
    SDL_SENSOR_INVALID = -1, SDL_SENSOR_UNKNOWN, SDL_SENSOR_ACCEL, SDL_SENSOR_GYRO
} SDL_SensorType;
#define SDL_STANDARD_GRAVITY 9.80665f

// ---- SDL_gamepad.h
typedef struct SDL_Gamepad SDL_Gamepad;
typedef enum SDL_GamepadButton {
    SDL_GAMEPAD_BUTTON_INVALID = -1,
    SDL_GAMEPAD_BUTTON_SOUTH, SDL_GAMEPAD_BUTTON_EAST, SDL_GAMEPAD_BUTTON_WEST, SDL_GAMEPAD_BUTTON_NORTH,
    SDL_GAMEPAD_BUTTON_BACK, SDL_GAMEPAD_BUTTON_GUIDE, SDL_GAMEPAD_BUTTON_START,
    SDL_GAMEPAD_BUTTON_LEFT_STICK, SDL_GAMEPAD_BUTTON_RIGHT_STICK,
    SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER,
    SDL_GAMEPAD_BUTTON_DPAD_UP, SDL_GAMEPAD_BUTTON_DPAD_DOWN, SDL_GAMEPAD_BUTTON_DPAD_LEFT, SDL_GAMEPAD_BUTTON_DPAD_RIGHT,
    SDL_GAMEPAD_BUTTON_MISC1, SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1, SDL_GAMEPAD_BUTTON_LEFT_PADDLE1,
    SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2, SDL_GAMEPAD_BUTTON_LEFT_PADDLE2, SDL_GAMEPAD_BUTTON_TOUCHPAD,
    SDL_GAMEPAD_BUTTON_MISC2, SDL_GAMEPAD_BUTTON_MISC3, SDL_GAMEPAD_BUTTON_MISC4, SDL_GAMEPAD_BUTTON_MISC5,
    SDL_GAMEPAD_BUTTON_MISC6,
    SDL_GAMEPAD_BUTTON_COUNT
} SDL_GamepadButton;
typedef enum SDL_GamepadButtonLabel {
    SDL_GAMEPAD_BUTTON_LABEL_UNKNOWN, SDL_GAMEPAD_BUTTON_LABEL_A, SDL_GAMEPAD_BUTTON_LABEL_B, SDL_GAMEPAD_BUTTON_LABEL_X,
    SDL_GAMEPAD_BUTTON_LABEL_Y, SDL_GAMEPAD_BUTTON_LABEL_CROSS, SDL_GAMEPAD_BUTTON_LABEL_CIRCLE,
    SDL_GAMEPAD_BUTTON_LABEL_SQUARE, SDL_GAMEPAD_BUTTON_LABEL_TRIANGLE
} SDL_GamepadButtonLabel;
typedef enum SDL_GamepadAxis {
    SDL_GAMEPAD_AXIS_INVALID = -1,
    SDL_GAMEPAD_AXIS_LEFTX, SDL_GAMEPAD_AXIS_LEFTY, SDL_GAMEPAD_AXIS_RIGHTX, SDL_GAMEPAD_AXIS_RIGHTY,
    SDL_GAMEPAD_AXIS_LEFT_TRIGGER, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER,
    SDL_GAMEPAD_AXIS_COUNT
} SDL_GamepadAxis;
#define SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN "SDL.joystick.cap.rumble"

SDL_JoystickID *SDL_GetGamepads(int *count);
SDL_Gamepad *SDL_OpenGamepad(SDL_JoystickID instance_id);
void SDL_CloseGamepad(SDL_Gamepad *gamepad);
bool SDL_GamepadConnected(SDL_Gamepad *gamepad);
SDL_JoystickID SDL_GetGamepadID(SDL_Gamepad *gamepad);
const char *SDL_GetGamepadName(SDL_Gamepad *gamepad);
// upstream 3042c59: input.c's Xbox dead zone (the pad's type and vendor) and the battery rows (input_power.c), with
// SDL3's values. The console's pads are DualSense controllers (Sony, 0x054c); their battery is not read yet.
typedef enum SDL_GamepadType {
    SDL_GAMEPAD_TYPE_UNKNOWN = 0, SDL_GAMEPAD_TYPE_STANDARD, SDL_GAMEPAD_TYPE_XBOX360, SDL_GAMEPAD_TYPE_XBOXONE,
    SDL_GAMEPAD_TYPE_PS3, SDL_GAMEPAD_TYPE_PS4, SDL_GAMEPAD_TYPE_PS5
} SDL_GamepadType;
typedef enum SDL_PowerState {
    SDL_POWERSTATE_ERROR = -1, SDL_POWERSTATE_UNKNOWN, SDL_POWERSTATE_ON_BATTERY, SDL_POWERSTATE_NO_BATTERY,
    SDL_POWERSTATE_CHARGING, SDL_POWERSTATE_CHARGED
} SDL_PowerState;
SDL_GamepadType SDL_GetGamepadType(SDL_Gamepad *gamepad);
Uint16 SDL_GetGamepadVendor(SDL_Gamepad *gamepad);
SDL_PowerState SDL_GetGamepadPowerInfo(SDL_Gamepad *gamepad, int *percent);
SDL_PropertiesID SDL_GetGamepadProperties(SDL_Gamepad *gamepad);
SDL_GamepadButtonLabel SDL_GetGamepadButtonLabel(SDL_Gamepad *gamepad, SDL_GamepadButton button);
void SDL_UpdateGamepads(void);
bool SDL_GetGamepadButton(SDL_Gamepad *gamepad, SDL_GamepadButton button);
Sint16 SDL_GetGamepadAxis(SDL_Gamepad *gamepad, SDL_GamepadAxis axis);
bool SDL_RumbleGamepad(SDL_Gamepad *gamepad, Uint16 low_frequency_rumble, Uint16 high_frequency_rumble, Uint32 duration_ms);
bool SDL_RumbleGamepadTriggers(SDL_Gamepad *gamepad, Uint16 left_rumble, Uint16 right_rumble, Uint32 duration_ms);
bool SDL_GamepadHasSensor(SDL_Gamepad *gamepad, SDL_SensorType type);
bool SDL_SetGamepadSensorEnabled(SDL_Gamepad *gamepad, SDL_SensorType type, bool enabled);
float SDL_GetGamepadSensorDataRate(SDL_Gamepad *gamepad, SDL_SensorType type);
bool SDL_GetGamepadSensorData(SDL_Gamepad *gamepad, SDL_SensorType type, float *data, int num_values);

// ---- SDL_audio.h: one playback stream on the default device, pulled through its callback
#define SDLCALL
typedef Uint32 SDL_AudioDeviceID;
#define SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK ((SDL_AudioDeviceID)0xFFFFFFFFu)
typedef enum SDL_AudioFormat { SDL_AUDIO_UNKNOWN = 0, SDL_AUDIO_S16 = 0x8010u, SDL_AUDIO_F32 = 0x8120u } SDL_AudioFormat;
typedef struct SDL_AudioSpec {
    SDL_AudioFormat format;
    int channels;
    int freq;
} SDL_AudioSpec;
typedef struct SDL_AudioStream SDL_AudioStream;
typedef void(SDLCALL *SDL_AudioStreamCallback)(void *userdata, SDL_AudioStream *stream, int additional_amount,
                                               int total_amount);
bool SDL_GetAudioDeviceFormat(SDL_AudioDeviceID devid, SDL_AudioSpec *spec, int *sample_frames);
SDL_AudioStream *SDL_OpenAudioDeviceStream(SDL_AudioDeviceID devid, const SDL_AudioSpec *spec,
                                           SDL_AudioStreamCallback callback, void *userdata);
bool SDL_ResumeAudioStreamDevice(SDL_AudioStream *stream);
bool SDL_PutAudioStreamData(SDL_AudioStream *stream, const void *buf, int len);
SDL_AudioDeviceID SDL_GetAudioStreamDevice(SDL_AudioStream *stream);
const char *SDL_GetAudioDeviceName(SDL_AudioDeviceID devid);
// upstream 3042c59's rate matching (audio_mixer.c): the AudioOut port plays at its own fixed rate; the ratio is taken
// and not applied (ps5_sdl.c), the driver is named "ps5"
bool SDL_SetAudioStreamFrequencyRatio(SDL_AudioStream *stream, float ratio);
const char *SDL_GetCurrentAudioDriver(void);
void SDL_DestroyAudioStream(SDL_AudioStream *stream);

// ---- SDL_video.h (display modes): ui/menu/settings_tabs.cpp asks the primary display's height when the game window
// gives none (upstream 7594caa); the console's one display, the size the PS5 window has (platform/ps5_window.c).
typedef Uint32 SDL_DisplayID;
typedef Uint32 SDL_PixelFormat;
typedef struct SDL_DisplayModeData SDL_DisplayModeData;
typedef struct SDL_DisplayMode {
    SDL_DisplayID displayID;
    SDL_PixelFormat format;
    int w, h;
    float pixel_density, refresh_rate;
    int refresh_rate_numerator, refresh_rate_denominator;
    SDL_DisplayModeData *internal;
} SDL_DisplayMode;
SDL_DisplayID SDL_GetPrimaryDisplay(void);
const SDL_DisplayMode *SDL_GetCurrentDisplayMode(SDL_DisplayID displayID);

#ifdef __cplusplus
}
#endif
