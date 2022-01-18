#pragma once

// system includes
#include <cstdint>

// local includes
#include "unifiedmodelmode.h"
#include "modes/larsmmode.h"

struct ProfileSettings
{
    struct Limits {
        int16_t iMotMax;      // [A] Maximum motor current limit
        int16_t iDcMax;       // [A] Maximum DC Link current limit (This is the current protection. Above this value, current chopping is applied. To avoid this make sure that I_DC_MAX = I_MOT_MAX + 2A)
        int16_t nMotMax;      // [rpm] Maximum motor speed limit
        int16_t fieldWeakMax; // [A] Maximum Field Weakening D axis current (only for FOC). Higher current results in higher maximum speed.
        int16_t phaseAdvMax;  // [deg] Maximum Phase Advance angle (only for SIN). Higher angle results in higher maximum speed.
    } limits;

    struct ControllerHardware {
        bool enableFrontLeft, enableFrontRight, enableBackLeft, enableBackRight;
        bool invertFrontLeft, invertFrontRight, invertBackLeft, invertBackRight;
    } controllerHardware;

    struct DefaultMode {
        UnifiedModelMode modelMode;
        bool squareGas;
        bool squareBrems;
        bool enableSmoothingUp;
        bool enableSmoothingDown;
        bool enableFieldWeakSmoothingUp;
        bool enableFieldWeakSmoothingDown;
        int16_t smoothing;
        int16_t frontPercentage;
        int16_t backPercentage;
        int16_t add_schwelle;
        int16_t gas1_wert;
        int16_t gas2_wert;
        int16_t brems1_wert;
        int16_t brems2_wert;
        int16_t fwSmoothLowerLimit;
    } defaultMode;

    struct TempomatMode {
        UnifiedModelMode modelMode;
    } tempomatMode;

    struct LarsmMode {
        UnifiedModelMode modelMode;
        LarsmModeMode mode;
        uint8_t iterations;
    } larsmMode;

    struct MotortestMode {
        uint8_t multiplikator;
        uint16_t maxPwm;
    } motortestMode;

#ifdef FEATURE_JOYSTICK
    struct WheelchairMode {
        uint16_t sensitivity0Kmh;
        uint16_t sensitivity50Kmh;
    } wheelchairMode;
#endif

    template<typename T>
    void executeForEveryProfileSetting(T &&callable);
};

template<typename T>
void ProfileSettings::executeForEveryProfileSetting(T &&callable)
{
    callable("iMotMax", limits.iMotMax);
    callable("iDcMax", limits.iDcMax);
    callable("nMotMax", limits.nMotMax);
    callable("fieldWeakMax", limits.fieldWeakMax);
    callable("phaseAdvMax", limits.phaseAdvMax);

    callable("enableFrontLeft", controllerHardware.enableFrontLeft);
    callable("enableFrontRigh", controllerHardware.enableFrontRight);
    callable("enableBackLeft", controllerHardware.enableBackLeft);
    callable("enableBackRight", controllerHardware.enableBackRight);

    callable("invertFrontLeft", controllerHardware.invertFrontLeft);
    callable("invertFrontRigh", controllerHardware.invertFrontRight);
    callable("invertBackLeft", controllerHardware.invertBackLeft);
    callable("invertBackRight", controllerHardware.invertBackRight);

    callable("default.modelMo", defaultMode.modelMode);
    callable("default.enSmUp_", defaultMode.enableSmoothingUp);
    callable("default.enSmDow", defaultMode.enableSmoothingDown);
    callable("default.enSmFUp", defaultMode.enableFieldWeakSmoothingUp);
    callable("default.enSmFDo", defaultMode.enableFieldWeakSmoothingDown);
    callable("default.fwSmLLi", defaultMode.fwSmoothLowerLimit);
    callable("default.smoothi", defaultMode.smoothing);
    callable("default.frontPe", defaultMode.frontPercentage);
    callable("default.backPer", defaultMode.backPercentage);
    callable("default.add_sch", defaultMode.add_schwelle);
    callable("default.gas1_we", defaultMode.gas1_wert);
    callable("default.gas2_we", defaultMode.gas2_wert);
    callable("default.brems1_", defaultMode.brems1_wert);
    callable("default.brems2_", defaultMode.brems2_wert);
    callable("default.squareG", defaultMode.squareGas);
    callable("default.squareB", defaultMode.squareBrems);

    callable("tempoma.modelMo", tempomatMode.modelMode);

    callable("larsm.modelMode", larsmMode.modelMode);
    callable("larsm.mode", larsmMode.mode);
    callable("larsm.iters", larsmMode.iterations);

#ifdef FEATURE_JOYSTICK
    callable("wc.ses0", wheelchairMode.sensitivity0Kmh);
    callable("wc.ses50", wheelchairMode.sensitivity50Kmh);
#endif
}
