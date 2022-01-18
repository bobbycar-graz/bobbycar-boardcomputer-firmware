#pragma once

// local includes
#include "profilesettings.h"

using namespace std::chrono_literals;

namespace presets {
constexpr ProfileSettings::Limits defaultLimits {
    .iMotMax = DEFAULT_IMOTMAX,           // profileSetting
    .iDcMax = DEFAULT_IDCMAX,             // profileSetting
    .nMotMax = DEFAULT_NMOTMAX,           // profileSetting
    .fieldWeakMax = DEFAULT_FIELDWEAKMAX, // profileSetting
    .phaseAdvMax = DEFAULT_FIELDADVMAX    // profileSetting
};

constexpr ProfileSettings::Limits kidsLimits {
    .iMotMax = 5,
    .iDcMax = 7,
    .nMotMax = 500,
    .fieldWeakMax = 2,
    .phaseAdvMax = 20
};

constexpr ProfileSettings::ControllerHardware defaultControllerHardware {
    .enableFrontLeft = true,  // profileSetting
    .enableFrontRight = true, // profileSetting
    .enableBackLeft = true,   // profileSetting
    .enableBackRight = true,  // profileSetting

    .invertFrontLeft = DEFAULT_INVERTFRONTLEFT,   // profileSetting
    .invertFrontRight = DEFAULT_INVERTFRONTRIGHT, // profileSetting
    .invertBackLeft = DEFAULT_INVERTBACKLEFT,     // profileSetting
    .invertBackRight = DEFAULT_INVERTBACKRIGHT,   // profileSetting
};

constexpr ProfileSettings::ControllerHardware mosfetsOffControllerHardware {
    .enableFrontLeft = false,
    .enableFrontRight = false,
    .enableBackLeft = false,
    .enableBackRight = false,

    .invertFrontLeft = DEFAULT_INVERTFRONTLEFT,
    .invertFrontRight = DEFAULT_INVERTFRONTRIGHT,
    .invertBackLeft = DEFAULT_INVERTBACKLEFT,
    .invertBackRight = DEFAULT_INVERTBACKRIGHT,
};

constexpr ProfileSettings::ControllerHardware spinnerControllerHardware {
    .enableFrontLeft = true,
    .enableFrontRight = true,
    .enableBackLeft = true,
    .enableBackRight = true,

    .invertFrontLeft = DEFAULT_INVERTFRONTLEFT,
    .invertFrontRight = !DEFAULT_INVERTFRONTRIGHT,
    .invertBackLeft = DEFAULT_INVERTBACKLEFT,
    .invertBackRight = !DEFAULT_INVERTBACKRIGHT,
};

constexpr ProfileSettings::DefaultMode defaultDefaultMode {
    .modelMode = UnifiedModelMode::FocTorque, // profileSetting
    .squareGas = true,
    .squareBrems = true,
    .enableSmoothingUp = true,                // profileSetting
    .enableSmoothingDown = true,              // profileSetting
    .enableFieldWeakSmoothingUp = false,      // profileSetting
    .enableFieldWeakSmoothingDown = false,    // profileSetting
    .smoothing = 20,                          // profileSetting
    .frontPercentage = 100,                   // profileSetting
    .backPercentage = 100,                    // profileSetting
    .add_schwelle = 750,                      // profileSetting
    .gas1_wert = 1250,                        // profileSetting
    .gas2_wert = 1250,                        // profileSetting
    .brems1_wert = 250,                       // profileSetting
    .brems2_wert = 750,                       // profileSetting
    .fwSmoothLowerLimit = 800                 // profileSetting
};

constexpr ProfileSettings::DefaultMode sinusoidalDefaultMode {
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

constexpr ProfileSettings::TempomatMode defaultTempomatMode {
    .modelMode = UnifiedModelMode::FocSpeed
};

constexpr ProfileSettings::LarsmMode defaultLarsmMode {
    .modelMode = UnifiedModelMode::Commutation,
    .mode = LarsmModeMode::Mode4,
    .iterations = 100
};

constexpr ProfileSettings::MotortestMode defaultMotortestMode {
    .multiplikator = 2,
    .maxPwm = 400
};

#ifdef FEATURE_JOYSTICK
constexpr ProfileSettings::WheelchairMode defaultWheelChairMode {
    .sensitivity0Kmh = 100,
    .sensitivity50Kmh = 10,
};
#endif

constexpr ProfileSettings defaultProfileSettings {
    .limits = defaultLimits,
    .controllerHardware = defaultControllerHardware,
    .defaultMode = defaultDefaultMode,
    .tempomatMode = defaultTempomatMode,
    .larsmMode = defaultLarsmMode,
    .motortestMode = defaultMotortestMode,
#ifdef FEATURE_JOYSTICK
    .wheelchairMode = defaultWheelChairMode
#endif
};
} // namespace presets
