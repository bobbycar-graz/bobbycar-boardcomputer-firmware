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
float avgSpeed{};
float avgSpeedKmh{};
float sumCurrent{};
bool isLocked{};

char deviceName[32] = STRING(DEVICE_PREFIX) "_ERR";

bool simplified =
#if defined(HAS_SIMPLIFIED)
    true
#else
    false
#endif
;

#ifdef FEATURE_DNS_NS
std::string dns_lastIpAddress_v4 = "";
std::string dns_lastIpAddress_v6 = "";
std::string dns_lastIpAddress_v6_global = "";
#endif

Settings settings;
StringSettings stringSettings;
SettingsPersister settingsPersister;

std::array<CRGB, 8> ledstrip_custom_colors;

Controllers controllers;

#ifdef FEATURE_BLUETOOTH
BluetoothSerial bluetoothSerial;
#endif

ModeInterface *lastMode{};
ModeInterface *currentMode{};
