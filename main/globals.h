#pragma once

// system includes
#include <array>
#include <memory>
#include <optional>

// Arduino includes
#ifdef FEATURE_BLUETOOTH
#include <BluetoothSerial.h>
#endif
#ifdef FEATURE_SERIAL
#include <HardwareSerial.h>
#endif
#include <SPI.h>

// 3rdparty lib includes
#include <espchrono.h>
#include <TFT_eSPI.h>

// local includes
#include "controller.h"
#include "display.h"
#include "modeinterface.h"
#include "settings.h"
#include "stringsettings.h"
#include "settingspersister.h"
#include "macros_bobbycar.h"

using namespace espgui;

namespace {
std::optional<int16_t> raw_gas, raw_brems;
std::optional<float> gas, brems;

#ifdef FEATURE_GAMETRAK
int16_t raw_gametrakX, raw_gametrakY, raw_gametrakDist;
float gametrakX, gametrakY, gametrakDist;
#endif
float avgSpeed, avgSpeedKmh, sumCurrent;

char deviceName[32] = STRING(DEVICE_PREFIX) "_ERR";

#ifdef GLOBALS_PLUGIN
#include GLOBALS_PLUGIN
#endif

#if defined(HAS_SIMPLIFIED)
bool simplified = true;
#else
bool simplified = false;
#endif

Settings settings;
StringSettings stringSettings;
SettingsPersister settingsPersister;

std::array<CRGB, 8> ledstrip_custom_colors = {};

class Controllers : public std::array<Controller, 2>
{
public:
    explicit Controllers() :
        std::array<Controller, 2>{{
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
        }}
    {}

    Controllers(const Controllers &) = delete;
    Controllers &operator=(const Controllers &) = delete;

    Controller &front{operator[](0)};
    Controller &back{operator[](1)};
};

Controllers controllers;
struct FrontControllerGetter { static Controller &get() { return controllers.front; }};
struct BackControllerGetter { static Controller &get() { return controllers.back; }};

struct {
    espchrono::millis_clock::time_point lastTime;
    int current{};
    int last{};
} performance;

#ifdef FEATURE_BLUETOOTH
BluetoothSerial bluetoothSerial;
#endif

ModeInterface *lastMode{};
ModeInterface *currentMode{};

#ifdef FEATURE_LEDBACKLIGHT
constexpr const bool ledBacklightInverted =
#ifdef LEDBACKLIGHT_INVERTED
        true
#else
        false
#endif
;
#endif
}
