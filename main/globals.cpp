#include "globals.h"

std::optional<int16_t> raw_gas, raw_brems;
std::optional<float> gas, brems;

#ifdef FEATURE_GAMETRAK
int16_t raw_gametrakX{};
int16_t raw_gametrakY{};
int16_t raw_gametrakDist{};
float gametrakX;
float gametrakY;
float gametrakDist;
#endif

#ifdef FEATURE_JOYSTICK
bool joystick_button{};
int16_t joystick_x{};
int16_t joystick_y{};
int16_t raw_joystick_x{};
int16_t raw_joystick_y{};
#endif

float avgSpeed{};
float avgSpeedKmh{};
float sumCurrent{};
bool isLocked{};

bool simplified =
#if defined(HAS_SIMPLIFIED)
    true
#else
    false
#endif
;

ProfileSettings profileSettings;
SettingsPersister settingsPersister;

std::array<CRGB, 8> ledstrip_custom_colors;

Controllers controllers;

#ifdef FEATURE_BLUETOOTH
BluetoothSerial bluetoothSerial;
#endif

ModeInterface *lastMode{};
ModeInterface *currentMode{};
