#pragma once

#include <cstdint>

#include <esp_wifi_types.h>

#include "bobbycar-common.h"

#ifdef FEATURE_BLUETOOTH
#include "bluetoothmode.h"
#endif
#include "unifiedmodelmode.h"

namespace {
enum class LarsmModeMode : uint8_t { Mode1, Mode2, Mode3, Mode4 };

struct Settings
{
#ifdef FEATURE_BMS
    bool autoConnectBms;
#endif

    bool reverseBeep;
    uint8_t reverseBeepFreq0;
    uint8_t reverseBeepFreq1;
    int16_t reverseBeepDuration0;
    int16_t reverseBeepDuration1;

    struct Limits {
        int16_t iMotMax;      // [A] Maximum motor current limit
        int16_t iDcMax;       // [A] Maximum DC Link current limit (This is the current protection. Above this value, current chopping is applied. To avoid this make sure that I_DC_MAX = I_MOT_MAX + 2A)
        int16_t nMotMax;      // [rpm] Maximum motor speed limit
        int16_t fieldWeakMax; // [A] Maximum Field Weakening D axis current (only for FOC). Higher current results in higher maximum speed.
        int16_t phaseAdvMax;  // [deg] Maximum Phase Advance angle (only for SIN). Higher angle results in higher maximum speed.
    } limits;

    struct WifiSettings {
        wifi_mode_t autoWifiMode;
        bool autoEnableAp;
    } wifiSettings;

#ifdef FEATURE_BLUETOOTH
    struct BluetoothSettings {
        BluetoothMode autoBluetoothMode;
    } bluetoothSettings;
#endif

    struct ControllerHardware {
        bool enableFrontLeft, enableFrontRight, enableBackLeft, enableBackRight;
        bool invertFrontLeft, invertFrontRight, invertBackLeft, invertBackRight;

        int16_t wheelDiameter; // in mm
        int16_t numMagnetPoles; // virtual RPM per one real RPM
        bool swapFrontBack;
    } controllerHardware;

    struct BoardcomputerHardware {
        int16_t sampleCount;
        int16_t gasMin, gasMax, bremsMin, bremsMax;
#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW)
        uint8_t dpadDebounce;
#endif
#ifdef FEATURE_GAMETRAK
        int16_t gametrakXMin, gametrakXMax, gametrakYMin, gametrakYMax, gametrakDistMin, gametrakDistMax;
#endif
        bool swapScreenBytes;

        struct TimersSettings {
            int16_t potiReadRate;
            int16_t modeUpdateRate;
            int16_t statsUpdateRate;
            int16_t displayUpdateRate;
            int16_t displayRedrawRate;
        } timersSettings;
    } boardcomputerHardware;

    struct DefaultMode {
        UnifiedModelMode modelMode;
        bool squareGas;
        bool squareBrems;
        bool enableSmoothing;
        int16_t smoothing;
        int16_t frontPercentage;
        int16_t backPercentage;
        int16_t add_schwelle;
        int16_t gas1_wert;
        int16_t gas2_wert;
        int16_t brems1_wert;
        int16_t brems2_wert;
    } defaultMode;

    struct TempomatMode {
        UnifiedModelMode modelMode;
    } tempomatMode;

    struct LarsmMode {
        UnifiedModelMode modelMode;
        LarsmModeMode mode;
        uint8_t iterations;
    } larsmMode;


    template<typename T>
    void executeForEverySetting(T &&callable);
};

template<typename T>
void Settings::executeForEverySetting(T &&callable)
{
#ifdef FEATURE_BMS
    callable("autoConnectBms", autoConnectBms);
#endif

    callable("reverseBeep", reverseBeep);
    callable("revBeepFreq0", reverseBeepFreq0);
    callable("revBeepFreq1", reverseBeepFreq1);
    callable("revBeepDur0", reverseBeepDuration0);
    callable("revBeepDur1", reverseBeepDuration1);

    callable("iMotMax", limits.iMotMax);
    callable("iDcMax", limits.iDcMax);
    callable("nMotMax", limits.nMotMax);
    callable("fieldWeakMax", limits.fieldWeakMax);
    callable("phaseAdvMax", limits.phaseAdvMax);

#ifdef FEATURE_BLUETOOTH
    callable("autoBluetoothMo", bluetoothSettings.autoBluetoothMode);
#endif

    callable("autoWifiMode", wifiSettings.autoWifiMode);
    callable("autoEnableAp", wifiSettings.autoEnableAp);

    callable("enableFrontLeft", controllerHardware.enableFrontLeft);
    callable("enableFrontRigh", controllerHardware.enableFrontRight);
    callable("enableBackLeft", controllerHardware.enableBackLeft);
    callable("enableBackRight", controllerHardware.enableBackRight);

    callable("invertFrontLeft", controllerHardware.invertFrontLeft);
    callable("invertFrontRigh", controllerHardware.invertFrontRight);
    callable("invertBackLeft", controllerHardware.invertBackLeft);
    callable("invertBackRight", controllerHardware.invertBackRight);

    callable("wheelDiameter", controllerHardware.wheelDiameter);
    callable("numMagnetPoles", controllerHardware.numMagnetPoles);
    callable("swapFrontBack", controllerHardware.swapFrontBack);

    callable("sampleCount", boardcomputerHardware.sampleCount);
    callable("gasMin", boardcomputerHardware.gasMin);
    callable("gasMax", boardcomputerHardware.gasMax);
    callable("bremsMin", boardcomputerHardware.bremsMin);
    callable("bremsMax", boardcomputerHardware.bremsMax);
#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW)
    callable("dpadDebounce", boardcomputerHardware.dpadDebounce);
#endif
#ifdef FEATURE_GAMETRAK
    callable("gametrakXMin", boardcomputerHardware.gametrakXMin);
    callable("gametrakXMax", boardcomputerHardware.gametrakXMax);
    callable("gametrakYMin", boardcomputerHardware.gametrakYMin);
    callable("gametrakYMax", boardcomputerHardware.gametrakYMax);
    callable("gametrakDistMin", boardcomputerHardware.gametrakDistMin);
    callable("gametrakDistMax", boardcomputerHardware.gametrakDistMax);
#endif
    callable("swapScreenBytes", boardcomputerHardware.swapScreenBytes);
    callable("potiReadRate", boardcomputerHardware.timersSettings.potiReadRate);
    callable("modeUpdateRate", boardcomputerHardware.timersSettings.modeUpdateRate);
    callable("statsUpdateRate", boardcomputerHardware.timersSettings.statsUpdateRate);
    callable("displayUpdateRa", boardcomputerHardware.timersSettings.displayUpdateRate);
    callable("displayRedrawRa", boardcomputerHardware.timersSettings.displayRedrawRate);

    callable("default.modelMo", defaultMode.modelMode);
    callable("default.enableS", defaultMode.enableSmoothing);
    callable("default.smoothi", defaultMode.smoothing);
    callable("default.frontPe", defaultMode.frontPercentage);
    callable("default.backPer", defaultMode.backPercentage);
    callable("default.add_sch", defaultMode.add_schwelle);
    callable("default.gas1_we", defaultMode.gas1_wert);
    callable("default.gas2_we", defaultMode.gas2_wert);
    callable("default.brems1_", defaultMode.brems1_wert);
    callable("default.brems2_", defaultMode.brems2_wert);

    callable("tempoma.modelMo", tempomatMode.modelMode);

    callable("larsm.modelMode", larsmMode.modelMode);
    callable("larsm.mode", larsmMode.mode);
    callable("larsm.iters", larsmMode.iterations);
}
}
