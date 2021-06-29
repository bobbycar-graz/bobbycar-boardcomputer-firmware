#pragma once

#include "settings.h"

namespace presets {
constexpr Settings::Limits defaultLimits {
    .iMotMax = DEFAULT_IMOTMAX,
    .iDcMax = DEFAULT_IDCMAX,
    .nMotMax = DEFAULT_NMOTMAX,
    .fieldWeakMax = DEFAULT_FIELDWEAKMAX,
    .phaseAdvMax = DEFAULT_FIELDADVMAX
};

constexpr Settings::Limits kidsLimits {
    .iMotMax = 5,
    .iDcMax = 7,
    .nMotMax = 500,
    .fieldWeakMax = 2,
    .phaseAdvMax = 20
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
    .swapFrontBack = false
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
    .swapFrontBack = false
};

constexpr Settings::WifiSettings defaultWifiSettings {
    .autoWifiMode = WIFI_MODE_APSTA,
    .autoEnableAp = true
};

#ifdef FEATURE_BLUETOOTH
constexpr Settings::BluetoothSettings defaultBluetoothSettings {
    .autoBluetoothMode = BluetoothMode::Master
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
    .swapFrontBack = false
};

constexpr Settings::BoardcomputerHardware::TimersSettings defaultTimersSettings {
    .potiReadRate = 50,
    .modeUpdateRate = 50,
    .statsUpdateRate = 50,
    .displayUpdateRate = 50,
    .displayRedrawRate = 50
};

constexpr Settings::BoardcomputerHardware defaultBoardcomputerHardware {
    .sampleCount = 100,
    .gasMin = DEFAULT_GASMIN,
    .gasMax = DEFAULT_GASMAX,
    .bremsMin = DEFAULT_BREMSMIN,
    .bremsMax = DEFAULT_BREMSMAX,
#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW)
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
    .swapScreenBytes = DEFAULT_SWAPSCREENBYTES,
    .timersSettings = defaultTimersSettings
};

constexpr Settings::DefaultMode defaultDefaultMode {
    .modelMode = UnifiedModelMode::FocTorque,
    .squareGas = true,
    .squareBrems = true,
    .enableSmoothing = true,
    .smoothing = 20,
    .frontPercentage = 100,
    .backPercentage = 100,
    .add_schwelle = 750,
    .gas1_wert = 1250,
    .gas2_wert = 1250,
    .brems1_wert = 250,
    .brems2_wert = 750
};

constexpr Settings::DefaultMode sinusoidalDefaultMode {
    .modelMode = UnifiedModelMode::Sinusoidal,
    .squareGas = true,
    .squareBrems = true,
    .enableSmoothing = true,
    .smoothing = 20,
    .frontPercentage = 100,
    .backPercentage = 100,
    .add_schwelle = 500,
    .gas1_wert = 1250,
    .gas2_wert = 1250,
    .brems1_wert = 150,
    .brems2_wert = 1000
};

constexpr Settings::TempomatMode defaultTempomatMode {
    .modelMode = UnifiedModelMode::FocSpeed
};

constexpr Settings::LarsmMode defaultLarsmMode {
    .modelMode = UnifiedModelMode::Commutation,
    .mode = LarsmModeMode::Mode4,
    .iterations = 100
};

constexpr Settings defaultSettings{
#ifdef FEATURE_BMS
    .autoConnectBms = false,
#endif
    .reverseBeep = false,
    .reverseBeepFreq0 = 3,
    .reverseBeepFreq1 = 0,
    .reverseBeepDuration0 = 500,
    .reverseBeepDuration1 = 500,
    .limits = defaultLimits,
    .wifiSettings = defaultWifiSettings,
#ifdef FEATURE_BLUETOOTH
    .bluetoothSettings = defaultBluetoothSettings,
#endif
    .controllerHardware = defaultControllerHardware,
    .boardcomputerHardware = defaultBoardcomputerHardware,
    .defaultMode = defaultDefaultMode,
    .tempomatMode = defaultTempomatMode,
    .larsmMode = defaultLarsmMode
};
}
