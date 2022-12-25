#pragma once

// system includes
#include <array>
#include <atomic>
#include <memory>
#include <optional>

// Arduino includes
#ifdef FEATURE_SERIAL
#include <HardwareSerial.h>
#endif
#include <SPI.h>

// esp-idf includes
#include <esp_chip_info.h>
#include <esp32/pm.h>

// 3rdparty lib includes
#include <espchrono.h>

// local includes
#include "controller.h"
#include "display.h"
#include "modeinterface.h"
#include "newsettings.h"
#include "profilesettings.h"
#include "settingspersister.h"

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

extern std::optional<int> sunrise;
extern std::optional<int> sunset;
extern std::optional<espchrono::DateTime> sunrise_dt;

extern esp_chip_info_t chip_info;
extern esp_pm_config_esp32_t pm_config;

extern float avgSpeed;
extern float avgSpeedKmh;
extern float lastAvgSpeedKmh;
extern float avgAccel;
extern float sumCurrent;

extern espchrono::millis_clock::time_point lastAvgSpeedKmhTs;

extern bool isLocked;

#ifdef GLOBALS_PLUGIN
#include GLOBALS_PLUGIN
GLOBALS_HEADER
#endif

extern bool simplified;

extern ProfileSettings profileSettings;
extern SettingsPersister settingsPersister;

extern std::atomic<int8_t> rawButtonRequest;
extern std::atomic<int8_t> buttonRequest;
extern bool initScreenRequest;

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

    std::optional<float> getAvgVoltage() const
    {
        uint8_t voltages{0};
        float avgVoltage{0.f};
        for (auto &controller : *this)
        {
            if (const auto result = controller.getCalibratedVoltage(); !std::isnan(result) && controller.feedbackValid)
            {
                avgVoltage += result;
                voltages++;
            }
        }
        if (!voltages)
            return std::nullopt;
        return avgVoltage / voltages;
    }
};

extern Controllers controllers;
struct FrontControllerGetter { static Controller &get() { return controllers.front; }};
struct BackControllerGetter { static Controller &get() { return controllers.back; }};

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
