#pragma once

#include "globals.h"
#include "accessorinterface.h"
#include "utils.h"

namespace {
//! Special type of RefAccessor that also saves settings after setValue()
template<typename T>
struct RefAccessorSaveSettings : public virtual RefAccessor<T>
{
    void setValue(T value) override { RefAccessor<T>::setValue(value); saveSettings(); };
};

#ifdef FEATURE_BMS
struct AutoConnectBmsAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.autoConnectBms; } };
#endif
struct BluetoothModeAccessor : public RefAccessorSaveSettings<BluetoothMode> { BluetoothMode &getRef() const override { return settings.bluetoothMode; } };
struct ReverseBeepAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.reverseBeep; } };
struct ReverseBeepFreq0Accessor : public RefAccessorSaveSettings<uint8_t> { uint8_t &getRef() const override { return settings.reverseBeepFreq0; } };
struct ReverseBeepFreq1Accessor : public RefAccessorSaveSettings<uint8_t> { uint8_t &getRef() const override { return settings.reverseBeepFreq1; } };
struct ReverseBeepDuration0Accessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.reverseBeepDuration0; } };
struct ReverseBeepDuration1Accessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.reverseBeepDuration1; } };

struct IMotMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.limits.iMotMax; } };
struct IDcMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.limits.iDcMax; } };
struct NMotMaxKmhAccessor : public virtual AccessorInterface<int16_t>
{
    int16_t getValue() const override { return convertToKmh(settings.limits.nMotMax); }
    void setValue(int16_t value) override { settings.limits.nMotMax = convertFromKmh(value); saveSettings(); }
};
struct NMotMaxRpmAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.limits.nMotMax; } };
struct FieldWeakMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.limits.fieldWeakMax; } };
struct PhaseAdvMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.limits.phaseAdvMax; } };

#ifdef GLUMP_CONTROLLER
struct FrontLeftEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.enableFrontLeft; } };
struct FrontRightEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.enableFrontRight; } };
struct BackLeftEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.enableBackLeft; } };
struct BackRightEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.enableBackRight; } };

struct FrontLeftInvertedAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.invertFrontLeft; } };
struct FrontRightInvertedAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.invertFrontRight; } };
struct BackLeftInvertedAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.invertBackLeft; } };
struct BackRightInvertedAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.invertBackRight; } };
#endif
#ifdef VESC_CONTROLLER
struct OneEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.enableOne; } };
struct TwoEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.enableTwo; } };

struct OneInvertedAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.invertOne; } };
struct TwoInvertedAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.invertTwo; } };
#endif

struct WheelDiameterMmAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.controllerHardware.wheelDiameter; } };
struct WheelDiameterInchAccessor : public virtual AccessorInterface<float>
{
    float getValue() const override { return convertToInch(settings.controllerHardware.wheelDiameter); }
    void setValue(float value) override { settings.controllerHardware.wheelDiameter = convertFromInch(value); saveSettings(); }
};
struct NumMagnetPolesAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.controllerHardware.numMagnetPoles; } };

#ifdef GLUMP_CONTROLLER
struct SwapFrontBackAccessor : public RefAccessorSaveSettings<bool> {
    bool &getRef() const override { return settings.controllerHardware.swapFrontBack; }
    void setValue(bool value) override { RefAccessorSaveSettings<bool>::setValue(value); updateSwapFrontBack(); };
};
#endif

struct SampleCountAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.sampleCount; } };
struct GasMinAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gasMin; } };
struct GasMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gasMax; } };
struct BremsMinAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.bremsMin; } };
struct BremsMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.bremsMax; } };
#ifdef FEATURE_GAMETRAK
struct GametrakXMinAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gametrakXMin; } };
struct GametrakXMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gametrakXMax; } };
struct GametrakYMinAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gametrakYMin; } };
struct GametrakYMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gametrakYMax; } };
struct GametrakDistMinAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gametrakDistMin; } };
struct GametrakDistMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gametrakDistMax; } };
#endif
struct SwapScreenBytesAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.boardcomputerHardware.swapScreenBytes; } };

#ifdef GLUMP_CONTROLLER
struct DefaultModeCtrlTypAccessor : public RefAccessorSaveSettings<ControlType> { ControlType &getRef() const override { return settings.defaultMode.ctrlTyp; } };
struct DefaultModeCtrlModAccessor : public RefAccessorSaveSettings<ControlMode> { ControlMode &getRef() const override { return settings.defaultMode.ctrlMod; } };
#endif
struct DefaultModeEnableSmoothingAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.defaultMode.enableSmoothing; } };
struct DefaultModeSmoothingAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.smoothing; } };
struct DefaultModeFrontPercentageAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.frontPercentage; } };
struct DefaultModeBackPercentageAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.backPercentage; } };
struct DefaultModeAddSchwelleAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.add_schwelle; } };
struct DefaultModeGas1WertAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.gas1_wert; } };
struct DefaultModeGas2WertAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.gas2_wert; } };
struct DefaultModeBrems1WertAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.brems1_wert; } };
struct DefaultModeBrems2WertAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.brems2_wert; } };

#ifdef GLUMP_CONTROLLER
struct TempomatModeCtrlTypAccessor : public RefAccessorSaveSettings<ControlType> { ControlType &getRef() const override { return settings.tempomatMode.ctrlTyp; } };
struct TempomatModeCtrlModAccessor : public RefAccessorSaveSettings<ControlMode> { ControlMode &getRef() const override { return settings.tempomatMode.ctrlMod; } };

struct LarsmModeModeAccessor : public RefAccessorSaveSettings<LarsmModeMode> { LarsmModeMode &getRef() const override { return settings.larsmMode.mode; } };
struct LarsmModeIterationsAccessor : public RefAccessorSaveSettings<uint8_t> { uint8_t &getRef() const override { return settings.larsmMode.iterations; } };
#endif
}
