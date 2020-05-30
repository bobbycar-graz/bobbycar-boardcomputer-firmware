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
  #ifdef GLUMP_CONTROLLER
    .enableFrontLeft = true,
    .enableFrontRight = true,
    .enableBackLeft = true,
    .enableBackRight = true,

    .invertFrontLeft = false,
    .invertFrontRight = true,
    .invertBackLeft = false,
    .invertBackRight = true,

    .swapFrontBack = false,
  #endif

#ifdef VESC_CONTROLLER
    .enableOne = true,
    .enableTwo = true,

    .invertOne = true,
    .invertTwo = true,
#endif

    .wheelDiameter = 165,
    .numMagnetPoles = 15,
};

constexpr Settings::ControllerHardware mosfetsOffControllerHardware {
  #ifdef GLUMP_CONTROLLER
    .enableFrontLeft = false,
    .enableFrontRight = false,
    .enableBackLeft = false,
    .enableBackRight = false,

    .invertFrontLeft = false,
    .invertFrontRight = true,
    .invertBackLeft = false,
    .invertBackRight = true,

    .swapFrontBack = false,
  #endif

#ifdef VESC_CONTROLLER
    .enableOne = false,
    .enableTwo = false,

    .invertOne = false,
    .invertTwo = false,
#endif

    .wheelDiameter = 165,
    .numMagnetPoles = 15,
};

#ifdef GLUMP_CONTROLLER
constexpr Settings::ControllerHardware spinnerControllerHardware {
    .enableFrontLeft = true,
    .enableFrontRight = true,
    .enableBackLeft = true,
    .enableBackRight = true,

    .invertFrontLeft = false,
    .invertFrontRight = false,
    .invertBackLeft = false,
    .invertBackRight = false,

    .swapFrontBack = false,

    .wheelDiameter = 165,
    .numMagnetPoles = 15,
};
#endif

constexpr Settings::BoardcomputerHardware defaultBoardcomputerHardware {
    .sampleCount = 100,
    .gasMin = DEFAULT_GASMIN,
    .gasMax = DEFAULT_GASMAX,
    .bremsMin = DEFAULT_BREMSMIN,
    .bremsMax = DEFAULT_BREMSMAX,
#ifdef FEATURE_GAMETRAK
    .gametrakXMin = DEFAULT_GAMETRAKXMIN,
    .gametrakXMax = DEFAULT_GAMETRAKXMAX,
    .gametrakYMin = DEFAULT_GAMETRAKYMIN,
    .gametrakYMax = DEFAULT_GAMETRAKYMAX,
    .gametrakDistMin = DEFAULT_GAMETRAKDISTMIN,
    .gametrakDistMax = DEFAULT_GAMETRAKDISTMAX,
#endif
    .swapScreenBytes = DEFAULT_SWAPSCREENBYTES
};

constexpr Settings::DefaultMode defaultDefaultMode {
#ifdef GLUMP_CONTROLLER
    .ctrlTyp = ControlType::FieldOrientedControl,
    .ctrlMod = ControlMode::Torque,
#endif
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

#ifdef GLUMP_CONTROLLER
constexpr Settings::TempomatMode defaultTempomatMode {
    .ctrlTyp = ControlType::FieldOrientedControl,
    .ctrlMod = ControlMode::Speed
};

constexpr Settings::LarsmMode defaultLarsmMode {
    .mode = LarsmModeMode::Mode4,
    .iterations = 100
};
#endif

constexpr Settings defaultSettings{
#ifdef FEATURE_BMS
    .autoConnectBms = false,
#endif
    .bluetoothMode = BluetoothMode::Off,
    .reverseBeep = false,
    .reverseBeepFreq0 = 3,
    .reverseBeepFreq1 = 0,
    .reverseBeepDuration0 = 500,
    .reverseBeepDuration1 = 500,
    .limits = defaultLimits,
    .controllerHardware = defaultControllerHardware,
    .boardcomputerHardware = defaultBoardcomputerHardware,
    .defaultMode = defaultDefaultMode,

#ifdef GLUMP_CONTROLLER
    .tempomatMode = defaultTempomatMode,
    .larsmMode = defaultLarsmMode
#endif
};
}
