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
#include "profilesettings.h"
#include "newsettings.h"
#include "settingspersister.h"
#include "macros_bobbycar.h"

extern std::optional<int16_t> raw_gas, raw_brems;
extern std::optional<float> gas, brems;

#ifdef FEATURE_GAMETRAK
extern int16_t raw_gametrakX;
extern int16_t raw_gametrakY;
extern int16_t raw_gametrakDist;
extern float gametrakX;
extern float gametrakY;
extern float gametrakDist;
#endif

extern float avgSpeed;
extern float avgSpeedKmh;
extern float sumCurrent;

extern bool isLocked;

#ifdef GLOBALS_PLUGIN
#include GLOBALS_PLUGIN
#endif

extern bool simplified;

extern ProfileSettings profileSettings;
extern SettingsPersister settingsPersister;

extern std::array<CRGB, 8> ledstrip_custom_colors;

class Controllers : public std::array<Controller, 2>
{
public:
    explicit Controllers() :
        std::array<Controller, 2>{{
            Controller {
#ifdef FEATURE_SERIAL
                Serial1,
#endif
                profileSettings.controllerHardware.enableFrontLeft, profileSettings.controllerHardware.enableFrontRight, profileSettings.controllerHardware.invertFrontLeft, profileSettings.controllerHardware.invertFrontRight,
                configs.battery.front30VoltCalibration, configs.battery.front50VoltCalibration
            },
            Controller {
#ifdef FEATURE_SERIAL
                Serial2,
#endif
                profileSettings.controllerHardware.enableBackLeft, profileSettings.controllerHardware.enableBackRight, profileSettings.controllerHardware.invertBackLeft, profileSettings.controllerHardware.invertBackRight,
                configs.battery.back30VoltCalibration, configs.battery.back50VoltCalibration
            }
        }}
    {}

    Controllers(const Controllers &) = delete;
    Controllers &operator=(const Controllers &) = delete;

    Controller &front{operator[](0)};
    Controller &back{operator[](1)};
};

extern Controllers controllers;
struct FrontControllerGetter { static Controller &get() { return controllers.front; }};
struct BackControllerGetter { static Controller &get() { return controllers.back; }};

#ifdef FEATURE_BLUETOOTH
extern BluetoothSerial bluetoothSerial;
#endif

extern ModeInterface *lastMode;
extern ModeInterface *currentMode;

#ifdef FEATURE_LEDBACKLIGHT
constexpr const bool ledBacklightInverted =
#ifdef LEDBACKLIGHT_INVERTED
        true
#else
        false
#endif
;
#endif
