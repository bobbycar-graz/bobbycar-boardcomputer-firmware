#include "texts.h"
namespace bobbytexts {
//AboutMenu
char TEXT_ABOUT[] = "About";
char TEXT_BACK[] = "Back";

//AccessPointWifiSettingsMenu
char TEXT_ACCESSPOINTWIFISETTINGS[] = "Access Point WiFi settings";
char TEXT_WIFIAPENABLED[] = "AP enabled";
char TEXT_RESEND_DNS[] = "Resend DNS";
//char TEXT_BACK[] = "Back";

#ifdef TEXTS_PLUGIN
#include TEXTS_PLUGIN
#endif

#ifdef FEATURE_BLUETOOTH
//BluetoothSettingsMenu
char TEXT_BLUETOOTHSETTINGS[] = "Bluetooth settings";
char TEXT_BLUETOOTHBEGIN[] = "begin()";
char TEXT_BLUETOOTHBEGINMASTER[] = "begin() Master";
char TEXT_BLUETOOTHFLUSH[] = "flush()";
char TEXT_BLUETOOTHEND[] = "end()";
char TEXT_BLUETOOTHDISCONNECT[] = "disconnect()";
char TEXT_AUTOBLUETOOTHMODE[] = "Auto bluetooth mode";
//char TEXT_BACK[] = "Back";

//BmsMenu
#ifdef FEATURE_BMS
char TEXT_BMS[] = "BMS";
char TEXT_CONNECTBMS[] = "Connect BMS";
char TEXT_DISCONNECTBMS[] = "Disonnect BMS";
char TEXT_TURNONCHARGE[] = "Turn on charge";
char TEXT_TURNOFFCHARGE[] = "Turn off charge";
char TEXT_TURNONDISCHARGE[] = "Turn on discharge";
char TEXT_TURNOFFDISCHARGE[] = "Turn off discharge";
//char TEXT_BACK[] = "Back";
#endif
#endif

#ifdef FEATURE_BLE
//BleSettingsMenu
char TEXT_BLESETTINGS[] = "BLE settings";
char TEXT_BLEENABLED[] = "BLE enabled";
//char TEXT_BACK[] = "Back";
#endif

#ifdef FEATURE_CLOUD
//CloudSettingsMenu
char TEXT_CLOUDSETTINGS[] = "Cloud settings";
char TEXT_CLOUDENABLED[] = "Tcp Cloud enabled";
char TEXT_UDPCLOUDENABLED[] = "Udp Cloud enabled";
char TEXT_CLOUDTRANSMITTIMEOUT[] = "Transmit timeout";
char TEXT_CLOUDCOLLECTRATE[] = "Cloud collect rate";
char TEXT_CLOUDSENDRATE[] = "Cloud send rate";
char TEXT_UDPSENDRATE[] = "Udp send rate";
char TEXT_UDPUSESTRING[] = "Udp use std::string";
//char TEXT_BACK[] = "Back";
#endif

//DebugMenu
char TEXT_LOADSETTINGS[] = "Load settings";
char TEXT_SAVESETTINGS[] = "Save settings";
char TEXT_ERASENVS[] = "Erase NVS";
char TEXT_FRONTCOMMAND[] = "Front command";
char TEXT_BACKCOMMAND[] = "Back command";
char TEXT_FRONTLEFTCOMMAND[] = "Front left command";
char TEXT_FRONTRIGHTCOMMAND[] = "Front right command";
char TEXT_BACKLEFTCOMMAND[] = "Back left command";
char TEXT_BACKRIGHTCOMMAND[] = "Back right command";
char TEXT_FRONTFEEDBACK[] = "Front feedback";
char TEXT_BACKFEEDBACK[] = "Back feedback";
char TEXT_FRONTLEFTFEEDBACK[] = "Front left feedback";
char TEXT_FRONTRIGHTFEEDBACK[] = "Front right feedback";
char TEXT_BACKLEFTFEEDBACK[] = "Back left feedback";
char TEXT_BACKRIGHTFEEDBACK[] = "Back right feedback";
//char TEXT_BACK[] = "Back";

//MainMenu
char TEXT_MAINMENU[] = "Main menu";
char TEXT_STATUS[] = "Status";
char TEXT_SELECTMODE[] = "Select mode";
char TEXT_MODESETTINGS[] = "Mode settings";
char TEXT_PRESETS[] = "Presets";
char TEXT_PROFILES[] = "Profiles";
char TEXT_GRAPHS[] = "Graphs";
//char TEXT_BMS[] = "BMS";
char TEXT_SETTINGS[] = "Settings";
char TEXT_LOCKVEHICLE[] = "Lock vehicle";
char TEXT_MOSFETS[] = "Mosfets";
char TEXT_DEMOS[] = "Demos";
char TEXT_GARAGE[] = "Garage";
char TEXT_UPDATE[] = "Update";
char TEXT_POWEROFF[] = "Poweroff";
char TEXT_REBOOT[] = "Reboot";
char TEXT_DEBUG[] = "Debug";
char TEXT_BATTERY[] = "Battery";
char TEXT_BATTERYDEBUG[] = "Bat Debug Menu";
char TEXT_TOGGLECLOUDDEBUG[] = "Cloud Debug";

//BatteryMenu
char TEXT_CELL_SERIES[] = "Cells (Series)";
char TEXT_CELL_PARALLEL[] = "Cells (Parallel)";
char TEXT_SELECT_CELL_TYPE[] = "Select Cell Type";
char TEXT_CELL_TYPE[] = "Cell Type";
char TEXT_BATTERY_CALIBRATE[] = "Calibrate Voltages";
char TEXT_BATTERY_TYPE_22P[] = "22P cells";
char TEXT_BATTERY_TYPE_HG2[] = "HG2 cells";
char TEXT_BATTERY_TYPE_MH1[] = "MH1 cells";
char TEXT_BATTERY_TYPE_VTC5[] = "VTC5 cells";
char TEXT_BATTERY_TYPE_BAK_25R[] = "BAK / 25R cells";
char TEXT_BATTERY_WHKM[] = "Wh per km";
char TEXT_BATTERY_APPLYCALIB[] = "Apply calibration";
char TEXT_VOLTAGECALIBRATION_RESET[] = "Reset calibration";

//CalibrateVoltageDisplay
char TEXT_VOLTAGECALIBRATION_30V[] = "Calibrate 30.0V";
char TEXT_VOLTAGECALIBRATION_50V[] = "Calibrate 50.0V";
char TEXT_VOLTAGECALIBRATION_VALUE_30_FRONT[] = "30V Front";
char TEXT_VOLTAGECALIBRATION_VALUE_30_BACK[] = "30V Back";
char TEXT_VOLTAGECALIBRATION_VALUE_50_FRONT[] = "50V Front";
char TEXT_VOLTAGECALIBRATION_VALUE_50_BACK[] = "50V Back";

//SettingsMenu
//char TEXT_SETTINGS[] = "Settings";
char TEXT_BACKLIGHT[] = "Backlight";
char TEXT_LIMITSSETTINGS[] = "Limits settings";
char TEXT_WIFISETTINGS[] = "WiFi settings";
//char TEXT_BLUETOOTHSETTINGS[] = "Bluetooth settings";
//char TEXT_BLESETTINGS[] = "BLE settings";
//char TEXT_CLOUDSETTINGS[] = "Cloud settings";
char TEXT_TIME[] = "Time";
char TEXT_MODESSETTINGS[] = "Modes settings";
char TEXT_CONTROLLERHARDWARESETTINGS[] = "Controller H/W settings";
char TEXT_BOARDCOMPUTERHARDWARESETTINGS[] = "Boardcomputer H/W settings";
char TEXT_AUTOCONNECTBMS[] = "Auto connect BMS";
char TEXT_BUZZER[] = "Buzzer";
char TEXT_FRONTLED[] = "Front LED";
char TEXT_BACKLED[] = "Back LED";
//char TEXT_ABOUT[] = "About";
//char TEXT_BACK[] = "Back";

//ControllerHardwareSettings
//char TEXT_CONTROLLERHARDWARESETTINGS[] = "Controller H/W settings";
char TEXT_WHEELDIAMETERMM[] = "Wheel diameter (mm)";
char TEXT_WHEELDIAMETERINCH[] = "Wheel diameter (inch)";
char TEXT_NUMMAGNETPOLES[] = "Num magnet poles";
char TEXT_SETENABLED[] = "Set enabled";
char TEXT_SETINVERTED[] = "Set inverted";
char TEXT_SWAPFRONTBACK[] = "Swap front/back";
#ifdef FEATURE_CAN
char TEXT_FRONTSENDCAN[] = "Front send CAN";
char TEXT_BACKSENDCAN[] = "Back send CAN";
char TEXT_CANTRANSMITTIMEOUT[] = "CanTransmitTimeout";
char TEXT_CANRECEIVETIMEOUT[] = "CanReceiveTimeout";
#endif
//char TEXT_BACK[] = "Back";

//StationWifiSettingsMenu
char TEXT_STATIONWIFISETTINGS[] = "Station WiFi settings";
char TEXT_WIFISTAENABLED[] = "STA enabled";
char TEXT_WIFITRIGGERSCAN[] = "Trigger Scan";
char TEXT_WIFISCANRESULTS[] = "WiFi scan results";
//char TEXT_BACK[] = "Back";

//DemosMenu
//char TEXT_DEMOS[] = "Demos";
char TEXT_STARFIELD[] = "Starfield";
char TEXT_PINGPONG[] = "PingPong";
char TEXT_SPIRO[] = "Spiro";
char TEXT_GAMEOFLIFE[] = "GameOfLife";
//char TEXT_BACK[] = "Back";

//BuzzerMenu
//char TEXT_BUZZER[] = "Buzzer";
char TEXT_FRONTFREQ[] = "Front freq";
char TEXT_FRONTPATTERN[] = "Front pattern";
char TEXT_BACKFREQ[] = "Back freq";
char TEXT_BACKPATTERN[] = "Back pattern";
char TEXT_REVERSEBEEP[] = "Reverse beep";
char TEXT_REVERSEBEEPFREQ0[] = "Reverse beep freq0";
char TEXT_REVERSEBEEPFREQ1[] = "Reverse beep freq1";
char TEXT_REVERSEBEEPDURATION0[] = "Reverse beep duration0";
char TEXT_REVERSEBEEPDURATION1[] = "Reverse beep duration1";
//char TEXT_BACK[] = "Back";

//LimitsSettingsMenu
//char TEXT_LIMITSSETTINGS[] = "Limit settings";
char TEXT_IMOTMAX[] = "iMotMax";
char TEXT_IDCMAX[] = "iDcMax";
char TEXT_NMOTMAXKMH[] = "nMotMaxKmh";
char TEXT_NMOTMAX[] = "nMotMax";
char TEXT_FIELDWEAKMAX[] = "fldWkMax";
char TEXT_PHASEADVMAX[] = "phsAdvMax";
//char TEXT_BACK[] = "Back";

//DebugMenu
//char TEXT_DEBUG[] = "Debug";
char TEXT_DYNAMICMENU[] = "Dynamic menu";
//char TEXT_BACK[] = "Back";

//DefaultModeSettingsMenu
//char TEXT_DEFAULTMODESETTINGS[] = "Default mode settings";
char TEXT_MODELMODE[] = "Model mode";
char TEXT_HYBRIDMODE[] = "Hybrid mode";
char TEXT_SQUAREGAS[] = "Square gas";
char TEXT_SQUAREBREMS[] = "Square brems";
char TEXT_ENABLESMOOTHINGUP[] = "Enable up smoothing";
char TEXT_ENABLESMOOTHINGDOWN[] = "Enable down smoothing";
char TEXT_ENABLEFWSMOOTHINGUP[] = "Enable fw up smoothing";
char TEXT_ENABLEFWSMOOTHINGDOWN[] = "Enable fw down smoothing";
char TEXT_FWSMOOTHING_LIMIT[] = "Fw smooth lower limit";
char TEXT_SMOOTHINGVAL[] = "Smoothing";
char TEXT_FRONTPERCENTAGE[] = "Front %";
char TEXT_BACKPERCENTAGE[] = "Back %";
char TEXT_ADDSCHWELLE[] = "Add/Sub Lim";
char TEXT_SUBGASVAL[] = "Add Gas";
char TEXT_SUBBRAKEVAL[] = "Add Brake";
char TEXT_ADDGASVAL[] = "Sub Gas";
char TEXT_ADDBRAKEVAL[] = "Sub Brake";
char TEXT_HYBRIDENABLE[] = "Enable Hybrid mode";
char TEXT_HYBRIDACTIVATIONLIMIT[] = "Hybrid activation-limit";
char TEXT_HYBRIDDEACTIVATIONLIMIT[] = "Hybrid deactivation-limit";
char TEXT_LIMITS_TO_NEAR[] = "Hybrid limits too near (>20)";

char TEXT_HANDBREMSE_ENABLE[] = "Enable Handbrems";
char TEXT_HANDBREMSE_AUTOMATIC[] = "Automatic Handbrems";
char TEXT_HANDBREMSE_MODE[] = "Handbrems Mode";
char TEXT_HANDBREMSE_TRIGGERTIMEOUT[] = "Handbrems Timeout";
//char TEXT_BACK[] = "Back";

//DynamicDebugMenu
//char TEXT_DYNAMICMENU[] = "Dynamic menu";
//char TEXT_BACK[] = "Back";

//EnableMenu
//char TEXT_SETENABLED[] = "Set enabled";
char TEXT_ENABLEFRONTLEFT[] = "Enable front left";
char TEXT_ENABLEFRONTRIGHT[] = "Enable front right";
char TEXT_ENABLEBACKLEFT[] = "Enable back left";
char TEXT_ENABLEBACKRIGHT[] = "Enable back right";
//char TEXT_BACK[] = "Back";

//GametrakModeSettingsMenu
char TEXT_GAMETRAKMODESETTINGS[] = "Gametrak mode settings";

//GenericWifiSettingsMenu
char TEXT_GENERICWIFISETTINGS[] = "Generic WiFi settings";
//char TEXT_BACK[] = "Back";

//GraphsMenu
//char TEXT_GRAPHS[] = "Graphs";
char TEXT_GAS[] = "Gas";
char TEXT_BREMS[] = "Brems";
char TEXT_POTIS[] = "Potis";
char TEXT_AVGSPEED[] = "Avg. speed";
char TEXT_AVGSPEEDKMH[] = "Avg. speed KMH";
char TEXT_SUMCURRENT[] = "Sum current";
char TEXT_FRONTVOLTAGE[] = "Front voltage";
char TEXT_BACKVOLTAGE[] = "Back voltage";
char TEXT_VOLTAGES[] = "Voltages";
char TEXT_BMSVOLTAGE[] = "BMS voltage";
char TEXT_BMSCURRENT[] = "BMS current";
char TEXT_BMSPOWER[] = "BMS power";
char TEXT_SUMCURRENTSCOMPARISON[] = "Sum currents comparison";
char TEXT_MOTORCURRENTS[] = "Motor currents";
char TEXT_RSSI[] = "RSSI";
//char TEXT_BACK[] = "Back";

//InvertMenu
//char TEXT_SETINVERTED[] = "Set inverted";
char TEXT_INVERTFRONTLEFT[] = "Invert front left";
char TEXT_INVERTFRONTRIGHT[] = "Invert front right";
char TEXT_INVERTBACKLEFT[] = "Invert back left";
char TEXT_INVERTBACKRIGHT[] = "Invert back right";
//char TEXT_BACK[] = "Back";

//LarsmModeSettingsMenu
char TEXT_LARSMMODESETTINGS[] = "Larsm mode settings";
//char TEXT_MODELMODE[] = "Model mode";
char TEXT_SETMODE[] = "Set mode";
char TEXT_SETITERATIONS[] = "Set iterations";
//char TEXT_BACK[] = "Back";

//MotortestModeSettingsMenu
char TEXT_MOTORTESTMODESETTINGS[] = "Motortest mode seetings";
char TEXT_MOTORTESTMAXPWM[] = "Max Pwm";
char TEXT_MOTORTESTMULTIPLIKATOR[] = "Acceleration";

//LedstripMenu
char TEXT_LEDSTRIP[] = "Ledstrip";
char TEXT_LEDANIMATION[] = "LED Animation";
char TEXT_BRAKELIGHTS[] = "Brake Lights";
char TEXT_BLINKANIMATION[] = "Blink animation";
char TEXT_ANIMATION_TYPE[] = "Blink animation";
char TEXT_LEDSCOUNT[] = "LEDs Count";
char TEXT_CENTEROFFSET[] = "Center Offset";
char TEXT_SMALLOFFSET[] = "Small Offset";
char TEXT_BIGOFFSET[] = "Big Offset";
char TEXT_LEDSTRIP_MILLIAMP[] = "Ledstrip 0.1A";
char TEXT_BLINKBEEP[] = "Blink Beep";
char TEXT_FULLBLINK[] = "Full blink";
char TEXT_LEDSTRIP_STVO[] = "Enable StVO";
char TEXT_STVO_FRONTOFFSET[] = "StVO Front Offset";
char TEXT_STVO_FRONTLENGTH[] = "StVO Front Length";
char TEXT_STVO_ENABLEFRONTLIGHT[] = "StVO Front Enable";
char TEXT_ANIMATION_MULTIPLIER[] = "Animation Multiplier";
char TEXT_LEDSTRIP_BRIGHTNESS[] = "Ledstrip Brightness";
char TEXT_LEDSTRIP_ALLCUSTOMOFF[] = "All custom off";
char TEXT_LEDSTRIP_EN_BLINK_ANIM[] = "Animated Blink";
char TEXT_LEDSTRIP_CHANGE_OTA_ANIM[] = "Change Ota animation";

char TEXT_OTAANIM_NONE[] = "None";
char TEXT_OTAANIM_PROGRESS[] = "Progress Bar";
char TEXT_OTAANIM_COLOR[] = "Color change";
//char TEXT_BACK[] = "Back";

//LedstripSelectAnimationMenu
char TEXT_SELECTANIMATION[] = "Select Animation";
char TEXT_ANIMATION_DEFAULTRAINBOW[] = "Default Rainbow";
char TEXT_ANIMATION_BETTERRAINBOW[] = "Better Rainbow";
char TEXT_ANIMATION_SPEEDSYNCANIMATION[] = "Speed Sync";
char TEXT_ANIMATION_CUSTOMCOLOR[] = "Custom Color";

//LedstripSelectBlinkMenu
char TEXT_ANIMATION_BLINKNONE[] = "Blink Off";
char TEXT_ANIMATION_BLINKLEFT[] = "Blink Left";
char TEXT_ANIMATION_BLINKRIGHT[] = "Blink Right";
char TEXT_ANIMATION_BLINKBOTH[] = "Blink Both";

//LockscreenSettingsMenu
char TEXT_LOCKSCREENSETTINGS[] = "Lockscreen Settings";
char TEXT_ALLOWPRESETSWITCH[] = "Allow preset switch";
char TEXT_PINDIGIT0[] = "PIN digit0";
char TEXT_PINDIGIT1[] = "PIN digit1";
char TEXT_PINDIGIT2[] = "PIN digit2";
char TEXT_PINDIGIT3[] = "PIN digit3";
//char TEXT_BACK[] = "Back";

//ModesSettingsMenu
//char TEXT_MODESSETTINGS[] = "Modes settings";
char TEXT_DEFAULTMODESETTIGNS[] = "Default mode settings";
char TEXT_TEMPOMATMODESETTINGS[] = "Tempomat mode settings";
//char TEXT_LARSMMODESETTINGS[] = "Larsm mode settings";
//char TEXT_GAMETRAKMODESETTINGS[] = "Gametrak mode settings";
//char TEXT_BACK[] = "Back";

//MosfetsMenu
//char TEXT_MOSFETS[] = "Mosfets";
char TEXT_MOSFET0[] = "Mosfet0";
char TEXT_MOSFET1[] = "Mosfet1";
char TEXT_MOSFET2[] = "Mosfet2";

//TempomatModeSettingsMenu
//char TEXT_TEMPOMATMODESETTINGS[] = "Tempomat mode settings";
char TEXT_APPLY[] = "Apply";
char TEXT_NCRUISEMOTTGT[] = "nCruiseMotTgt";
//char TEXT_MODELMODE[] = "Model mode";
//char TEXT_BACK[] = "Back";

//WiFiSettingsMenu
//char TEXT_WIFISETTINGS[] = "WiFi settings";
//char TEXT_GENERICWIFISETTINGS[] = "Generic WiFi settings";
//char TEXT_BACK[] = "Back";

//BoardcomputerHardwareSettingsMenu
//char TEXT_BOARDCOMPUTERHARDWARESETTINGS[] = "Boardcomputer H/W settings";
//char TEXT_LOCKSCREENSETTINGS[] = "Lockscreen Settings";
char TEXT_CALIBRATE[] = "Calibrate";
char TEXT_SAMPLECOUNT[] = "sampleCount";
char TEXT_GASMIN[] = "gasMin";
char TEXT_GASMAX[] = "gasMax";
char TEXT_BREMSMIN[] = "bremsMin";
char TEXT_BREMSMAX[] = "bremsMax";
char TEXT_DPADDEBOUNCE[] = "dpadDebounce";
char TEXT_GAMETRAKCALIBRATE[] = "Gametrak calibrate";
char TEXT_SETGAMETRAKXMIN[] = "Set gametrakXMin";
char TEXT_SETGAMETRAKXMAX[] = "Set gametrakXMax";
char TEXT_SETGAMETRAKYMIN[] = "Set gametrakYMin";
char TEXT_SETGAMETRAKYMAX[] = "Set gametrakYMax";
char TEXT_SETGAMETRAKDISTMIN[] = "Set gametrakDistMin";
char TEXT_SETGAMETRAKDISTMAX[] = "Set gametrakDistMax";
char TEXT_TIMERS[] = "Timers";
//char TEXT_BACK[] = "Back";

//ProfilesMenu
//char TEXT_PROFILES[] = "Profiles";
char TEXT_PROFILE0[] = "Profile 0";
char TEXT_PROFILE1[] = "Profile 1";
char TEXT_PROFILE2[] = "Profile 2";
char TEXT_PROFILE3[] = "Profile 3";
//char TEXT_BACK[] = "Back";

//PresetsMenu
//char TEXT_PRESETS[] = "Presets";
char TEXT_DEFAULTEVERYTHING[] = "Default everything";
char TEXT_DEFAULTLIMITS[] = "Default limits";
char TEXT_KIDSLIMITS[] = "Kids limits";
char TEXT_DEFAULTPOTI[] = "Default poti";
char TEXT_DEFAULTCONTROLLERHARDWARE[] = "Default controller H/W";
char TEXT_MOSFETSOFFCONTROLLERHARDWARE[] = "MOSFETs off controller H/W";
char TEXT_SPINNERCONTROLLERHARDWARE[] = "Spinner controller H/W";
char TEXT_DEFAULTBOARDCOMPUTERHARDWARE[] = "Default boardcomputer H/W";
char TEXT_DEFAULTDEFAULTMODE[] = "Default defaultMode";
char TEXT_SINUSOIDALDEFAULTMODE[] = "Sinusoidal defaultMode";
char TEXT_DEFAULTTEMPOMATMODE[] = "Default tempomatMode";
char TEXT_DEFAULTLARSMMODE[] = "Default larsmMode";
char TEXT_STREET[] = "Street";
char TEXT_SIDEWALK[] = "Sidewalk";
char TEXT_POLICE[] = "Police";
char TEXT_RACE[] = "Race";

//SelectModeMenu
//char TEXT_SELECTMODE[] = "Select mode";
char TEXT_DEFAULT[] = "Default";
char TEXT_TEMPOMAT[] = "Tempomat";
char TEXT_LARSM[] = "Larsm";
char TEXT_REMOTECONTROL[] = "Remote control";
char TEXT_GAMETRAK[] = "Gametrak";
char TEXT_MOTORTEST[] = "Motortest";
//char TEXT_BACK[] = "Back";

//TimersMenu
//char TEXT_TIMERS[] = "Timers";
char TEXT_POTIREADRATE[] = "Poti read rate";
char TEXT_MODEUPDATERATE[] = "Mode update rate";
char TEXT_STATSUPDATERATE[] = "Stats update rate";
char TEXT_DISPLAYUPDATERATE[] = "Display update rate";
char TEXT_DISPLAYREDRAWRATE[] = "Display redraw rate";
#ifdef FEATURE_CAN
char TEXT_CANRECEIVERATE[] = "CAN receive rate";
#endif
//char TEXT_BACK[] = "Back";

//TimeSettingsMenu
//char TEXT_TIME[] = "Time";
char TEXT_OFFSET[] = "Offset";
char TEXT_DAYLIGHTSAVINGMODE[] = "Daylight Saving";
char TEXT_NTPENABLED[] = "NTP Enabled";
char TEXT_NTPSERVER[] = "NTP Server";
char TEXT_NTPMODE[] = "NTP Mode";
char TEXT_NTPINTERVAL[] = "NTP Interval";
//char TEXT_BACK[] = "Back";

//ChangeValueDisplay<BluetoothMode>
char TEXT_OFF[] = "Off";
char TEXT_MASTER[] = "Master";
char TEXT_SLAVE[] = "Slave";

//ChangeValueDisplay<ControlMode>
char TEXT_OPENMODE[] = "Open mode";
char TEXT_VOLTAGE[] = "Voltage";
char TEXT_SPEED[] = "Speed";
char TEXT_TORQUE[] = "Torque";
//char TEXT_BACK[] = "Back";

//ChangeValueDisplay<ControlType>
char TEXT_COMMUTATION[] = "Commutation";
char TEXT_SINUSOIDAL[] = "Sinusoidal";
char TEXT_FIELDORIENTEDCONTROL[] = "Field oriented control";
//char TEXT_BACK[] = "Back";

//ChangeValueDisplay<HandbremseMode>
char TEXT_HANDBREMS_MOSFETS_OFF[] = "Mosfets off";
char TEXT_HANDBREMS_OPENMODE[] = "Open Mode";
char TEXT_HANDBREMS_SPEED0[] = "Speed 0";
char TEXT_HANDBREMSE[] = "Handbremse";

//ChangeValueDisplay<LarsmMode::Mode>
char TEXT_LARSMMODE1[] = "Mode1";
char TEXT_LARSMMODE2[] = "Mode2";
char TEXT_LARSMMODE3[] = "Mode3";
char TEXT_LARSMMODE4[] = "Mode4";
//char TEXT_BACK[] = "Back";

//ChangeValueDisplay<UnifiedModelMode>
//char TEXT_COMMUTATION[] = "Commutation";
//char TEXT_SINUSOIDAL[] = "Sinusoidal";
char TEXT_FOCVOLTAGE[] = "FOC/Voltage";
char TEXT_FOCSPEED[] = "FOC/Speed";
char TEXT_FOCTORQUE[] = "FOC/Torque";
//char TEXT_BACK[] = "Back";

//ChangeValueDisplay<wifi_mode_t>
char TEXT_WIFI_MODE_NULL[] = "WIFI_MODE_NULL";
char TEXT_WIFI_MODE_STA[] = "WIFI_MODE_STA";
char TEXT_WIFI_MODE_AP[] = "WIFI_MODE_AP";
char TEXT_WIFI_MODE_APSTA[] = "WIFI_MODE_APSTA";
//char TEXT_BACK[] = "Back";

//ChangeValueDisplay<wifi_power_t>
char TEXT_WIFI_POWER_19_5dBm[] = "19.5dBm";
char TEXT_WIFI_POWER_19dBm[] = "19dBm";
char TEXT_WIFI_POWER_18_5dBm[] = "18.5dBm";
char TEXT_WIFI_POWER_17dBm[] = "17dBm";
char TEXT_WIFI_POWER_15dBm[] = "15dBm";
char TEXT_WIFI_POWER_13dBm[] = "13dBm";
char TEXT_WIFI_POWER_11dBm[] = "11dBm";
char TEXT_WIFI_POWER_8_5dBm[] = "8.5dBm";
char TEXT_WIFI_POWER_7dBm[] = "7dBm";
char TEXT_WIFI_POWER_5dBm[] = "5dBm";
char TEXT_WIFI_POWER_2dBm[] = "2dBm";
char TEXT_WIFI_POWER_MINUS_1dBm[] = "-1dBm";
//char TEXT_BACK[] = "Back";

//Crashmenu
char TEXT_CRASHMENU[] = "Crash Menu";
char TEXT_CRASH_ASSERT[] = "assert(0)";
char TEXT_CRASH_DIVZERO[] = "42 / 0";

//SelectBuildServerMenu
char TEXT_SELECTBUILDSERVERMENU[] = "Select Buildserver";
char TEXT_NOBUILDSERVERCONFIGURED[] = "Not configured";

//Otamenu
char TEXT_UPDATENOW[] = "Update now";
char TEXT_SELECTBUILD[] = "Select build";
char TEXT_OTA_NOBUILDSERVERAVAILABLE[] = "E:No server saved.";
char TEXT_OTA_NOBUILDSERVERSELECTED[] = "E:No server selected.";
char TEXT_OTA_NOCONNECTION[] = "E:No internet.";
char TEXT_OTA_WAITFORRESPONSE[] = "Wait for response...";

//LedstripColorMenu
char TEXT_LEDSTRIPCOLORMENU[] = "Customize Ledstrip";

//StatisticsMenu
char TEXT_STATISTICSMENU[] = "Statistics";
char TEXT_STATSSAVE[] = "Save kilometers";
char TEXT_STATSCLEAR[] = "Clear current km";

#ifdef FEATURE_CAN
char TEXT_POWERSUPPLY[] = "Powersupply";
#endif
char TEXT_REENABLE_MENUITEMS[] = "Show advanced";
} // namespace
