#pragma once

// esp-idf includes
#ifdef FEATURE_NTP
#include <lwip/apps/snmp.h>
#include <esp_sntp.h>
#endif

// 3rdparty lib includes
#include <espchrono.h>

// local includes
#include "settings.h"
#include "stringsettings.h"
#include "ledstripdefines.h"

using namespace std::chrono_literals;

namespace presets {
constexpr Settings::Limits defaultLimits {
    .iMotMax = DEFAULT_IMOTMAX,
    .iDcMax = DEFAULT_IDCMAX,
    .nMotMax = DEFAULT_NMOTMAX,
    .fieldWeakMax = DEFAULT_FIELDWEAKMAX,
    .phaseAdvMax = DEFAULT_FIELDADVMAX
};

constexpr Settings::Buzzer defaultBuzzer {
    .reverseBeep = false,
    .reverseBeepFreq0 = 3,
    .reverseBeepFreq1 = 0,
    .reverseBeepDuration0 = 500,
    .reverseBeepDuration1 = 500,
};

constexpr Settings::Limits kidsLimits {
    .iMotMax = 5,
    .iDcMax = 7,
    .nMotMax = 500,
    .fieldWeakMax = 2,
    .phaseAdvMax = 20
};

constexpr Settings::TimeSettings defaultTimeSettings {
    .timezoneOffset = 60min,
    .daylightSavingMode = espchrono::DayLightSavingMode::EuropeanSummerTime,
#ifdef FEATURE_NTP
    .timeServerEnabled = true,
    .timeSyncMode = SNTP_SYNC_MODE_IMMED,
    .timeSyncInterval = espchrono::milliseconds32{CONFIG_LWIP_SNTP_UPDATE_DELAY},
#endif
};

constexpr Settings::ControllerHardware defaultControllerHardware {
    .enableFrontLeft = true,
    .enableFrontRight = true,
    .enableBackLeft = true,
    .enableBackRight = true,

    .invertFrontLeft = DEFAULT_INVERTFRONTLEFT,
    .invertFrontRight = DEFAULT_INVERTFRONTRIGHT,
    .invertBackLeft = DEFAULT_INVERTBACKLEFT,
    .invertBackRight = DEFAULT_INVERTBACKRIGHT,

    .wheelDiameter = DEFAULT_WHEELDIAMETER,
    .numMagnetPoles = 15,
    .swapFrontBack = false,
#ifdef FEATURE_CAN
    .sendFrontCanCmd = true,
    .sendBackCanCmd = true,
    .canTransmitTimeout = 200,
    .canReceiveTimeout = 0,
#endif
};

constexpr Settings::ControllerHardware mosfetsOffControllerHardware {
    .enableFrontLeft = false,
    .enableFrontRight = false,
    .enableBackLeft = false,
    .enableBackRight = false,

    .invertFrontLeft = DEFAULT_INVERTFRONTLEFT,
    .invertFrontRight = DEFAULT_INVERTFRONTRIGHT,
    .invertBackLeft = DEFAULT_INVERTBACKLEFT,
    .invertBackRight = DEFAULT_INVERTBACKRIGHT,

    .wheelDiameter = 165,
    .numMagnetPoles = 15,
    .swapFrontBack = false,
#ifdef FEATURE_CAN
    .sendFrontCanCmd = true,
    .sendBackCanCmd = true,
    .canTransmitTimeout = 200,
    .canReceiveTimeout = 0,
#endif
};

constexpr Settings::WifiSettings defaultWifiSettings {
    .wifiEnabled = true
};

#ifdef FEATURE_BLUETOOTH
constexpr Settings::BluetoothSettings defaultBluetoothSettings {
    .autoBluetoothMode = BluetoothMode::Master
};
#endif

#ifdef FEATURE_BLE
constexpr Settings::BleSettings defaultBleSettings {
    .bleEnabled = true
};
#endif

constexpr Settings::ControllerHardware spinnerControllerHardware {
    .enableFrontLeft = true,
    .enableFrontRight = true,
    .enableBackLeft = true,
    .enableBackRight = true,

    .invertFrontLeft = DEFAULT_INVERTFRONTLEFT,
    .invertFrontRight = !DEFAULT_INVERTFRONTRIGHT,
    .invertBackLeft = DEFAULT_INVERTBACKLEFT,
    .invertBackRight = !DEFAULT_INVERTBACKRIGHT,

    .wheelDiameter = 165,
    .numMagnetPoles = 15,
    .swapFrontBack = false,
#ifdef FEATURE_CAN
    .sendFrontCanCmd = true,
    .sendBackCanCmd = true,
    .canTransmitTimeout = 200,
    .canReceiveTimeout = 0,
#endif
};

constexpr Settings::BoardcomputerHardware::TimersSettings defaultTimersSettings {
    .potiReadRate = 50,
    .modeUpdateRate = 50,
    .statsUpdateRate = 50,
    .displayUpdateRate = 50,
    .displayRedrawRate = 50,
#ifdef FEATURE_CAN
    .canReceiveRate = 100,
#endif
#ifdef FEATURE_CLOUD
    .cloudCollectRate = 100,
    .cloudSendRate = 1,
#endif
};

constexpr Settings::BoardcomputerHardware defaultBoardcomputerHardware {
    .sampleCount = 50,
    .gasMin = DEFAULT_GASMIN,
    .gasMax = DEFAULT_GASMAX,
    .bremsMin = DEFAULT_BREMSMIN,
    .bremsMax = DEFAULT_BREMSMAX,
#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW) || defined(FEATURE_DPAD_5WIRESW_2OUT) || defined (FEATURE_DPAD_6WIRESW)
    .dpadDebounce = 25,
#endif
#ifdef FEATURE_GAMETRAK
    .gametrakXMin = DEFAULT_GAMETRAKXMIN,
    .gametrakXMax = DEFAULT_GAMETRAKXMAX,
    .gametrakYMin = DEFAULT_GAMETRAKYMIN,
    .gametrakYMax = DEFAULT_GAMETRAKYMAX,
    .gametrakDistMin = DEFAULT_GAMETRAKDISTMIN,
    .gametrakDistMax = DEFAULT_GAMETRAKDISTMAX,
#endif
    .timersSettings = defaultTimersSettings
};

#ifdef FEATURE_CLOUD
constexpr Settings::CloudSettings defaultCloudSettings {
    .cloudEnabled = false,
    .cloudTransmitTimeout = 10
};
#endif

constexpr Settings::DefaultMode defaultDefaultMode {
    .modelMode = UnifiedModelMode::FocTorque,
    .squareGas = true,
    .squareBrems = true,
    .enableSmoothingUp = true,
    .enableSmoothingDown = true,
    .enableFieldWeakSmoothingUp = false,
    .enableFieldWeakSmoothingDown = false,
    .smoothing = 20,
    .frontPercentage = 100,
    .backPercentage = 100,
    .add_schwelle = 750,
    .gas1_wert = 1250,
    .gas2_wert = 1250,
    .brems1_wert = 250,
    .brems2_wert = 750,
    .fwSmoothLowerLimit = 800
};

constexpr Settings::DefaultMode sinusoidalDefaultMode {
    .modelMode = UnifiedModelMode::Sinusoidal,
    .squareGas = true,
    .squareBrems = true,
    .enableSmoothingUp = true,
    .enableSmoothingDown = true,
    .enableFieldWeakSmoothingUp = false,
    .enableFieldWeakSmoothingDown = false,
    .smoothing = 20,
    .frontPercentage = 100,
    .backPercentage = 100,
    .add_schwelle = 500,
    .gas1_wert = 1250,
    .gas2_wert = 1250,
    .brems1_wert = 150,
    .brems2_wert = 1000,
    .fwSmoothLowerLimit = 800
};

constexpr Settings::TempomatMode defaultTempomatMode {
    .modelMode = UnifiedModelMode::FocSpeed
};

constexpr Settings::LarsmMode defaultLarsmMode {
    .modelMode = UnifiedModelMode::Commutation,
    .mode = LarsmModeMode::Mode4,
    .iterations = 100
};

#ifdef FEATURE_LEDSTRIP
constexpr Settings::Ledstrip defaultLedstrip {
    .enableLedAnimation = true,
    .enableBrakeLights = true,
    .ledsCount = LEDSTRIP_LENGTH,
    .centerOffset = 1,
    .smallOffset = 4,
    .bigOffset = 10,
    .deziampere = 30,
    .enableBeepWhenBlink = false,
#ifdef LEDSTRIP_ANIMATION_DEFAULT
    .animationType = LEDSTRIP_ANIMATION_DEFAULT,
#else
    .animationType = LEDSTRIP_ANIMATION_TYPE_DEFAULTRAINBOW,
#endif
    .enableFullBlink = false,
    .enableStVO = false,
    .stvoFrontOffset = 0,
    .stvoFrontLength = 10,
    .stvoFrontEnable = false,
    .animationMultiplier = 10,
    .brightness = 255
};
#endif

constexpr Settings::LockscreenSettings defaultLockscreen {
    .allowPresetSwitch = true,
    .pin = { 1, 2, 3, 4 }
};

constexpr Settings::Battery defaultBattery {
    .cellsSeries = 12,
    .cellsParallel = 4,
    .cellType = 0,
    .watthoursPerKilometer = 20,
    .front30VoltCalibration = 3000,
    .back30VoltCalibration = 3000,
    .front50VoltCalibration = 5000,
    .back50VoltCalibration = 5000,
    .applyCalibration = true
};

constexpr Settings defaultSettings {
#ifdef FEATURE_BMS
    .autoConnectBms = false,
#endif
    .buzzer = defaultBuzzer,
    .limits = defaultLimits,
    .wifiSettings = defaultWifiSettings,
#ifdef FEATURE_BLUETOOTH
    .bluetoothSettings = defaultBluetoothSettings,
#endif
#ifdef FEATURE_BLE
    .bleSettings = defaultBleSettings,
#endif
    .timeSettings = defaultTimeSettings,
    .controllerHardware = defaultControllerHardware,
    .boardcomputerHardware = defaultBoardcomputerHardware,
#ifdef FEATURE_CLOUD
    .cloudSettings = defaultCloudSettings,
#endif
    .defaultMode = defaultDefaultMode,
    .tempomatMode = defaultTempomatMode,
    .larsmMode = defaultLarsmMode,
#ifdef FEATURE_LEDSTRIP
    .ledstrip = defaultLedstrip,
#endif
    .battery = defaultBattery,
    .lockscreen = defaultLockscreen
};

StringSettings makeDefaultStringSettings()
{
    using ConfiguredWifi = StringSettings::ConfiguredWifi;
#ifdef FEATURE_OTA
    using ConfiguredOtaServer = StringSettings::ConfiguredOtaServer;
#endif

    return {
        .wifis = std::array<ConfiguredWifi, 10> {
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} }
        },
#ifdef FEATURE_CLOUD
        .cloudUrl = {},
#endif
#ifdef FEATURE_OTA
        .otaUrl = {},
#endif
#ifdef FEATURE_GARAGE
        .garageUrl = {},
#endif
#ifdef FEATURE_NTP
        .timeServer = "europe.pool.ntp.org",
#endif
#ifdef FEATURE_OTA
        .otaServers = std::array<ConfiguredOtaServer, 2> {
            ConfiguredOtaServer { .name = {}, .url = {} },
            ConfiguredOtaServer { .name = {}, .url = {} },/*
            ConfiguredOtaServer { .name = {}, .url = {} },
            ConfiguredOtaServer { .name = {}, .url = {} },
            ConfiguredOtaServer { .name = {}, .url = {} },
            ConfiguredOtaServer { .name = {}, .url = {} },
            ConfiguredOtaServer { .name = {}, .url = {} },
            ConfiguredOtaServer { .name = {}, .url = {} },
            ConfiguredOtaServer { .name = {}, .url = {} },
            ConfiguredOtaServer { .name = {}, .url = {} },*/
        },
        .otaServerUrl = {},
#endif
    };
}
}
