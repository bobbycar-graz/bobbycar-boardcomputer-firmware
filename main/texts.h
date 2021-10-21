#pragma once

namespace {
//AboutMenu
constexpr char TEXT_ABOUT[] = "About";
constexpr char TEXT_BACK[] = "Back";

//AccessPointWifiSettingsMenu
constexpr char TEXT_ACCESSPOINTWIFISETTINGS[] = "Access Point WiFi settings";
//constexpr char TEXT_BACK[] = "Back";

#ifdef TEXTS_PLUGIN
#include TEXTS_PLUGIN
#endif

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

#ifdef FEATURE_BLE
//BleSettingsMenu
constexpr char TEXT_BLESETTINGS[] = "BLE settings";
constexpr char TEXT_BLEENABLED[] = "BLE enabled";
//constexpr char TEXT_BACK[] = "Back";
#endif

#ifdef FEATURE_CLOUD
//CloudSettingsMenu
constexpr char TEXT_CLOUDSETTINGS[] = "Cloud settings";
constexpr char TEXT_CLOUDENABLED[] = "Cloud enabled";
constexpr char TEXT_CLOUDTRANSMITTIMEOUT[] = "Transmit timeout";
constexpr char TEXT_CLOUDCOLLECTRATE[] = "Cloud collect rate";
constexpr char TEXT_CLOUDSENDRATE[] = "Cloud send rate";
//constexpr char TEXT_BACK[] = "Back";
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
constexpr char TEXT_GARAGE[] = "Garage";
constexpr char TEXT_UPDATE[] = "Update";
constexpr char TEXT_POWEROFF[] = "Poweroff";
constexpr char TEXT_REBOOT[] = "Reboot";
constexpr char TEXT_DEBUG[] = "Debug";
constexpr char TEXT_BATTERY[] = "Battery";

//BatteryMenu
constexpr char TEXT_CELL_SERIES[] = "Cells (Series)";
constexpr char TEXT_CELL_PARALLEL[] = "Cells (Parallel)";
constexpr char TEXT_SELECT_CELL_TYPE[] = "Select Cell Type";
constexpr char TEXT_CELL_TYPE[] = "Cell Type";
constexpr char TEXT_BATTERY_CALIBRATE[] = "Calibrate Voltages";
constexpr char TEXT_BATTERY_TYPE_22P[] = "22P cells";
constexpr char TEXT_BATTERY_TYPE_HG2[] = "HG2 cells";
constexpr char TEXT_BATTERY_TYPE_MH1[] = "MH1 cells";
constexpr char TEXT_BATTERY_TYPE_VTC5[] = "VTC5 cells";
constexpr char TEXT_BATTERY_WHKM[] = "Wh per km";
constexpr char TEXT_BATTERY_APPLYCALIB[] = "Apply calibration";
constexpr char TEXT_VOLTAGECALIBRATION_RESET[] = "Reset calibration";

//CalibrateVoltageDisplay
constexpr char TEXT_VOLTAGECALIBRATION_30V[] = "Calibrate 30.0V";
constexpr char TEXT_VOLTAGECALIBRATION_50V[] = "Calibrate 50.0V";
constexpr char TEXT_VOLTAGECALIBRATION_VALUE_30_FRONT[] = "30V Front";
constexpr char TEXT_VOLTAGECALIBRATION_VALUE_30_BACK[] = "30V Back";
constexpr char TEXT_VOLTAGECALIBRATION_VALUE_50_FRONT[] = "50V Front";
constexpr char TEXT_VOLTAGECALIBRATION_VALUE_50_BACK[] = "50V Back";

//SettingsMenu
//constexpr char TEXT_SETTINGS[] = "Settings";
constexpr char TEXT_BACKLIGHT[] = "Backlight";
constexpr char TEXT_LIMITSSETTINGS[] = "Limits settings";
constexpr char TEXT_WIFISETTINGS[] = "WiFi settings";
//constexpr char TEXT_BLUETOOTHSETTINGS[] = "Bluetooth settings";
//constexpr char TEXT_BLESETTINGS[] = "BLE settings";
//constexpr char TEXT_CLOUDSETTINGS[] = "Cloud settings";
constexpr char TEXT_TIME[] = "Time";
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
#ifdef FEATURE_CAN
constexpr char TEXT_FRONTSENDCAN[] = "Front send CAN";
constexpr char TEXT_BACKSENDCAN[] = "Back send CAN";
constexpr char TEXT_CANTRANSMITTIMEOUT[] = "CanTransmitTimeout";
constexpr char TEXT_CANRECEIVETIMEOUT[] = "CanReceiveTimeout";
#endif
//constexpr char TEXT_BACK[] = "Back";

//StationWifiSettingsMenu
constexpr char TEXT_STATIONWIFISETTINGS[] = "Station WiFi settings";
constexpr char TEXT_WIFIENABLED[] = "WiFi enabled";
constexpr char TEXT_WIFITRIGGERSCAN[] = "Trigger Scan";
constexpr char TEXT_WIFISCANRESULTS[] = "WiFi scan results";
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
constexpr char TEXT_MODELMODE[] = "Model mode";
constexpr char TEXT_HYBRIDMODE[] = "Hybrid mode";
constexpr char TEXT_SQUAREGAS[] = "Square gas";
constexpr char TEXT_SQUAREBREMS[] = "Square brems";
constexpr char TEXT_ENABLESMOOTHINGUP[] = "Enable up smoothing";
constexpr char TEXT_ENABLESMOOTHINGDOWN[] = "Enable down smoothing";
constexpr char TEXT_ENABLEFWSMOOTHINGUP[] = "Enable fw up smoothing";
constexpr char TEXT_ENABLEFWSMOOTHINGDOWN[] = "Enable fw down smoothing";
constexpr char TEXT_FWSMOOTHING_LIMIT[] = "Fw smooth lower limit";
constexpr char TEXT_SMOOTHINGVAL[] = "Smoothing";
constexpr char TEXT_FRONTPERCENTAGE[] = "Front %";
constexpr char TEXT_BACKPERCENTAGE[] = "Back %";
constexpr char TEXT_ADDSCHWELLE[] = "Add/Sub Lim";
constexpr char TEXT_SUBGASVAL[] = "Add Gas";
constexpr char TEXT_SUBBRAKEVAL[] = "Add Brake";
constexpr char TEXT_ADDGASVAL[] = "Sub Gas";
constexpr char TEXT_ADDBRAKEVAL[] = "Sub Brake";
constexpr char TEXT_HYBRIDENABLE[] = "Enable Hybrid mode";
constexpr char TEXT_HYBRIDACTIVATIONLIMIT[] = "Hybrid activation-limit";
constexpr char TEXT_HYBRIDDEACTIVATIONLIMIT[] = "Hybrid deactivation-limit";
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
constexpr char TEXT_RSSI[] = "RSSI";
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
//constexpr char TEXT_MODELMODE[] = "Model mode";
constexpr char TEXT_SETMODE[] = "Set mode";
constexpr char TEXT_SETITERATIONS[] = "Set iterations";
//constexpr char TEXT_BACK[] = "Back";

//LedstripMenu
constexpr char TEXT_LEDSTRIP[] = "Ledstrip";
constexpr char TEXT_LEDANIMATION[] = "LED Animation";
constexpr char TEXT_BRAKELIGHTS[] = "Brake Lights";
constexpr char TEXT_BLINKANIMATION[] = "Blink animation";
constexpr char TEXT_ANIMATION_TYPE[] = "Blink animation";
constexpr char TEXT_LEDSCOUNT[] = "LEDs Count";
constexpr char TEXT_CENTEROFFSET[] = "Center Offset";
constexpr char TEXT_SMALLOFFSET[] = "Small Offset";
constexpr char TEXT_BIGOFFSET[] = "Big Offset";
constexpr char TEXT_LEDSTRIP_MILLIAMP[] = "Ledstrip 0.1A";
constexpr char TEXT_BLINKBEEP[] = "Blink Beep";
constexpr char TEXT_FULLBLINK[] = "Full blink";
constexpr char TEXT_LEDSTRIP_STVO[] = "Enable StVO";
constexpr char TEXT_STVO_FRONTOFFSET[] = "StVO Front Offset";
constexpr char TEXT_STVO_FRONTLENGTH[] = "StVO Front Length";
constexpr char TEXT_STVO_ENABLEFRONTLIGHT[] = "StVO Front Enable";
constexpr char TEXT_ANIMATION_MULTIPLIER[] = "Animation Multiplier";
constexpr char TEXT_LEDSTRIP_BRIGHTNESS[] = "Ledstrip Brightness";
//constexpr char TEXT_BACK[] = "Back";

//LedstripSelectAnimationMenu
constexpr char TEXT_SELECTANIMATION[] = "Select Animation";
constexpr char TEXT_ANIMATION_DEFAULTRAINBOW[] = "Default Rainbow";
constexpr char TEXT_ANIMATION_BETTERRAINBOW[] = "Better Rainbow";
constexpr char TEXT_ANIMATION_SPEEDSYNCANIMATION[] = "Speed Sync";

//LedstripSelectBlinkMenu
constexpr char TEXT_ANIMATION_BLINKNONE[] = "Blink Off";
constexpr char TEXT_ANIMATION_BLINKLEFT[] = "Blink Left";
constexpr char TEXT_ANIMATION_BLINKRIGHT[] = "Blink Right";
constexpr char TEXT_ANIMATION_BLINKBOTH[] = "Blink Both";

//LockscreenSettingsMenu
constexpr char TEXT_LOCKSCREENSETTINGS[] = "Lockscreen Settings";
constexpr char TEXT_ALLOWPRESETSWITCH[] = "Allow preset switch";
constexpr char TEXT_PINDIGIT0[] = "PIN digit0";
constexpr char TEXT_PINDIGIT1[] = "PIN digit1";
constexpr char TEXT_PINDIGIT2[] = "PIN digit2";
constexpr char TEXT_PINDIGIT3[] = "PIN digit3";
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
constexpr char TEXT_APPLY[] = "Apply";
constexpr char TEXT_NCRUISEMOTTGT[] = "nCruiseMotTgt";
//constexpr char TEXT_MODELMODE[] = "Model mode";
//constexpr char TEXT_BACK[] = "Back";

//WiFiSettingsMenu
//constexpr char TEXT_WIFISETTINGS[] = "WiFi settings";
//constexpr char TEXT_GENERICWIFISETTINGS[] = "Generic WiFi settings";
//constexpr char TEXT_BACK[] = "Back";

//BoardcomputerHardwareSettingsMenu
//constexpr char TEXT_BOARDCOMPUTERHARDWARESETTINGS[] = "Boardcomputer H/W settings";
//constexpr char TEXT_LOCKSCREENSETTINGS[] = "Lockscreen Settings";
constexpr char TEXT_CALIBRATE[] = "Calibrate";
constexpr char TEXT_SAMPLECOUNT[] = "sampleCount";
constexpr char TEXT_GASMIN[] = "gasMin";
constexpr char TEXT_GASMAX[] = "gasMax";
constexpr char TEXT_BREMSMIN[] = "bremsMin";
constexpr char TEXT_BREMSMAX[] = "bremsMax";
constexpr char TEXT_DPADDEBOUNCE[] = "dpadDebounce";
constexpr char TEXT_GAMETRAKCALIBRATE[] = "Gametrak calibrate";
constexpr char TEXT_SETGAMETRAKXMIN[] = "Set gametrakXMin";
constexpr char TEXT_SETGAMETRAKXMAX[] = "Set gametrakXMax";
constexpr char TEXT_SETGAMETRAKYMIN[] = "Set gametrakYMin";
constexpr char TEXT_SETGAMETRAKYMAX[] = "Set gametrakYMax";
constexpr char TEXT_SETGAMETRAKDISTMIN[] = "Set gametrakDistMin";
constexpr char TEXT_SETGAMETRAKDISTMAX[] = "Set gametrakDistMax";
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
constexpr char TEXT_REMOTECONTROL[] = "Remote control";
constexpr char TEXT_GAMETRAK[] = "Gametrak";
//constexpr char TEXT_BACK[] = "Back";

//TimersMenu
//constexpr char TEXT_TIMERS[] = "Timers";
constexpr char TEXT_POTIREADRATE[] = "Poti read rate";
constexpr char TEXT_MODEUPDATERATE[] = "Mode update rate";
constexpr char TEXT_STATSUPDATERATE[] = "Stats update rate";
constexpr char TEXT_DISPLAYUPDATERATE[] = "Display update rate";
constexpr char TEXT_DISPLAYREDRAWRATE[] = "Display redraw rate";
#ifdef FEATURE_CAN
constexpr char TEXT_CANRECEIVERATE[] = "CAN receive rate";
#endif
//constexpr char TEXT_BACK[] = "Back";

//TimeSettingsMenu
//constexpr char TEXT_TIME[] = "Time";
constexpr char TEXT_OFFSET[] = "Offset";
constexpr char TEXT_DAYLIGHTSAVINGMODE[] = "Daylight Saving";
constexpr char TEXT_NTPENABLED[] = "NTP Enabled";
constexpr char TEXT_NTPSERVER[] = "NTP Server";
constexpr char TEXT_NTPMODE[] = "NTP Mode";
constexpr char TEXT_NTPINTERVAL[] = "NTP Interval";
//constexpr char TEXT_BACK[] = "Back";

//ChangeValueDisplay<BluetoothMode>
constexpr char TEXT_OFF[] = "Off";
constexpr char TEXT_MASTER[] = "Master";
constexpr char TEXT_SLAVE[] = "Slave";

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

//Crashmenu
constexpr char TEXT_CRASHMENU[] = "Crash Menu";
constexpr char TEXT_CRASH_ASSERT[] = "assert(0)";
constexpr char TEXT_CRASH_DIVZERO[] = "42 / 0";

//SelectBuildServerMenu
constexpr char TEXT_SELECTBUILDSERVERMENU[] = "Select Buildserver";
constexpr char TEXT_NOBUILDSERVERCONFIGURED[] = "Not configured";

//Otamenu
constexpr char TEXT_UPDATENOW[] = "Update now";
constexpr char TEXT_SELECTBUILD[] = "Select build";
constexpr char TEXT_OTA_NOBUILDSERVERAVAILABLE[] = "E:No server saved.";
constexpr char TEXT_OTA_NOBUILDSERVERSELECTED[] = "E:No server selected.";
constexpr char TEXT_OTA_NOCONNECTION[] = "E:No internet.";
constexpr char TEXT_OTA_WAITFORRESPONSE[] = "Wait for response...";
constexpr char TEXT_REDOWNLOAD[] = "Reload list";

#ifdef FEATURE_CAN
constexpr char TEXT_POWERSUPPLY[] = "Powersupply";
#endif
}
