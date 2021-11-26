#pragma once

// 3rdparty lib includes
#include <accessorinterface.h>

// local includes
#include "globals.h"
#include "utils.h"

//! Special type of RefAccessor that also saves settings after setValue()
template<typename T>
struct RefAccessorSaveSettings : public virtual espgui::RefAccessor<T>
{
    void setValue(T value) override { espgui::RefAccessor<T>::setValue(value); saveSettings(); };
};

#ifdef FEATURE_BMS
struct AutoConnectBmsAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.autoConnectBms; } };
#endif
struct ReverseBeepAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.buzzer.reverseBeep; } };
struct ReverseBeepFreq0Accessor : public RefAccessorSaveSettings<uint8_t> { uint8_t &getRef() const override { return settings.buzzer.reverseBeepFreq0; } };
struct ReverseBeepFreq1Accessor : public RefAccessorSaveSettings<uint8_t> { uint8_t &getRef() const override { return settings.buzzer.reverseBeepFreq1; } };
struct ReverseBeepDuration0Accessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.buzzer.reverseBeepDuration0; } };
struct ReverseBeepDuration1Accessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.buzzer.reverseBeepDuration1; } };

struct IMotMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.limits.iMotMax; } };
struct IDcMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.limits.iDcMax; } };
struct NMotMaxKmhAccessor : public virtual espgui::AccessorInterface<int16_t>
{
    int16_t getValue() const override { return convertToKmh(settings.limits.nMotMax); }
    void setValue(int16_t value) override { settings.limits.nMotMax = convertFromKmh(value); saveSettings(); }
};
struct NMotMaxRpmAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.limits.nMotMax; } };
struct FieldWeakMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.limits.fieldWeakMax; } };
struct PhaseAdvMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.limits.phaseAdvMax; } };

struct WifiStaEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.wifiSettings.wifiStaEnabled; } };
struct WifiApEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.wifiSettings.wifiApEnabled; } };

#ifdef FEATURE_BLUETOOTH
struct AutoBluetoothModeAccessor : public RefAccessorSaveSettings<BluetoothMode> { BluetoothMode &getRef() const override { return settings.bluetoothSettings.autoBluetoothMode; } };
#endif

#ifdef FEATURE_BLE
struct BleEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.bleSettings.bleEnabled; } };
#endif

#ifdef FEATURE_CLOUD
struct CloudEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.cloudSettings.cloudEnabled; } };
struct CloudTransmitTimeoutAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.cloudSettings.cloudTransmitTimeout; } };
#endif

struct TimezoneOffsetAccessor : public virtual espgui::AccessorInterface<int32_t>
{
    int32_t getValue() const override { return settings.timeSettings.timezoneOffset.count(); }
    void setValue(int32_t value) override { settings.timeSettings.timezoneOffset = espchrono::minutes32{value}; saveSettings(); }
};
struct DaylightSavingModeAccessor : public RefAccessorSaveSettings<espchrono::DayLightSavingMode> { espchrono::DayLightSavingMode &getRef() const override { return settings.timeSettings.daylightSavingMode; } };
#ifdef FEATURE_NTP
struct TimeServerEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.timeSettings.timeServerEnabled; } };
struct TimeSyncModeAccessor : public RefAccessorSaveSettings<sntp_sync_mode_t> { sntp_sync_mode_t &getRef() const override { return settings.timeSettings.timeSyncMode; } };
struct TimeSyncIntervalAccessor : public virtual espgui::AccessorInterface<int32_t>
{
    int32_t getValue() const override { return settings.timeSettings.timeSyncInterval.count(); }
    void setValue(int32_t value) override { settings.timeSettings.timeSyncInterval = espchrono::milliseconds32{value}; saveSettings(); }
};
#endif

struct FrontLeftEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.enableFrontLeft; } };
struct FrontRightEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.enableFrontRight; } };
struct BackLeftEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.enableBackLeft; } };
struct BackRightEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.enableBackRight; } };

struct FrontLeftInvertedAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.invertFrontLeft; } };
struct FrontRightInvertedAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.invertFrontRight; } };
struct BackLeftInvertedAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.invertBackLeft; } };
struct BackRightInvertedAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.invertBackRight; } };

struct WheelDiameterMmAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.controllerHardware.wheelDiameter; } };
struct WheelDiameterInchAccessor : public virtual espgui::AccessorInterface<float>
{
    float getValue() const override { return convertToInch(settings.controllerHardware.wheelDiameter); }
    void setValue(float value) override { settings.controllerHardware.wheelDiameter = convertFromInch(value); saveSettings(); }
};
struct NumMagnetPolesAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.controllerHardware.numMagnetPoles; } };
struct SwapFrontBackAccessor : public RefAccessorSaveSettings<bool> {
    bool &getRef() const override { return settings.controllerHardware.swapFrontBack; }
#ifdef FEATURE_SERIAL
    void setValue(bool value) override { RefAccessorSaveSettings<bool>::setValue(value); updateSwapFrontBack(); };
#endif
};
#ifdef FEATURE_CAN
struct SendFrontCanCmdAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.sendFrontCanCmd; } };
struct SendBackCanCmdAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.controllerHardware.sendBackCanCmd; } };
struct CanTransmitTimeoutAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.controllerHardware.canTransmitTimeout; } };
struct CanReceiveTimeoutAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.controllerHardware.canReceiveTimeout; } };
#endif

struct SampleCountAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.sampleCount; } };
struct GasMinAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gasMin; } };
struct GasMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gasMax; } };
struct BremsMinAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.bremsMin; } };
struct BremsMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.bremsMax; } };
#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW) || defined(FEATURE_DPAD_5WIRESW_2OUT) || defined (FEATURE_DPAD_6WIRESW)
struct DPadDebounceAccessor : public RefAccessorSaveSettings<uint8_t> { uint8_t &getRef() const override { return settings.boardcomputerHardware.dpadDebounce; } };
#endif
#ifdef FEATURE_GAMETRAK
struct GametrakXMinAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gametrakXMin; } };
struct GametrakXMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gametrakXMax; } };
struct GametrakYMinAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gametrakYMin; } };
struct GametrakYMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gametrakYMax; } };
struct GametrakDistMinAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gametrakDistMin; } };
struct GametrakDistMaxAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.gametrakDistMax; } };
#endif
struct PotiReadRateAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.timersSettings.potiReadRate; } };
struct ModeUpdateRateAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.timersSettings.modeUpdateRate; } };
struct StatsUpdateRateAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.timersSettings.statsUpdateRate; } };
struct DisplayUpdateRateAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.timersSettings.displayUpdateRate; } };
struct DisplayRedrawRateAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.timersSettings.displayRedrawRate; } };
#ifdef FEATURE_CAN
struct CanReceiveRateAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.timersSettings.canReceiveRate; } };
#endif
#ifdef FEATURE_CLOUD
struct CloudCollectRateAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.timersSettings.cloudCollectRate; } };
struct CloudSendRateAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.timersSettings.cloudSendRate; } };
struct UdpCloudSendIntervalAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.boardcomputerHardware.timersSettings.udpSendRateMs; } };
struct UdpCloudEnabledAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.cloudSettings.udpCloudEnabled; } };
struct CloudDebugEnableAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.cloudSettings.enableCloudDebug; } };
struct UdpUseStdStringAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.cloudSettings.udpUseStdString; } };
#endif

struct DefaultModeModelModeAccessor : public RefAccessorSaveSettings<UnifiedModelMode> { UnifiedModelMode &getRef() const override { return settings.defaultMode.modelMode; } };
struct DefaultModeHybridModelModeAccessor : public RefAccessorSaveSettings<UnifiedModelMode> { UnifiedModelMode &getRef() const override { return settings.hybrid.hybridMode; } };
struct DefaultModeSquareGasAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.defaultMode.squareGas; } };
struct DefaultModeSquareBremsAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.defaultMode.squareBrems; } };

struct DefaultModeEnableSmoothingUpAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.defaultMode.enableSmoothingUp; } };
struct DefaultModeEnableSmoothingDownAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.defaultMode.enableSmoothingDown; } };
struct DefaultModeEnableFieldWeakSmoothingUpAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.defaultMode.enableFieldWeakSmoothingUp; } };
struct DefaultModeEnableFieldWeakSmoothingDownAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.defaultMode.enableFieldWeakSmoothingDown; } };
struct DefaultModeEnableFieldWeakSmoothingLowerLimitAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.fwSmoothLowerLimit; } };

struct DefaultModeSmoothingAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.smoothing; } };
struct DefaultModeFrontPercentageAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.frontPercentage; } };
struct DefaultModeBackPercentageAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.backPercentage; } };
struct DefaultModeAddSchwelleAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.add_schwelle; } };
struct DefaultModeGas1WertAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.gas1_wert; } };
struct DefaultModeGas2WertAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.gas2_wert; } };
struct DefaultModeBrems1WertAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.brems1_wert; } };
struct DefaultModeBrems2WertAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.defaultMode.brems2_wert; } };

struct DefaultModeEnableHybridAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.hybrid.enable; } };
struct DefaultModeHybridActivationLimitAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.hybrid.activationLimit; } };
struct DefaultModeHybridDeactivationLimitAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.hybrid.deactivationLimit; } };

struct TempomatModeModelModeAccessor : public RefAccessorSaveSettings<UnifiedModelMode> { UnifiedModelMode &getRef() const override { return settings.tempomatMode.modelMode; } };

struct LarsmModeModelModeAccessor : public RefAccessorSaveSettings<UnifiedModelMode> { UnifiedModelMode &getRef() const override { return settings.larsmMode.modelMode; } };
struct LarsmModeModeAccessor : public RefAccessorSaveSettings<LarsmModeMode> { LarsmModeMode &getRef() const override { return settings.larsmMode.mode; } };
struct LarsmModeIterationsAccessor : public RefAccessorSaveSettings<uint8_t> { uint8_t &getRef() const override { return settings.larsmMode.iterations; } };

struct MotortestModeMultiplikatorAccessor : public RefAccessorSaveSettings<uint8_t> { uint8_t &getRef() const override { return settings.motortestMode.multiplikator; } };
struct MotortestMaxPwmAccessor : public RefAccessorSaveSettings<uint16_t> { uint16_t &getRef() const override { return settings.motortestMode.maxPwm; } };

#ifdef FEATURE_LEDSTRIP
struct EnableLedAnimationAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.ledstrip.enableLedAnimation; } };
struct EnableBrakeLightsAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.ledstrip.enableBrakeLights; } };
struct LedsCountAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.ledstrip.ledsCount; } };
struct CenterOffsetAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.ledstrip.centerOffset; } };
struct SmallOffsetAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.ledstrip.smallOffset; } };
struct BigOffsetAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.ledstrip.bigOffset; } };
struct DeziampereAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.ledstrip.deziampere; } };
struct EnableBeepWhenBlinkAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.ledstrip.enableBeepWhenBlink; } };
struct EnableFullBlinkAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.ledstrip.enableFullBlink; } };
struct EnableLedstripStVOAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.ledstrip.enableStVO; } };
struct LedsStVOFrontOffsetAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.ledstrip.stvoFrontOffset; } };
struct LedsStVOFrontLengthAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.ledstrip.stvoFrontLength; } };
struct EnableLedstripStVOFrontlight : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.ledstrip.stvoFrontEnable; } };
struct AnimationMultiplierAccessor : public RefAccessorSaveSettings<int16_t> { int16_t &getRef() const override { return settings.ledstrip.animationMultiplier; } };
struct LedstripBrightnessAccessor : public RefAccessorSaveSettings<uint8_t> { uint8_t &getRef() const override { return settings.ledstrip.brightness; } };
struct LedstripEnableBlinkAnimationAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.ledstrip.enableAnimBlink; } };
struct LedstripOtaAnimationAccessor : public RefAccessorSaveSettings<OtaAnimationModes> { OtaAnimationModes &getRef() const override { return settings.ledstrip.otaMode; } };
#endif

// Battery
struct BatterySeriesCellsAccessor : public RefAccessorSaveSettings<uint8_t> { uint8_t &getRef() const override { return settings.battery.cellsSeries; } };
struct BatteryParallelCellsAccessor : public RefAccessorSaveSettings<uint8_t> { uint8_t &getRef() const override { return settings.battery.cellsParallel; } };
struct BatteryWHperKMAccessor : public RefAccessorSaveSettings<uint16_t> { uint16_t &getRef() const override { return settings.battery.watthoursPerKilometer; } };
struct BatteryApplyCalibrationAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.battery.applyCalibration; } };


struct LockscreenAllowPresetSwitchAccessor : public RefAccessorSaveSettings<bool> { bool &getRef() const override { return settings.lockscreen.allowPresetSwitch; } };
template<uint8_t index>
struct LockscreenPinDigitAccessor : public RefAccessorSaveSettings<int8_t> { int8_t &getRef() const override { return settings.lockscreen.pin[index]; } };
