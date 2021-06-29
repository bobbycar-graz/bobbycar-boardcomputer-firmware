#pragma once

namespace {
//AboutMenu
constexpr char TEXT_ABOUT[] = "About";
constexpr char TEXT_BACK[] = "Back";

//AccessPointWifiSettingsMenu
constexpr char TEXT_ACCESSPOINTWIFISETTINGS[] = "Access Point WiFi settings";
constexpr char TEXT_WIFISOFTAP[] = "softAP()";
constexpr char TEXT_WIFISOFTAPDISCONNECT[] = "softAPdisconnect()";
constexpr char TEXT_WIFISOFTAPDISCONNECTWIFIOFF[] = "softAPdisconnect() wifioff";
constexpr char TEXT_WIFISOFTAPENABLEIPV6[] = "softAPenableIpV6()";
//constexpr char TEXT_BACK[] = "Back";

#ifdef FEATURE_BLUETOOTH
//BluetoothSettingsMenu
constexpr char TEXT_BLUETOOTHSETTINGS[] = "Bluetooth settings";
constexpr char TEXT_BLUETOOTHBEGIN[] = "begin()";
constexpr char TEXT_BLUETOOTHBEGINMASTER[] = "begin() Master";
constexpr char TEXT_BLUETOOTHFLUSH[] = "flush()";
constexpr char TEXT_BLUETOOTHEND[] = "end()";
constexpr char TEXT_BLUETOOTHDISCONNECT[] = "disconnect()";
constexpr char TEXT_AUTOBLUETOOTHMODE[] = "Auto bluetooth mode";
//constexpr char TEXT_BACK[] = "Back";

//BmsMenu
#ifdef FEATURE_BMS
constexpr char TEXT_BMS[] = "BMS";
constexpr char TEXT_CONNECTBMS[] = "Connect BMS";
constexpr char TEXT_DISCONNECTBMS[] = "Disonnect BMS";
constexpr char TEXT_TURNONCHARGE[] = "Turn on charge";
constexpr char TEXT_TURNOFFCHARGE[] = "Turn off charge";
constexpr char TEXT_TURNONDISCHARGE[] = "Turn on discharge";
constexpr char TEXT_TURNOFFDISCHARGE[] = "Turn off discharge";
//constexpr char TEXT_BACK[] = "Back";
#endif
#endif

//DebugMenu
constexpr char TEXT_LOADSETTINGS[] = "Load settings";
constexpr char TEXT_SAVESETTINGS[] = "Save settings";
constexpr char TEXT_ERASENVS[] = "Erase NVS";
constexpr char TEXT_FRONTCOMMAND[] = "Front command";
constexpr char TEXT_BACKCOMMAND[] = "Back command";
constexpr char TEXT_FRONTLEFTCOMMAND[] = "Front left command";
constexpr char TEXT_FRONTRIGHTCOMMAND[] = "Front right command";
constexpr char TEXT_BACKLEFTCOMMAND[] = "Back left command";
constexpr char TEXT_BACKRIGHTCOMMAND[] = "Back right command";
constexpr char TEXT_FRONTFEEDBACK[] = "Front feedback";
constexpr char TEXT_BACKFEEDBACK[] = "Back feedback";
constexpr char TEXT_FRONTLEFTFEEDBACK[] = "Front left feedback";
constexpr char TEXT_FRONTRIGHTFEEDBACK[] = "Front right feedback";
constexpr char TEXT_BACKLEFTFEEDBACK[] = "Back left feedback";
constexpr char TEXT_BACKRIGHTFEEDBACK[] = "Back right feedback";
//constexpr char TEXT_BACK[] = "Back";

//MainMenu
constexpr char TEXT_MAINMENU[] = "Main menu";
constexpr char TEXT_STATUS[] = "Status";
constexpr char TEXT_SELECTMODE[] = "Select mode";
constexpr char TEXT_MODESETTINGS[] = "Mode settings";
constexpr char TEXT_PRESETS[] = "Presets";
constexpr char TEXT_PROFILES[] = "Profiles";
constexpr char TEXT_GRAPHS[] = "Graphs";
//constexpr char TEXT_BMS[] = "BMS";
constexpr char TEXT_SETTINGS[] = "Settings";
constexpr char TEXT_LOCKVEHICLE[] = "Lock vehicle";
constexpr char TEXT_MOSFETS[] = "Mosfets";
constexpr char TEXT_DEMOS[] = "Demos";
constexpr char TEXT_POWEROFF[] = "Poweroff";
constexpr char TEXT_REBOOT[] = "Reboot";
constexpr char TEXT_DEBUG[] = "Debug";

//SettingsMenu
//constexpr char TEXT_SETTINGS[] = "Settings";
constexpr char TEXT_LIMITSSETTINGS[] = "Limits settings";
constexpr char TEXT_WIFISETTINGS[] = "WiFi settings";
//constexpr char TEXT_BLUETOOTHSETTINGS[] = "Bluetooth settings";
constexpr char TEXT_MODESSETTINGS[] = "Modes settings";
constexpr char TEXT_CONTROLLERHARDWARESETTINGS[] = "Controller H/W settings";
constexpr char TEXT_BOARDCOMPUTERHARDWARESETTINGS[] = "Boardcomputer H/W settings";
constexpr char TEXT_AUTOCONNECTBMS[] = "Auto connect BMS";
constexpr char TEXT_BUZZER[] = "Buzzer";
constexpr char TEXT_FRONTLED[] = "Front LED";
constexpr char TEXT_BACKLED[] = "Back LED";
//constexpr char TEXT_ABOUT[] = "About";
//constexpr char TEXT_BACK[] = "Back";

//ControllerHardwareSettings
//constexpr char TEXT_CONTROLLERHARDWARESETTINGS[] = "Controller H/W settings";
constexpr char TEXT_WHEELDIAMETERMM[] = "Wheel diameter (mm)";
constexpr char TEXT_WHEELDIAMETERINCH[] = "Wheel diameter (inch)";
constexpr char TEXT_NUMMAGNETPOLES[] = "Num magnet poles";
constexpr char TEXT_SETENABLED[] = "Set enabled";
constexpr char TEXT_SETINVERTED[] = "Set inverted";
constexpr char TEXT_SWAPFRONTBACK[] = "Swap front/back";
//constexpr char TEXT_BACK[] = "Back";

//StationWifiSettingsMenu
constexpr char TEXT_STATIONWIFISETTINGS[] = "Station WiFi settings";
constexpr char TEXT_AUTOWIFIMODE[] = "Auto wifi mode";
constexpr char TEXT_AUTOENABLEAP[] = "Auto enable AP";
constexpr char TEXT_WIFIRECONNECT[] = "reconnect()";
constexpr char TEXT_WIFIDISCONNECT[] = "disconnect()";
constexpr char TEXT_WIFICHANGEAUTOCONNECT[] = "Change auto connect";
constexpr char TEXT_WIFICHANGEAUTORECONNECT[] = "Change auto reconnect";
constexpr char TEXT_WIFIENABLEIPV6[] = "enableIpV6()";
//constexpr char TEXT_BACK[] = "Back";

//DemosMenu
//constexpr char TEXT_DEMOS[] = "Demos";
constexpr char TEXT_STARFIELD[] = "Starfield";
constexpr char TEXT_PINGPONG[] = "PingPong";
constexpr char TEXT_SPIRO[] = "Spiro";
constexpr char TEXT_GAMEOFLIFE[] = "GameOfLife";
//constexpr char TEXT_BACK[] = "Back";

//BuzzerMenu
//constexpr char TEXT_BUZZER[] = "Buzzer";
constexpr char TEXT_FRONTFREQ[] = "Front freq";
constexpr char TEXT_FRONTPATTERN[] = "Front pattern";
constexpr char TEXT_BACKFREQ[] = "Back freq";
constexpr char TEXT_BACKPATTERN[] = "Back pattern";
constexpr char TEXT_REVERSEBEEP[] = "Reverse beep";
constexpr char TEXT_REVERSEBEEPFREQ0[] = "Reverse beep freq0";
constexpr char TEXT_REVERSEBEEPFREQ1[] = "Reverse beep freq1";
constexpr char TEXT_REVERSEBEEPDURATION0[] = "Reverse beep duration0";
constexpr char TEXT_REVERSEBEEPDURATION1[] = "Reverse beep duration1";
//constexpr char TEXT_BACK[] = "Back";

//LimitsSettingsMenu
//constexpr char TEXT_LIMITSSETTINGS[] = "Limit settings";
constexpr char TEXT_IMOTMAX[] = "iMotMax";
constexpr char TEXT_IDCMAX[] = "iDcMax";
constexpr char TEXT_NMOTMAXKMH[] = "nMotMaxKmh";
constexpr char TEXT_NMOTMAX[] = "nMotMax";
constexpr char TEXT_FIELDWEAKMAX[] = "fldWkMax";
constexpr char TEXT_PHASEADVMAX[] = "phsAdvMax";
//constexpr char TEXT_BACK[] = "Back";

//DebugMenu
//constexpr char TEXT_DEBUG[] = "Debug";
constexpr char TEXT_DYNAMICMENU[] = "Dynamic menu";
//constexpr char TEXT_BACK[] = "Back";

//DefaultModeSettingsMenu
//constexpr char TEXT_DEFAULTMODESETTINGS[] = "Default mode settings";
constexpr char TEXT_SETMODELMODE[] = "Set model mode";
constexpr char TEXT_SQUAREGAS[] = "Square gas";
constexpr char TEXT_SQUAREBREMS[] = "Square brems";
constexpr char TEXT_ENABLESMOOTHING[] = "Enable smoothing";
constexpr char TEXT_SETSMOOTHING[] = "Set smoothing";
constexpr char TEXT_SETFRONTPERCENTAGE[] = "Set front percentage";
constexpr char TEXT_SETBACKPERCENTAGE[] = "Set back percentage";
constexpr char TEXT_SETADDSCHWELLE[] = "Set add Schwelle";
constexpr char TEXT_SETGAS1WERT[] = "Set Gas 1 Wert";
constexpr char TEXT_SETGAS2WERT[] = "Set Gas 2 Wert";
constexpr char TEXT_SETBREMS1WERT[] = "Set Brems 1 Wert";
constexpr char TEXT_SETBREMS2WERT[] = "Set Brems 2 Wert";
//constexpr char TEXT_BACK[] = "Back";

//DynamicDebugMenu
//constexpr char TEXT_DYNAMICMENU[] = "Dynamic menu";
//constexpr char TEXT_BACK[] = "Back";

//EnableMenu
//constexpr char TEXT_SETENABLED[] = "Set enabled";
constexpr char TEXT_ENABLEFRONTLEFT[] = "Enable front left";
constexpr char TEXT_ENABLEFRONTRIGHT[] = "Enable front right";
constexpr char TEXT_ENABLEBACKLEFT[] = "Enable back left";
constexpr char TEXT_ENABLEBACKRIGHT[] = "Enable back right";
//constexpr char TEXT_BACK[] = "Back";

//GametrakModeSettingsMenu
constexpr char TEXT_GAMETRAKMODESETTINGS[] = "Gametrak mode settings";

//GenericWifiSettingsMenu
constexpr char TEXT_GENERICWIFISETTINGS[] = "Generic WiFi settings";
constexpr char TEXT_WIFICHANGEMODE[] = "Change mode";
constexpr char TEXT_WIFICHANGESLEEP[] = "Change sleep";
constexpr char TEXT_WIFICHANGETXPOWER[] = "Change tx power";
constexpr char TEXT_WIFISCAN[] = "WiFi scan";
//constexpr char TEXT_BACK[] = "Back";

//GraphsMenu
//constexpr char TEXT_GRAPHS[] = "Graphs";
constexpr char TEXT_GAS[] = "Gas";
constexpr char TEXT_BREMS[] = "Brems";
constexpr char TEXT_POTIS[] = "Potis";
constexpr char TEXT_AVGSPEED[] = "Avg. speed";
constexpr char TEXT_AVGSPEEDKMH[] = "Avg. speed KMH";
constexpr char TEXT_SUMCURRENT[] = "Sum current";
constexpr char TEXT_FRONTVOLTAGE[] = "Front voltage";
constexpr char TEXT_BACKVOLTAGE[] = "Back voltage";
constexpr char TEXT_VOLTAGES[] = "Voltages";
constexpr char TEXT_BMSVOLTAGE[] = "BMS voltage";
constexpr char TEXT_BMSCURRENT[] = "BMS current";
constexpr char TEXT_BMSPOWER[] = "BMS power";
constexpr char TEXT_SUMCURRENTSCOMPARISON[] = "Sum currents comparison";
constexpr char TEXT_MOTORCURRENTS[] = "Motor currents";
//constexpr char TEXT_BACK[] = "Back";

//InvertMenu
//constexpr char TEXT_SETINVERTED[] = "Set inverted";
constexpr char TEXT_INVERTFRONTLEFT[] = "Invert front left";
constexpr char TEXT_INVERTFRONTRIGHT[] = "Invert front right";
constexpr char TEXT_INVERTBACKLEFT[] = "Invert back left";
constexpr char TEXT_INVERTBACKRIGHT[] = "Invert back right";
//constexpr char TEXT_BACK[] = "Back";

//LarsmModeSettingsMenu
constexpr char TEXT_LARSMMODESETTINGS[] = "Larsm mode settings";
//constexpr char TEXT_SETMODELMODE[] = "Set model mode";
constexpr char TEXT_SETMODE[] = "Set mode";
constexpr char TEXT_SETITERATIONS[] = "Set iterations";
//constexpr char TEXT_BACK[] = "Back";

//ModesSettingsMenu
//constexpr char TEXT_MODESSETTINGS[] = "Modes settings";
constexpr char TEXT_DEFAULTMODESETTIGNS[] = "Default mode settings";
constexpr char TEXT_TEMPOMATMODESETTINGS[] = "Tempomat mode settings";
//constexpr char TEXT_LARSMMODESETTINGS[] = "Larsm mode settings";
//constexpr char TEXT_GAMETRAKMODESETTINGS[] = "Gametrak mode settings";
//constexpr char TEXT_BACK[] = "Back";

//MosfetsMenu
//constexpr char TEXT_MOSFETS[] = "Mosfets";
constexpr char TEXT_MOSFET0[] = "Mosfet0";
constexpr char TEXT_MOSFET1[] = "Mosfet1";
constexpr char TEXT_MOSFET2[] = "Mosfet2";

//TempomatModeSettingsMenu
//constexpr char TEXT_TEMPOMATMODESETTINGS[] = "Tempomat mode settings";
//constexpr char TEXT_SETMODELMODE[] = "Set model mode";
//constexpr char TEXT_BACK[] = "Back";

//WiFiSettingsMenu
//constexpr char TEXT_WIFISETTINGS[] = "WiFi settings";
//constexpr char TEXT_GENERICWIFISETTINGS[] = "Generic WiFi settings";
//constexpr char TEXT_BACK[] = "Back";

//BoardcomputerHardwareSettingsMenu
//constexpr char TEXT_BOARDCOMPUTERHARDWARESETTINGS[] = "Boardcomputer H/W settings";
constexpr char TEXT_CALIBRATE[] = "Calibrate";
constexpr char TEXT_SETSAMPLECOUNT[] = "Set sampleCount";
constexpr char TEXT_SETGASMIN[] = "Set gasMin";
constexpr char TEXT_SETGASMAX[] = "Set gasMax";
constexpr char TEXT_SETBREMSMIN[] = "Set bremsMin";
constexpr char TEXT_SETBREMSMAX[] = "Set bremsMax";
constexpr char TEXT_SETDPADDEBOUNCE[] = "Set dpadDebounce";
constexpr char TEXT_GAMETRAKCALIBRATE[] = "Gametrak calibrate";
constexpr char TEXT_SETGAMETRAKXMIN[] = "Set gametrakXMin";
constexpr char TEXT_SETGAMETRAKXMAX[] = "Set gametrakXMax";
constexpr char TEXT_SETGAMETRAKYMIN[] = "Set gametrakYMin";
constexpr char TEXT_SETGAMETRAKYMAX[] = "Set gametrakYMax";
constexpr char TEXT_SETGAMETRAKDISTMIN[] = "Set gametrakDistMin";
constexpr char TEXT_SETGAMETRAKDISTMAX[] = "Set gametrakDistMax";
constexpr char TEXT_SWAPSCREENBYTES[] = "Swap screen bytes";
constexpr char TEXT_TIMERS[] = "Timers";
//constexpr char TEXT_BACK[] = "Back";

//ProfilesMenu
//constexpr char TEXT_PROFILES[] = "Profiles";
constexpr char TEXT_PROFILE0[] = "Profile 0";
constexpr char TEXT_PROFILE1[] = "Profile 1";
constexpr char TEXT_PROFILE2[] = "Profile 2";
constexpr char TEXT_PROFILE3[] = "Profile 3";
//constexpr char TEXT_BACK[] = "Back";

//PresetsMenu
//constexpr char TEXT_PRESETS[] = "Presets";
constexpr char TEXT_DEFAULTEVERYTHING[] = "Default everything";
constexpr char TEXT_DEFAULTLIMITS[] = "Default limits";
constexpr char TEXT_KIDSLIMITS[] = "Kids limits";
constexpr char TEXT_DEFAULTPOTI[] = "Default poti";
constexpr char TEXT_DEFAULTCONTROLLERHARDWARE[] = "Default controller H/W";
constexpr char TEXT_MOSFETSOFFCONTROLLERHARDWARE[] = "MOSFETs off controller H/W";
constexpr char TEXT_SPINNERCONTROLLERHARDWARE[] = "Spinner controller H/W";
constexpr char TEXT_DEFAULTBOARDCOMPUTERHARDWARE[] = "Default boardcomputer H/W";
constexpr char TEXT_DEFAULTDEFAULTMODE[] = "Default defaultMode";
constexpr char TEXT_SINUSOIDALDEFAULTMODE[] = "Sinusoidal defaultMode";
constexpr char TEXT_DEFAULTTEMPOMATMODE[] = "Default tempomatMode";
constexpr char TEXT_DEFAULTLARSMMODE[] = "Default larsmMode";
constexpr char TEXT_STREET[] = "Street";
constexpr char TEXT_SIDEWALK[] = "Sidewalk";
constexpr char TEXT_POLICE[] = "Police";
constexpr char TEXT_RACE[] = "Race";

//SelectModeMenu
//constexpr char TEXT_SELECTMODE[] = "Select mode";
constexpr char TEXT_DEFAULT[] = "Default";
constexpr char TEXT_TEMPOMAT[] = "Tempomat";
constexpr char TEXT_LARSM[] = "Larsm";
constexpr char TEXT_GAMETRAK[] = "Gametrak";
//constexpr char TEXT_BACK[] = "Back";

//TimersMenu
//constexpr char TEXT_TIMERS[] = "Timers";
constexpr char TEXT_POTIREADRATE[] = "Poti read rate";
constexpr char TEXT_MODEUPDATERATE[] = "Mode update rate";
constexpr char TEXT_STATSUPDATERATE[] = "Stats update rate";
constexpr char TEXT_DISPLAYUPDATERATE[] = "Display update rate";
constexpr char TEXT_DISPLAYREDRAWRATE[] = "Display redraw rate";
//constexpr char TEXT_BACK[] = "Back";

//ChangeValueDisplay<BluetoothMode>
constexpr char TEXT_OFF[] = "Off";
constexpr char TEXT_MASTER[] = "Master";
constexpr char TEXT_SLAVE[] = "Slave";

//ChangeValueDisplay<bool>
constexpr char TEXT_TRUE[] = "true";
constexpr char TEXT_FALSE[] = "false";
//constexpr char TEXT_BACK[] = "Back";

//ChangeValueDisplay<ControlMode>
constexpr char TEXT_OPENMODE[] = "Open mode";
constexpr char TEXT_VOLTAGE[] = "Voltage";
constexpr char TEXT_SPEED[] = "Speed";
constexpr char TEXT_TORQUE[] = "Torque";
//constexpr char TEXT_BACK[] = "Back";

//ChangeValueDisplay<ControlType>
constexpr char TEXT_COMMUTATION[] = "Commutation";
constexpr char TEXT_SINUSOIDAL[] = "Sinusoidal";
constexpr char TEXT_FIELDORIENTEDCONTROL[] = "Field oriented control";
//constexpr char TEXT_BACK[] = "Back";

//ChangeValueDisplay<LarsmMode::Mode>
constexpr char TEXT_LARSMMODE1[] = "Mode1";
constexpr char TEXT_LARSMMODE2[] = "Mode2";
constexpr char TEXT_LARSMMODE3[] = "Mode3";
constexpr char TEXT_LARSMMODE4[] = "Mode4";
//constexpr char TEXT_BACK[] = "Back";

//ChangeValueDisplay<UnifiedModelMode>
//constexpr char TEXT_COMMUTATION[] = "Commutation";
//constexpr char TEXT_SINUSOIDAL[] = "Sinusoidal";
constexpr char TEXT_FOCVOLTAGE[] = "FOC/Voltage";
constexpr char TEXT_FOCSPEED[] = "FOC/Speed";
constexpr char TEXT_FOCTORQUE[] = "FOC/Torque";
//constexpr char TEXT_BACK[] = "Back";

//ChangeValueDisplay<wifi_mode_t>
constexpr char TEXT_WIFI_MODE_NULL[] = "WIFI_MODE_NULL";
constexpr char TEXT_WIFI_MODE_STA[] = "WIFI_MODE_STA";
constexpr char TEXT_WIFI_MODE_AP[] = "WIFI_MODE_AP";
constexpr char TEXT_WIFI_MODE_APSTA[] = "WIFI_MODE_APSTA";
//constexpr char TEXT_BACK[] = "Back";

//ChangeValueDisplay<wifi_power_t>
constexpr char TEXT_WIFI_POWER_19_5dBm[] = "19.5dBm";
constexpr char TEXT_WIFI_POWER_19dBm[] = "19dBm";
constexpr char TEXT_WIFI_POWER_18_5dBm[] = "18.5dBm";
constexpr char TEXT_WIFI_POWER_17dBm[] = "17dBm";
constexpr char TEXT_WIFI_POWER_15dBm[] = "15dBm";
constexpr char TEXT_WIFI_POWER_13dBm[] = "13dBm";
constexpr char TEXT_WIFI_POWER_11dBm[] = "11dBm";
constexpr char TEXT_WIFI_POWER_8_5dBm[] = "8.5dBm";
constexpr char TEXT_WIFI_POWER_7dBm[] = "7dBm";
constexpr char TEXT_WIFI_POWER_5dBm[] = "5dBm";
constexpr char TEXT_WIFI_POWER_2dBm[] = "2dBm";
constexpr char TEXT_WIFI_POWER_MINUS_1dBm[] = "-1dBm";
//constexpr char TEXT_BACK[] = "Back";

#ifdef FEATURE_CAN
constexpr char TEXT_POWERSUPPLY[] = "Powersupply";
#endif
}
