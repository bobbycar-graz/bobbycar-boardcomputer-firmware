#pragma once

#include "displays/menus/aboutmenu.h"
#include "displays/menus/accesspointwifisettingsmenu.h"
#include "displays/menus/bluetoothsettingsmenu.h"
#include "displays/menus/bmsmenu.h"
#include "displays/menus/buzzermenu.h"
#include "displays/menus/commanddebugmenu.h"
#include "displays/menus/debugmenu.h"
#include "displays/menus/defaultmodesettingsmenu.h"
#include "displays/menus/demosmenu.h"
#include "displays/menus/dynamicdebugmenu.h"
#include "displays/menus/enablemenu.h"
#include "displays/menus/feedbackdebugmenu.h"
#include "displays/menus/gametrakmodesettingsmenu.h"
#include "displays/menus/genericwifisettingsmenu.h"
#include "displays/menus/graphsmenu.h"
#include "displays/menus/controllerhardwaresettingsmenu.h"
#include "displays/menus/invertmenu.h"
#include "displays/menus/larsmmodesettingsmenu.h"
#include "displays/menus/limitssettingsmenu.h"
#include "displays/menus/mainmenu.h"
#include "displays/menus/tempomatmodesettingsmenu.h"
#include "displays/menus/modessettingsmenu.h"
#include "displays/menus/mosfetsmenu.h"
#include "displays/menus/motorfeedbackdebugmenu.h"
#include "displays/menus/motorstatedebugmenu.h"
#include "displays/menus/presetsmenu.h"
#include "displays/menus/boardcomputerhardwaresettingsmenu.h"
#include "displays/menus/selectmodemenu.h"
#include "displays/menus/settingsmenu.h"
#include "displays/menus/stationwifisettingsmenu.h"
#include "displays/menus/wifiscanmenu.h"
#include "displays/menus/wifisettingsmenu.h"
#include "displays/bmsdisplay.h"
#include "displays/calibratedisplay.h"
#include "displays/dualgraphdisplay.h"
#include "displays/gameoflifedisplay.h"
#include "displays/gametrakcalibratedisplay.h"
#include "displays/lockscreen.h"
#include "displays/metersdisplay.h"
#include "displays/pingpongdisplay.h"
#include "displays/poweroffdisplay.h"
#include "displays/spirodisplay.h"
#include "displays/starfielddisplay.h"
#include "displays/statusdisplay.h"
#include "displays/updatedisplay.h"

#include "globals.h"
#include "utils.h"
#include "icons/logo.h"

namespace {
union X {
    X() {}
    ~X() { ((Display&)statusDisplay).~Display(); }

    AboutMenu aboutMenu;
    AccessPointWifiSettingsMenu accessPointWifiSettingsMenu;
    BluetoothSettingsMenu bluetoothSettingsMenu;
#ifdef FEATURE_BMS
    BmsMenu bmsMenu;
#endif
#ifdef GLUMP_CONTROLLER
    BuzzerMenu buzzerMenu;
    FrontCommandDebugMenu frontCommandDebugMenu;
    BackCommandDebugMenu backCommandDebugMenu;
#endif
    ControllerHardwareSettingsMenu controllerHardwareSettingsMenu;
    DebugMenu debugMenu;
    DefaultModeSettingsMenu defaultModeSettingsMenu;
    DemosMenu demosMenu;
    DynamicDebugMenu dynamicDebugMenu;
    EnableMenu enableMenu;
#ifdef GLUMP_CONTROLLER
    FrontFeedbackDebugMenu frontFeedbackDebugMenu;
    BackFeedbackDebugMenu backFeedbackDebugMenu;
#endif
#ifdef FEATURE_GAMETRAK
    GametrakModeSettingsMenu gametrakModeSettingsMenu;
#endif
    GenericWifiSettingsMenu genericWifiSettingsMenu;
    GraphsMenu graphsMenu;
    InvertMenu invertMenu;

#ifdef GLUMP_CONTROLLER
    LarsmModeSettingsMenu larsmModeSettingsMenu;
#endif

    LimitsSettingsMenu limitsSettingsMenu;
    MainMenu mainMenu;
#ifdef GLUMP_CONTROLLER
    TempomatModeSettingsMenu tempomatModeSettingsMenu;
    ModesSettingsMenu modesSettingsMenu;
#ifdef FEATURE_MOSFETS
    MosfetsMenu mosfetsMenu;
#endif
    FrontLeftMotorStateDebugMenu frontLeftMotorStateDebugMenu;
    FrontRightMotorStateDebugMenu frontRightMotorStateDebugMenu;
    BackLeftMotorStateDebugMenu backLeftMotorStateDebugMenu;
    BackRightMotorStateDebugMenu backRightMotorStateDebugMenu;
    FrontLeftMotorFeedbackDebugMenu frontLeftMotorFeedbackDebugMenu;
    FrontRightMotorFeedbackDebugMenu frontRightMotorFeedbackDebugMenu;
    BackLeftMotorFeedbackDebugMenu backLeftMotorFeedbackDebugMenu;
    BackRightMotorFeedbackDebugMenu backRightMotorFeedbackDebugMenu;
    BoardcomputerHardwareSettingsMenu boardcomputerHardwareSettingsMenu;
#endif
    PresetsMenu presetsMenu;

#ifdef GLUMP_CONTROLLER
    SelectModeMenu selectModeMenu;
#endif

    SettingsMenu settingsMenu;
    StationWifiSettingsMenu stationWifiSettingsMenu;
    WifiScanMenu wifiScanMenu;
    WifiSettingsMenu wifiSettingsMenu;

#ifdef FEATURE_BMS
    BmsDisplay bmsDisplay;
#endif
    CalibrateDisplay calibrateDisplay;
    DualGraphDisplay dualGraphDisplay;
    GameOfLifeDisplay gameOfLifeDisplay;
#ifdef FEATURE_GAMETRAK
    GametrakCalibrateDisplay gametrakCalibrateDisplay;
#endif
    Lockscreen lockScreen;
    MetersDisplay metersDisplay;
    PingPongDisplay pingPongDisplay;
#ifdef GLUMP_CONTROLLER
    PoweroffDisplay poweroffDisplay;
#endif
    SpiroDisplay spiroDisplay;
    StarfieldDisplay starFieldDisplay;
    StatusDisplay statusDisplay;
#ifdef FEATURE_OTA
    UpdateDisplay updateDisplay;
#endif

    BluetoothModeChangeDisplay bluetoothModeChangeDisplay;

#ifdef GLUMP_CONTROLLER
    FrontFreqChangeScreen changeFrontFreq;
    FrontPatternChangeScreen changeFrontPattern;
    BackFreqChangeScreen changeBackFreq;
    BackPatternChangeScreen changeBackPattern;
    ReverseBeepFreq0ChangeScreen changeReverseBeepFreq0;
    ReverseBeepFreq1ChangeScreen changeReverseBeepFreq1;
    ReverseBeepDuration0ChangeScreen changeReverseBeepDuration0;
    ReverseBeepDuration1ChangeScreen changeReverseBeepDuration1;
#endif

    IMotMaxChangeScreen changeIMotMax;
    IDcMaxChangeScreen changeIDcMax;
    NMotMaxKmhChangeScreen changeNMotMaxKmh;
    NMotMaxRpmChangeScreen changeNMotMaxRpm;
    FieldWeakMaxChangeScreen changeFieldWeakMax;
    PhaseAdvMaxChangeScreen changePhaseAdvMax;

    WheelDiameterMmChangeScreen wheelDiameterMmChangeScreen;
    WheelDiameterInchChangeScreen wheelDiameterInchChangeScreen;
    NumMagnetPolesChangeScreen numMagnetPolesChangeScreen;

#ifdef GLUMP_CONTROLLER
    DefaultModeCtrlTypChangeDisplay changeDefaultModeCtrlTyp;
    DefaultModeCtrlModChangeDisplay changeDefaultModeCtrlMod;
#endif
    DefaultModeSmoothingChangeDisplay changeDefaultModeSmoothing;
#ifdef GLUMP_CONTROLLER
    DefaultModeFrontPercentageChangeDisplay changeDefaultModeFrontPercentage;
    DefaultModeBackPercentageChangeDisplay changeDefaultModeBackPercentage;
#endif
    DefaultModeAddSchwelleChangeDisplay changeDefaultModeAddSchwelle;
    DefaultModeGas1WertChangeDisplay changeDefaultModeGas1Wert;
    DefaultModeGas2WertChangeDisplay changeDefaultModeGas2Wert;
    DefaultModeBrems1WertChangeDisplay changeDefaultModeBrems1Wert;
    DefaultModeBrems2WertChangeDisplay changeDefaultModeBrems2Wert;

#ifdef GLUMP_CONTROLLER
    TempomatModeCtrlTypChangeScreen changeManualModeCtrlTyp;
    TempomatModeCtrlModChangeScreen changeManualModeCtrlMod;

    LarsmModeModeChangeDisplay larsmModeModeChangeDisplay;
    LarsmModeIterationsChangeDisplay larsmModeIterationsChangeDisplay;
#endif

    SampleCountChangeScreen sampleCountChangeScreen;
    GasMinChangeScreen changeGasMin;
    GasMaxChangeScreen changeGasMax;
    BremsMinChangeScreen changeBremsMin;
    BremsMaxChangeScreen changeBremsMax;
#ifdef FEATURE_GAMETRAK
    GametrakXMinChangeScreen changeGametrakXMin;
    GametrakXMaxChangeScreen changeGametrakXMax;
    GametrakYMinChangeScreen changeGametrakYMin;
    GametrakYMaxChangeScreen changeGametrakYMax;
    GametrakDistMinChangeScreen changeGametrakDistMin;
    GametrakDistMaxChangeScreen changeGametrakDistMax;
#endif

    WifiModeChangeScreen wifiModeChangeScreen;
    WifiTxPowerChangeScreen wifiTxPowerChangeScreen;

    GasGraphDisplay gasGraphDisplay;
    BremsGraphDisplay bremsGraphDisplay;
    AvgSpeedGraphDisplay avgSpeedGraphDisplay;
    AvgSpeedKmhGraphDisplay avgSpeedKmhGraphDisplay;
    SumCurrentGraphDisplay sumCurrentGraphDisplay;
    SumAbsoluteCurrentGraphDisplay sumAbsoluteCurrentGraphDisplay;
#ifdef GLUMP_CONTROLLER
    FrontVoltageGraphDisplay frontVoltageGraphDisplay;
    BackVoltageGraphDisplay backVoltageGraphDisplay;
#endif
#ifdef FEATURE_BMS
    BmsVoltageGraphDisplay bmsVoltageGraphDisplay;
    BmsCurrentGraphDisplay bmsCurrentGraphDisplay;
    BmsPowerGraphDisplay bmsPowerGraphDisplay;
    SumCurrentsComparisonGraphDisplay sumCurrentsComparisonGraphDisplay;
#endif
#ifdef GLUMP_CONTROLLER
    MotorCurrentsGraphDisplay motorCurrentsGraphDisplay;
#endif
} displays;

template<typename T> T &getRefByType() = delete;
template<> decltype(displays.aboutMenu)                                        &getRefByType<decltype(displays.aboutMenu)>()                                        { return displays.aboutMenu; }
template<> decltype(displays.accessPointWifiSettingsMenu)                      &getRefByType<decltype(displays.accessPointWifiSettingsMenu)>()                      { return displays.accessPointWifiSettingsMenu; }
template<> decltype(displays.bluetoothSettingsMenu)                            &getRefByType<decltype(displays.bluetoothSettingsMenu)>()                            { return displays.bluetoothSettingsMenu; }
#ifdef FEATURE_BMS
template<> decltype(displays.bmsMenu)                                          &getRefByType<decltype(displays.bmsMenu)>()                                          { return displays.bmsMenu; }
#endif
#ifdef GLUMP_CONTROLLER
template<> decltype(displays.buzzerMenu)                                       &getRefByType<decltype(displays.buzzerMenu)>()                                       { return displays.buzzerMenu; }
template<> decltype(displays.boardcomputerHardwareSettingsMenu)                &getRefByType<decltype(displays.boardcomputerHardwareSettingsMenu)>()                { return displays.boardcomputerHardwareSettingsMenu; }
template<> decltype(displays.controllerHardwareSettingsMenu)                   &getRefByType<decltype(displays.controllerHardwareSettingsMenu)>()                   { return displays.controllerHardwareSettingsMenu; }
template<> decltype(displays.frontCommandDebugMenu)                            &getRefByType<decltype(displays.frontCommandDebugMenu)>()                            { return displays.frontCommandDebugMenu; }
template<> decltype(displays.backCommandDebugMenu)                             &getRefByType<decltype(displays.backCommandDebugMenu)>()                             { return displays.backCommandDebugMenu; }
#endif
template<> decltype(displays.debugMenu)                                        &getRefByType<decltype(displays.debugMenu)>()                                        { return displays.debugMenu; }
template<> decltype(displays.defaultModeSettingsMenu)                          &getRefByType<decltype(displays.defaultModeSettingsMenu)>()                          { return displays.defaultModeSettingsMenu; }
template<> decltype(displays.demosMenu)                                        &getRefByType<decltype(displays.demosMenu)>()                                        { return displays.demosMenu; }
template<> decltype(displays.dynamicDebugMenu)                                 &getRefByType<decltype(displays.dynamicDebugMenu)>()                                 { return displays.dynamicDebugMenu; }
template<> decltype(displays.enableMenu)                                       &getRefByType<decltype(displays.enableMenu)>()                                       { return displays.enableMenu; }
#ifdef GLUMP_CONTROLLER
template<> decltype(displays.frontFeedbackDebugMenu)                           &getRefByType<decltype(displays.frontFeedbackDebugMenu)>()                           { return displays.frontFeedbackDebugMenu; }
template<> decltype(displays.backFeedbackDebugMenu)                            &getRefByType<decltype(displays.backFeedbackDebugMenu)>()                            { return displays.backFeedbackDebugMenu; }
#endif

#ifdef FEATURE_GAMETRAK
template<> decltype(displays.gametrakModeSettingsMenu)                         &getRefByType<decltype(displays.gametrakModeSettingsMenu)>()                         { return displays.gametrakModeSettingsMenu; }
#endif
template<> decltype(displays.genericWifiSettingsMenu)                          &getRefByType<decltype(displays.genericWifiSettingsMenu)>()                          { return displays.genericWifiSettingsMenu; }
template<> decltype(displays.graphsMenu)                                       &getRefByType<decltype(displays.graphsMenu)>()                                       { return displays.graphsMenu; }
template<> decltype(displays.invertMenu)                                       &getRefByType<decltype(displays.invertMenu)>()                                       { return displays.invertMenu; }
#ifdef GLUMP_CONTROLLER
template<> decltype(displays.larsmModeSettingsMenu)                            &getRefByType<decltype(displays.larsmModeSettingsMenu)>()                            { return displays.larsmModeSettingsMenu; }
#endif
template<> decltype(displays.limitsSettingsMenu)                               &getRefByType<decltype(displays.limitsSettingsMenu)>()                               { return displays.limitsSettingsMenu; }
template<> decltype(displays.mainMenu)                                         &getRefByType<decltype(displays.mainMenu)>()                                         { return displays.mainMenu; }
#ifdef GLUMP_CONTROLLER
template<> decltype(displays.tempomatModeSettingsMenu)                         &getRefByType<decltype(displays.tempomatModeSettingsMenu)>()                         { return displays.tempomatModeSettingsMenu; }
template<> decltype(displays.modesSettingsMenu)                                &getRefByType<decltype(displays.modesSettingsMenu)>()                                { return displays.modesSettingsMenu; }
#ifdef FEATURE_MOSFETS
template<> decltype(displays.mosfetsMenu)                                      &getRefByType<decltype(displays.mosfetsMenu)>()                                      { return displays.mosfetsMenu; }
#endif
template<> decltype(displays.frontLeftMotorStateDebugMenu)                     &getRefByType<decltype(displays.frontLeftMotorStateDebugMenu)>()                     { return displays.frontLeftMotorStateDebugMenu; }
template<> decltype(displays.frontRightMotorStateDebugMenu)                    &getRefByType<decltype(displays.frontRightMotorStateDebugMenu)>()                    { return displays.frontRightMotorStateDebugMenu; }
template<> decltype(displays.backLeftMotorStateDebugMenu)                      &getRefByType<decltype(displays.backLeftMotorStateDebugMenu)>()                      { return displays.backLeftMotorStateDebugMenu; }
template<> decltype(displays.backRightMotorStateDebugMenu)                     &getRefByType<decltype(displays.backRightMotorStateDebugMenu)>()                     { return displays.backRightMotorStateDebugMenu; }
template<> decltype(displays.frontLeftMotorFeedbackDebugMenu)                  &getRefByType<decltype(displays.frontLeftMotorFeedbackDebugMenu)>()                  { return displays.frontLeftMotorFeedbackDebugMenu; }
template<> decltype(displays.frontRightMotorFeedbackDebugMenu)                 &getRefByType<decltype(displays.frontRightMotorFeedbackDebugMenu)>()                 { return displays.frontRightMotorFeedbackDebugMenu; }
template<> decltype(displays.backLeftMotorFeedbackDebugMenu)                   &getRefByType<decltype(displays.backLeftMotorFeedbackDebugMenu)>()                   { return displays.backLeftMotorFeedbackDebugMenu; }
template<> decltype(displays.backRightMotorFeedbackDebugMenu)                  &getRefByType<decltype(displays.backRightMotorFeedbackDebugMenu)>()                  { return displays.backRightMotorFeedbackDebugMenu; }
#endif
template<> decltype(displays.presetsMenu)                                      &getRefByType<decltype(displays.presetsMenu)>()                                      { return displays.presetsMenu; }
#ifdef GLUMP_CONTROLLER
template<> decltype(displays.selectModeMenu)                                   &getRefByType<decltype(displays.selectModeMenu)>()                                   { return displays.selectModeMenu; }
#endif
template<> decltype(displays.settingsMenu)                                     &getRefByType<decltype(displays.settingsMenu)>()                                     { return displays.settingsMenu; }
template<> decltype(displays.stationWifiSettingsMenu)                          &getRefByType<decltype(displays.stationWifiSettingsMenu)>()                          { return displays.stationWifiSettingsMenu; }
template<> decltype(displays.wifiScanMenu)                                     &getRefByType<decltype(displays.wifiScanMenu)>()                                     { return displays.wifiScanMenu; }
template<> decltype(displays.wifiSettingsMenu)                                 &getRefByType<decltype(displays.wifiSettingsMenu)>()                                 { return displays.wifiSettingsMenu; }

#ifdef FEATURE_BMS
template<> decltype(displays.bmsDisplay)                                       &getRefByType<decltype(displays.bmsDisplay)>()                                       { return displays.bmsDisplay; }
#endif
template<> decltype(displays.calibrateDisplay)                                 &getRefByType<decltype(displays.calibrateDisplay)>()                                 { return displays.calibrateDisplay; }
template<> decltype(displays.dualGraphDisplay)                                 &getRefByType<decltype(displays.dualGraphDisplay)>()                                 { return displays.dualGraphDisplay; }
template<> decltype(displays.gameOfLifeDisplay)                                &getRefByType<decltype(displays.gameOfLifeDisplay)>()                                { return displays.gameOfLifeDisplay; }
#ifdef FEATURE_GAMETRAK
template<> decltype(displays.gametrakCalibrateDisplay)                         &getRefByType<decltype(displays.gametrakCalibrateDisplay)>()                         { return displays.gametrakCalibrateDisplay; }
#endif
template<> decltype(displays.lockScreen)                                       &getRefByType<decltype(displays.lockScreen)>()                                       { return displays.lockScreen; }
template<> decltype(displays.metersDisplay)                                    &getRefByType<decltype(displays.metersDisplay)>()                                    { return displays.metersDisplay; }
template<> decltype(displays.pingPongDisplay)                                  &getRefByType<decltype(displays.pingPongDisplay)>()                                  { return displays.pingPongDisplay; }
#ifdef GLUMP_CONTROLLER
template<> decltype(displays.poweroffDisplay)                                  &getRefByType<decltype(displays.poweroffDisplay)>()                                  { return displays.poweroffDisplay; }
#endif
template<> decltype(displays.spiroDisplay)                                     &getRefByType<decltype(displays.spiroDisplay)>()                                     { return displays.spiroDisplay; }
template<> decltype(displays.starFieldDisplay)                                 &getRefByType<decltype(displays.starFieldDisplay)>()                                 { return displays.starFieldDisplay; }
template<> decltype(displays.statusDisplay)                                    &getRefByType<decltype(displays.statusDisplay)>()                                    { return displays.statusDisplay; }
#ifdef FEATURE_OTA
template<> decltype(displays.updateDisplay)                                    &getRefByType<decltype(displays.updateDisplay)>()                                    { return displays.updateDisplay; }
#endif

template<> decltype(displays.bluetoothModeChangeDisplay)                       &getRefByType<decltype(displays.bluetoothModeChangeDisplay)>()                       { return displays.bluetoothModeChangeDisplay; }

#ifdef GLUMP_CONTROLLER
template<> decltype(displays.changeFrontFreq)                                  &getRefByType<decltype(displays.changeFrontFreq)>()                                  { return displays.changeFrontFreq; }
template<> decltype(displays.changeFrontPattern)                               &getRefByType<decltype(displays.changeFrontPattern)>()                               { return displays.changeFrontPattern; }
template<> decltype(displays.changeBackFreq)                                   &getRefByType<decltype(displays.changeBackFreq)>()                                   { return displays.changeBackFreq; }
template<> decltype(displays.changeBackPattern)                                &getRefByType<decltype(displays.changeBackPattern)>()                                { return displays.changeBackPattern; }
template<> decltype(displays.changeReverseBeepFreq0)                           &getRefByType<decltype(displays.changeReverseBeepFreq0)>()                           { return displays.changeReverseBeepFreq0; }
template<> decltype(displays.changeReverseBeepFreq1)                           &getRefByType<decltype(displays.changeReverseBeepFreq1)>()                           { return displays.changeReverseBeepFreq1; }
template<> decltype(displays.changeReverseBeepDuration0)                       &getRefByType<decltype(displays.changeReverseBeepDuration0)>()                       { return displays.changeReverseBeepDuration0; }
template<> decltype(displays.changeReverseBeepDuration1)                       &getRefByType<decltype(displays.changeReverseBeepDuration1)>()                       { return displays.changeReverseBeepDuration1; }
#endif

template<> decltype(displays.changeIMotMax)                                    &getRefByType<decltype(displays.changeIMotMax)>()                                    { return displays.changeIMotMax; }
template<> decltype(displays.changeIDcMax)                                     &getRefByType<decltype(displays.changeIDcMax)>()                                     { return displays.changeIDcMax; }
template<> decltype(displays.changeNMotMaxKmh)                                 &getRefByType<decltype(displays.changeNMotMaxKmh)>()                                 { return displays.changeNMotMaxKmh; }
template<> decltype(displays.changeNMotMaxRpm)                                 &getRefByType<decltype(displays.changeNMotMaxRpm)>()                                 { return displays.changeNMotMaxRpm; }
template<> decltype(displays.changeFieldWeakMax)                               &getRefByType<decltype(displays.changeFieldWeakMax)>()                               { return displays.changeFieldWeakMax; }
template<> decltype(displays.changePhaseAdvMax)                                &getRefByType<decltype(displays.changePhaseAdvMax)>()                                { return displays.changePhaseAdvMax; }

template<> decltype(displays.wheelDiameterMmChangeScreen)                      &getRefByType<decltype(displays.wheelDiameterMmChangeScreen)>()                      { return displays.wheelDiameterMmChangeScreen; }
template<> decltype(displays.wheelDiameterInchChangeScreen)                    &getRefByType<decltype(displays.wheelDiameterInchChangeScreen)>()                    { return displays.wheelDiameterInchChangeScreen; }
template<> decltype(displays.numMagnetPolesChangeScreen)                       &getRefByType<decltype(displays.numMagnetPolesChangeScreen)>()                       { return displays.numMagnetPolesChangeScreen; }

#ifdef GLUMP_CONTROLLER
template<> decltype(displays.changeDefaultModeCtrlTyp)                         &getRefByType<decltype(displays.changeDefaultModeCtrlTyp)>()                         { return displays.changeDefaultModeCtrlTyp; }
template<> decltype(displays.changeDefaultModeCtrlMod)                         &getRefByType<decltype(displays.changeDefaultModeCtrlMod)>()                         { return displays.changeDefaultModeCtrlMod; }
template<> decltype(displays.changeDefaultModeSmoothing)                       &getRefByType<decltype(displays.changeDefaultModeSmoothing)>()                       { return displays.changeDefaultModeSmoothing; }
template<> decltype(displays.changeDefaultModeFrontPercentage)                 &getRefByType<decltype(displays.changeDefaultModeFrontPercentage)>()                 { return displays.changeDefaultModeFrontPercentage; }
template<> decltype(displays.changeDefaultModeBackPercentage)                  &getRefByType<decltype(displays.changeDefaultModeBackPercentage)>()                  { return displays.changeDefaultModeBackPercentage; }
template<> decltype(displays.changeDefaultModeAddSchwelle)                     &getRefByType<decltype(displays.changeDefaultModeAddSchwelle)>()                     { return displays.changeDefaultModeAddSchwelle; }
template<> decltype(displays.changeDefaultModeGas1Wert)                        &getRefByType<decltype(displays.changeDefaultModeGas1Wert)>()                        { return displays.changeDefaultModeGas1Wert; }
template<> decltype(displays.changeDefaultModeGas2Wert)                        &getRefByType<decltype(displays.changeDefaultModeGas2Wert)>()                        { return displays.changeDefaultModeGas2Wert; }
template<> decltype(displays.changeDefaultModeBrems1Wert)                      &getRefByType<decltype(displays.changeDefaultModeBrems1Wert)>()                      { return displays.changeDefaultModeBrems1Wert; }
template<> decltype(displays.changeDefaultModeBrems2Wert)                      &getRefByType<decltype(displays.changeDefaultModeBrems2Wert)>()                      { return displays.changeDefaultModeBrems2Wert; }

template<> decltype(displays.changeManualModeCtrlTyp)                          &getRefByType<decltype(displays.changeManualModeCtrlTyp)>()                          { return displays.changeManualModeCtrlTyp; }
template<> decltype(displays.changeManualModeCtrlMod)                          &getRefByType<decltype(displays.changeManualModeCtrlMod)>()                          { return displays.changeManualModeCtrlMod; }

template<> decltype(displays.larsmModeModeChangeDisplay)                       &getRefByType<decltype(displays.larsmModeModeChangeDisplay)>()                       { return displays.larsmModeModeChangeDisplay; }
template<> decltype(displays.larsmModeIterationsChangeDisplay)                 &getRefByType<decltype(displays.larsmModeIterationsChangeDisplay)>()                 { return displays.larsmModeIterationsChangeDisplay; }
#endif
template<> decltype(displays.sampleCountChangeScreen)                          &getRefByType<decltype(displays.sampleCountChangeScreen)>()                          { return displays.sampleCountChangeScreen; }
template<> decltype(displays.changeGasMin)                                     &getRefByType<decltype(displays.changeGasMin)>()                                     { return displays.changeGasMin; }
template<> decltype(displays.changeGasMax)                                     &getRefByType<decltype(displays.changeGasMax)>()                                     { return displays.changeGasMax; }
template<> decltype(displays.changeBremsMin)                                   &getRefByType<decltype(displays.changeBremsMin)>()                                   { return displays.changeBremsMin; }
template<> decltype(displays.changeBremsMax)                                   &getRefByType<decltype(displays.changeBremsMax)>()                                   { return displays.changeBremsMax; }
#ifdef FEATURE_GAMETRAK
template<> decltype(displays.changeGametrakXMin)                               &getRefByType<decltype(displays.changeGametrakXMin)>()                               { return displays.changeGametrakXMin; }
template<> decltype(displays.changeGametrakXMax)                               &getRefByType<decltype(displays.changeGametrakXMax)>()                               { return displays.changeGametrakXMax; }
template<> decltype(displays.changeGametrakYMin)                               &getRefByType<decltype(displays.changeGametrakYMin)>()                               { return displays.changeGametrakYMin; }
template<> decltype(displays.changeGametrakYMax)                               &getRefByType<decltype(displays.changeGametrakYMax)>()                               { return displays.changeGametrakYMax; }
template<> decltype(displays.changeGametrakDistMin)                            &getRefByType<decltype(displays.changeGametrakDistMin)>()                            { return displays.changeGametrakDistMin; }
template<> decltype(displays.changeGametrakDistMax)                            &getRefByType<decltype(displays.changeGametrakDistMax)>()                            { return displays.changeGametrakDistMax; }
#endif

template<> decltype(displays.wifiModeChangeScreen)                             &getRefByType<decltype(displays.wifiModeChangeScreen)>()                             { return displays.wifiModeChangeScreen; }
template<> decltype(displays.wifiTxPowerChangeScreen)                          &getRefByType<decltype(displays.wifiTxPowerChangeScreen)>()                          { return displays.wifiTxPowerChangeScreen; }

template<> decltype(displays.gasGraphDisplay)                                  &getRefByType<decltype(displays.gasGraphDisplay)>()                                  { return displays.gasGraphDisplay; }
template<> decltype(displays.bremsGraphDisplay)                                &getRefByType<decltype(displays.bremsGraphDisplay)>()                                { return displays.bremsGraphDisplay; }
template<> decltype(displays.avgSpeedGraphDisplay)                             &getRefByType<decltype(displays.avgSpeedGraphDisplay)>()                             { return displays.avgSpeedGraphDisplay; }
template<> decltype(displays.avgSpeedKmhGraphDisplay)                          &getRefByType<decltype(displays.avgSpeedKmhGraphDisplay)>()                          { return displays.avgSpeedKmhGraphDisplay; }
template<> decltype(displays.sumCurrentGraphDisplay)                           &getRefByType<decltype(displays.sumCurrentGraphDisplay)>()                           { return displays.sumCurrentGraphDisplay; }
template<> decltype(displays.sumAbsoluteCurrentGraphDisplay)                   &getRefByType<decltype(displays.sumAbsoluteCurrentGraphDisplay)>()                   { return displays.sumAbsoluteCurrentGraphDisplay; }
#ifdef GLUMP_CONTROLLER
template<> decltype(displays.frontVoltageGraphDisplay)                         &getRefByType<decltype(displays.frontVoltageGraphDisplay)>()                         { return displays.frontVoltageGraphDisplay; }
template<> decltype(displays.backVoltageGraphDisplay)                          &getRefByType<decltype(displays.backVoltageGraphDisplay)>()                          { return displays.backVoltageGraphDisplay; }
#endif
#ifdef FEATURE_BMS
template<> decltype(displays.bmsVoltageGraphDisplay)                           &getRefByType<decltype(displays.bmsVoltageGraphDisplay)>()                           { return displays.bmsVoltageGraphDisplay; }
template<> decltype(displays.bmsCurrentGraphDisplay)                           &getRefByType<decltype(displays.bmsCurrentGraphDisplay)>()                           { return displays.bmsCurrentGraphDisplay; }
template<> decltype(displays.bmsPowerGraphDisplay)                             &getRefByType<decltype(displays.bmsPowerGraphDisplay)>()                             { return displays.bmsPowerGraphDisplay; }
template<> decltype(displays.sumCurrentsComparisonGraphDisplay)                &getRefByType<decltype(displays.sumCurrentsComparisonGraphDisplay)>()                { return displays.sumCurrentsComparisonGraphDisplay; }
#endif
#ifdef GLUMP_CONTROLLER
template<> decltype(displays.motorCurrentsGraphDisplay)                        &getRefByType<decltype(displays.motorCurrentsGraphDisplay)>()                        { return displays.motorCurrentsGraphDisplay; }
#endif


void deconstructScreen()
{
    if (currentDisplay)
    {
        currentDisplay->stop();
        currentDisplay->~Display();
        currentDisplay = nullptr;
    }
}

template<typename T, typename... Args>
void switchScreenImpl(Args&&... args)
{
    deconstructScreen();

    T &ref = getRefByType<T>();
    new (&ref) T{std::forward<Args>(args)...};
    currentDisplay = &ref;
    ref.start();
    ref.initScreen();
    ref.update();
    ref.redraw();
}

std::function<void()> changeScreenCallback;

template<typename T, typename... Args>
void switchScreen(Args&&... args)
{
    if (currentDisplay)
        changeScreenCallback = [&args...](){ switchScreenImpl<T>(std::forward<Args>(args)...); };
    else
        switchScreenImpl<T>(std::forward<Args>(args)...);
}

void initScreen()
{
    tft.init();
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK);
    tft.pushImage(0, 40, icons::logo.WIDTH, icons::logo.HEIGHT, icons::logo.buffer);
    tft.drawString("Bobbycar-OS", 32, 200, 4);
    tft.drawString("booting...", 32, 225, 4);
}

void updateDisplay()
{
    if (rotated)
    {
        const auto rotatedCopy = rotated;
        rotated = 0;

        if (currentDisplay)
            currentDisplay->rotate(rotatedCopy);
    }

    if (requestFullRedraw)
    {
        requestFullRedraw = false;

        tft.init();

        if (currentDisplay)
            currentDisplay->initScreen();
    }

    if (confirmButtonPressed)
    {
        confirmButtonPressed = false;

        if (currentDisplay)
            currentDisplay->confirm();
    }

    if (confirmButtonLongPressed)
    {
        confirmButtonLongPressed = false;
        Serial.println("todo: implement long press for confirm");
    }

    if (backButtonPressed)
    {
        backButtonPressed = false;

        if (currentDisplay)
            currentDisplay->back();
    }

    if (backButtonLongPressed)
    {
        backButtonLongPressed = false;
        Serial.println("todo: implement long press for back");
    }

    if (currentDisplay)
        currentDisplay->update();

    if (changeScreenCallback)
    {
        changeScreenCallback();
        changeScreenCallback = {};
    }
}

void redrawDisplay()
{
    if (currentDisplay)
    {
        tft.setSwapBytes(settings.boardcomputerHardware.swapScreenBytes);
        currentDisplay->redraw();
    }
}
}
