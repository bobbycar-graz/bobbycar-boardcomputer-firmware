#include "globals.h"

std::optional<int16_t> raw_gas, raw_brems;
std::optional<float> gas, brems;

#ifdef FEATURE_GAMETRAK
int16_t raw_gametrakX, raw_gametrakY, raw_gametrakDist;
float gametrakX, gametrakY, gametrakDist;
#endif
float avgSpeed, avgSpeedKmh, sumCurrent;

char deviceName[32] = STRING(DEVICE_PREFIX) "_ERR";

bool simplified =
#if defined(HAS_SIMPLIFIED)
    true
#else
    false
#endif
;

Settings settings;
StringSettings stringSettings;
SettingsPersister settingsPersister;

Controllers::Controllers() :
    std::array<Controller, 2>{
        Controller {
#ifdef FEATURE_SERIAL
            Serial1,
#endif
            settings.controllerHardware.enableFrontLeft, settings.controllerHardware.enableFrontRight, settings.controllerHardware.invertFrontLeft, settings.controllerHardware.invertFrontRight,
            settings.battery.front30VoltCalibration, settings.battery.front50VoltCalibration
        },
        Controller {
#ifdef FEATURE_SERIAL
            Serial2,
#endif
            settings.controllerHardware.enableBackLeft, settings.controllerHardware.enableBackRight, settings.controllerHardware.invertBackLeft, settings.controllerHardware.invertBackRight,
            settings.battery.back30VoltCalibration, settings.battery.back50VoltCalibration
        }
    }
{
}

Controllers controllers;

Performance performance;

#ifdef FEATURE_BLUETOOTH
BluetoothSerial bluetoothSerial;
#endif

ModeInterface *lastMode{};
ModeInterface *currentMode{};
