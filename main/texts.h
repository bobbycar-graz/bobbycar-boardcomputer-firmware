namespace bobbytexts {
//AboutMenu
extern char TEXT_ABOUT[];
extern char TEXT_BACK[];

//AccessPointWifiSettingsMenu
extern char TEXT_ACCESSPOINTWIFISETTINGS[];
extern char TEXT_WIFIAPENABLED[];
extern char TEXT_RESEND_DNS[];
//extern char TEXT_BACK[];

#ifdef TEXTS_PLUGIN
#include TEXTS_PLUGIN
#endif

#ifdef FEATURE_BLUETOOTH
//BluetoothSettingsMenu
extern char TEXT_BLUETOOTHSETTINGS[];
extern char TEXT_BLUETOOTHBEGIN[];
extern char TEXT_BLUETOOTHBEGINMASTER[];
extern char TEXT_BLUETOOTHFLUSH[];
extern char TEXT_BLUETOOTHEND[];
extern char TEXT_BLUETOOTHDISCONNECT[];
extern char TEXT_AUTOBLUETOOTHMODE[];
//extern char TEXT_BACK[];

//BmsMenu
#ifdef FEATURE_BMS
extern char TEXT_BMS[];
extern char TEXT_CONNECTBMS[];
extern char TEXT_DISCONNECTBMS[];
extern char TEXT_TURNONCHARGE[];
extern char TEXT_TURNOFFCHARGE[];
extern char TEXT_TURNONDISCHARGE[];
extern char TEXT_TURNOFFDISCHARGE[];
//extern char TEXT_BACK[];
#endif
#endif

#ifdef FEATURE_BLE
//BleSettingsMenu
extern char TEXT_BLESETTINGS[];
extern char TEXT_BLEENABLED[];
//extern char TEXT_BACK[];
#endif

#ifdef FEATURE_CLOUD
//CloudSettingsMenu
extern char TEXT_CLOUDSETTINGS[];
extern char TEXT_CLOUDENABLED[];
extern char TEXT_CLOUDTRANSMITTIMEOUT[];
extern char TEXT_CLOUDCOLLECTRATE[];
extern char TEXT_CLOUDSENDRATE[];
//extern char TEXT_BACK[];
#endif

#ifdef FEATURE_UDPCLOUD
//UdpCloudSettingsMenu
extern char TEXT_UDPCLOUDSETTINGS[];
extern char TEXT_UDPCLOUDENABLED[];
extern char TEXT_UDPSENDRATE[];
extern char TEXT_UDPUSESTRING[];
//extern char TEXT_BACK[];
#endif

//DebugMenu
extern char TEXT_LOADSETTINGS[];
extern char TEXT_SAVESETTINGS[];
extern char TEXT_ERASENVS[];
extern char TEXT_FRONTCOMMAND[];
extern char TEXT_BACKCOMMAND[];
extern char TEXT_FRONTLEFTCOMMAND[];
extern char TEXT_FRONTRIGHTCOMMAND[];
extern char TEXT_BACKLEFTCOMMAND[];
extern char TEXT_BACKRIGHTCOMMAND[];
extern char TEXT_FRONTFEEDBACK[];
extern char TEXT_BACKFEEDBACK[];
extern char TEXT_FRONTLEFTFEEDBACK[];
extern char TEXT_FRONTRIGHTFEEDBACK[];
extern char TEXT_BACKLEFTFEEDBACK[];
extern char TEXT_BACKRIGHTFEEDBACK[];
//extern char TEXT_BACK[];

//MainMenu
extern char TEXT_MAINMENU[];
extern char TEXT_STATUS[];
extern char TEXT_SELECTMODE[];
extern char TEXT_MODESETTINGS[];
extern char TEXT_PRESETS[];
extern char TEXT_PROFILES[];
extern char TEXT_GRAPHS[];
//extern char TEXT_BMS[];
extern char TEXT_SETTINGS[];
extern char TEXT_LOCKVEHICLE[];
extern char TEXT_MOSFETS[];
extern char TEXT_DEMOS[];
extern char TEXT_GARAGE[];
extern char TEXT_UPDATE[];
extern char TEXT_POWEROFF[];
extern char TEXT_REBOOT[];
extern char TEXT_DEBUG[];
extern char TEXT_BATTERY[];
extern char TEXT_BATTERYDEBUG[];
extern char TEXT_TOGGLECLOUDDEBUG[];

//BatteryMenu
extern char TEXT_CELL_SERIES[];
extern char TEXT_CELL_PARALLEL[];
extern char TEXT_SELECT_CELL_TYPE[];
extern char TEXT_CELL_TYPE[];
extern char TEXT_BATTERY_CALIBRATE[];
extern char TEXT_BATTERY_TYPE_22P[];
extern char TEXT_BATTERY_TYPE_HG2[];
extern char TEXT_BATTERY_TYPE_MH1[];
extern char TEXT_BATTERY_TYPE_VTC5[];
extern char TEXT_BATTERY_TYPE_BAK_25R[];
extern char TEXT_BATTERY_WHKM[];
extern char TEXT_BATTERY_APPLYCALIB[];
extern char TEXT_VOLTAGECALIBRATION_RESET[];

//CalibrateVoltageDisplay
extern char TEXT_VOLTAGECALIBRATION_30V[];
extern char TEXT_VOLTAGECALIBRATION_50V[];
extern char TEXT_VOLTAGECALIBRATION_VALUE_30_FRONT[];
extern char TEXT_VOLTAGECALIBRATION_VALUE_30_BACK[];
extern char TEXT_VOLTAGECALIBRATION_VALUE_50_FRONT[];
extern char TEXT_VOLTAGECALIBRATION_VALUE_50_BACK[];

//SettingsMenu
//extern char TEXT_SETTINGS[];
extern char TEXT_BACKLIGHT[];
extern char TEXT_LIMITSSETTINGS[];
extern char TEXT_WIFISETTINGS[];
//extern char TEXT_BLUETOOTHSETTINGS[];
//extern char TEXT_BLESETTINGS[];
//extern char TEXT_CLOUDSETTINGS[];
extern char TEXT_TIME[];
extern char TEXT_MODESSETTINGS[];
extern char TEXT_CONTROLLERHARDWARESETTINGS[];
extern char TEXT_BOARDCOMPUTERHARDWARESETTINGS[];
extern char TEXT_AUTOCONNECTBMS[];
extern char TEXT_BUZZER[];
extern char TEXT_FRONTLED[];
extern char TEXT_BACKLED[];
//extern char TEXT_ABOUT[];
//extern char TEXT_BACK[];

//ControllerHardwareSettings
//extern char TEXT_CONTROLLERHARDWARESETTINGS[];
extern char TEXT_WHEELDIAMETERMM[];
extern char TEXT_WHEELDIAMETERINCH[];
extern char TEXT_NUMMAGNETPOLES[];
extern char TEXT_SETENABLED[];
extern char TEXT_SETINVERTED[];
extern char TEXT_SWAPFRONTBACK[];
#ifdef FEATURE_CAN
extern char TEXT_FRONTSENDCAN[];
extern char TEXT_BACKSENDCAN[];
extern char TEXT_CANTRANSMITTIMEOUT[];
extern char TEXT_CANRECEIVETIMEOUT[];
#endif
//extern char TEXT_BACK[];

//StationWifiSettingsMenu
extern char TEXT_STATIONWIFISETTINGS[];
extern char TEXT_WIFISTAENABLED[];
extern char TEXT_WIFITRIGGERSCAN[];
extern char TEXT_WIFISCANRESULTS[];
//extern char TEXT_BACK[];

//DemosMenu
//extern char TEXT_DEMOS[];
extern char TEXT_STARFIELD[];
extern char TEXT_PINGPONG[];
extern char TEXT_SPIRO[];
extern char TEXT_GAMEOFLIFE[];
//extern char TEXT_BACK[];

//BuzzerMenu
//extern char TEXT_BUZZER[];
extern char TEXT_FRONTFREQ[];
extern char TEXT_FRONTPATTERN[];
extern char TEXT_BACKFREQ[];
extern char TEXT_BACKPATTERN[];
extern char TEXT_REVERSEBEEP[];
extern char TEXT_REVERSEBEEPFREQ0[];
extern char TEXT_REVERSEBEEPFREQ1[];
extern char TEXT_REVERSEBEEPDURATION0[];
extern char TEXT_REVERSEBEEPDURATION1[];
//extern char TEXT_BACK[];

//LimitsSettingsMenu
//extern char TEXT_LIMITSSETTINGS[];
extern char TEXT_IMOTMAX[];
extern char TEXT_IDCMAX[];
extern char TEXT_NMOTMAXKMH[];
extern char TEXT_NMOTMAX[];
extern char TEXT_FIELDWEAKMAX[];
extern char TEXT_PHASEADVMAX[];
//extern char TEXT_BACK[];

//DebugMenu
//extern char TEXT_DEBUG[];
extern char TEXT_DYNAMICMENU[];
//extern char TEXT_BACK[];

//DefaultModeSettingsMenu
//extern char TEXT_DEFAULTMODESETTINGS[];
extern char TEXT_MODELMODE[];
extern char TEXT_HYBRIDMODE[];
extern char TEXT_SQUAREGAS[];
extern char TEXT_SQUAREBREMS[];
extern char TEXT_ENABLESMOOTHINGUP[];
extern char TEXT_ENABLESMOOTHINGDOWN[];
extern char TEXT_ENABLEFWSMOOTHINGUP[];
extern char TEXT_ENABLEFWSMOOTHINGDOWN[];
extern char TEXT_FWSMOOTHING_LIMIT[];
extern char TEXT_SMOOTHINGVAL[];
extern char TEXT_FRONTPERCENTAGE[];
extern char TEXT_BACKPERCENTAGE[];
extern char TEXT_ADDSCHWELLE[];
extern char TEXT_SUBGASVAL[];
extern char TEXT_SUBBRAKEVAL[];
extern char TEXT_ADDGASVAL[];
extern char TEXT_ADDBRAKEVAL[];
extern char TEXT_HYBRIDENABLE[];
extern char TEXT_HYBRIDACTIVATIONLIMIT[];
extern char TEXT_HYBRIDDEACTIVATIONLIMIT[];
extern char TEXT_LIMITS_TO_NEAR[];

extern char TEXT_HANDBREMSE_ENABLE[];
extern char TEXT_HANDBREMSE_AUTOMATIC[];
extern char TEXT_HANDBREMSE_VISUALIZE[];
extern char TEXT_HANDBREMSE_MODE[];
extern char TEXT_HANDBREMSE_TRIGGERTIMEOUT[];
extern char TEXT_HANDBREMSE[];
//extern char TEXT_BACK[];

//DynamicDebugMenu
//extern char TEXT_DYNAMICMENU[];
//extern char TEXT_BACK[];

//EnableMenu
//extern char TEXT_SETENABLED[];
extern char TEXT_ENABLEFRONTLEFT[];
extern char TEXT_ENABLEFRONTRIGHT[];
extern char TEXT_ENABLEBACKLEFT[];
extern char TEXT_ENABLEBACKRIGHT[];
//extern char TEXT_BACK[];

//GametrakModeSettingsMenu
extern char TEXT_GAMETRAKMODESETTINGS[];

//GenericWifiSettingsMenu
extern char TEXT_GENERICWIFISETTINGS[];
//extern char TEXT_BACK[];

//GraphsMenu
//extern char TEXT_GRAPHS[];
extern char TEXT_GAS[];
extern char TEXT_BREMS[];
extern char TEXT_POTIS[];
extern char TEXT_AVGSPEED[];
extern char TEXT_AVGSPEEDKMH[];
extern char TEXT_SUMCURRENT[];
extern char TEXT_FRONTVOLTAGE[];
extern char TEXT_BACKVOLTAGE[];
extern char TEXT_VOLTAGES[];
extern char TEXT_BMSVOLTAGE[];
extern char TEXT_BMSCURRENT[];
extern char TEXT_BMSPOWER[];
extern char TEXT_SUMCURRENTSCOMPARISON[];
extern char TEXT_MOTORCURRENTS[];
extern char TEXT_RSSI[];
//extern char TEXT_BACK[];

//InvertMenu
//extern char TEXT_SETINVERTED[];
extern char TEXT_INVERTFRONTLEFT[];
extern char TEXT_INVERTFRONTRIGHT[];
extern char TEXT_INVERTBACKLEFT[];
extern char TEXT_INVERTBACKRIGHT[];
//extern char TEXT_BACK[];

//LarsmModeSettingsMenu
extern char TEXT_LARSMMODESETTINGS[];
//extern char TEXT_MODELMODE[];
extern char TEXT_SETMODE[];
extern char TEXT_SETITERATIONS[];
//extern char TEXT_BACK[];

//MotortestModeSettingsMenu
extern char TEXT_MOTORTESTMODESETTINGS[];
extern char TEXT_MOTORTESTMAXPWM[];
extern char TEXT_MOTORTESTMULTIPLIKATOR[];

//LedstripMenu
extern char TEXT_LEDSTRIP[];
extern char TEXT_LEDANIMATION[];
extern char TEXT_BRAKELIGHTS[];
extern char TEXT_BLINKANIMATION[];
extern char TEXT_ANIMATION_TYPE[];
extern char TEXT_LEDSCOUNT[];
extern char TEXT_CENTEROFFSET[];
extern char TEXT_SMALLOFFSET[];
extern char TEXT_BIGOFFSET[];
extern char TEXT_LEDSTRIP_MILLIAMP[];
extern char TEXT_BLINKBEEP[];
extern char TEXT_FULLBLINK[];
extern char TEXT_LEDSTRIP_STVO[];
extern char TEXT_STVO_FRONTOFFSET[];
extern char TEXT_STVO_FRONTLENGTH[];
extern char TEXT_STVO_ENABLEFRONTLIGHT[];
extern char TEXT_ANIMATION_MULTIPLIER[];
extern char TEXT_LEDSTRIP_BRIGHTNESS[];
extern char TEXT_LEDSTRIP_ALLCUSTOMOFF[];
extern char TEXT_LEDSTRIP_EN_BLINK_ANIM[];
extern char TEXT_LEDSTRIP_CHANGE_OTA_ANIM[];

extern char TEXT_OTAANIM_NONE[];
extern char TEXT_OTAANIM_PROGRESS[];
extern char TEXT_OTAANIM_COLOR[];
//extern char TEXT_BACK[];

//LedstripSelectAnimationMenu
extern char TEXT_SELECTANIMATION[];
extern char TEXT_ANIMATION_DEFAULTRAINBOW[];
extern char TEXT_ANIMATION_BETTERRAINBOW[];
extern char TEXT_ANIMATION_SPEEDSYNCANIMATION[];
extern char TEXT_ANIMATION_CUSTOMCOLOR[];

//LedstripSelectBlinkMenu
extern char TEXT_ANIMATION_BLINKNONE[];
extern char TEXT_ANIMATION_BLINKLEFT[];
extern char TEXT_ANIMATION_BLINKRIGHT[];
extern char TEXT_ANIMATION_BLINKBOTH[];

//LockscreenSettingsMenu
extern char TEXT_LOCKSCREENSETTINGS[];
extern char TEXT_ALLOWPRESETSWITCH[];
extern char TEXT_KEEPLOCKED[];
extern char TEXT_PINDIGIT0[];
extern char TEXT_PINDIGIT1[];
extern char TEXT_PINDIGIT2[];
extern char TEXT_PINDIGIT3[];
//extern char TEXT_BACK[];

//ModesSettingsMenu
//extern char TEXT_MODESSETTINGS[];
extern char TEXT_DEFAULTMODESETTIGNS[];
extern char TEXT_TEMPOMATMODESETTINGS[];
//extern char TEXT_LARSMMODESETTINGS[];
//extern char TEXT_GAMETRAKMODESETTINGS[];
//extern char TEXT_BACK[];

//MosfetsMenu
//extern char TEXT_MOSFETS[];
extern char TEXT_MOSFET0[];
extern char TEXT_MOSFET1[];
extern char TEXT_MOSFET2[];

//TempomatModeSettingsMenu
//extern char TEXT_TEMPOMATMODESETTINGS[];
extern char TEXT_APPLY[];
extern char TEXT_NCRUISEMOTTGT[];
//extern char TEXT_MODELMODE[];
//extern char TEXT_BACK[];

//WiFiSettingsMenu
//extern char TEXT_WIFISETTINGS[];
//extern char TEXT_GENERICWIFISETTINGS[];
//extern char TEXT_BACK[];

//BoardcomputerHardwareSettingsMenu
//extern char TEXT_BOARDCOMPUTERHARDWARESETTINGS[];
//extern char TEXT_LOCKSCREENSETTINGS[];
extern char TEXT_CALIBRATE[];
extern char TEXT_SAMPLECOUNT[];
extern char TEXT_GASMIN[];
extern char TEXT_GASMAX[];
extern char TEXT_BREMSMIN[];
extern char TEXT_BREMSMAX[];
extern char TEXT_DPADDEBOUNCE[];
extern char TEXT_GAMETRAKCALIBRATE[];
extern char TEXT_SETGAMETRAKXMIN[];
extern char TEXT_SETGAMETRAKXMAX[];
extern char TEXT_SETGAMETRAKYMIN[];
extern char TEXT_SETGAMETRAKYMAX[];
extern char TEXT_SETGAMETRAKDISTMIN[];
extern char TEXT_SETGAMETRAKDISTMAX[];
extern char TEXT_TIMERS[];
//extern char TEXT_BACK[];

//ProfilesMenu
//extern char TEXT_PROFILES[];
extern char TEXT_PROFILE0[];
extern char TEXT_PROFILE1[];
extern char TEXT_PROFILE2[];
extern char TEXT_PROFILE3[];
//extern char TEXT_BACK[];

//PresetsMenu
//extern char TEXT_PRESETS[];
extern char TEXT_DEFAULTEVERYTHING[];
extern char TEXT_DEFAULTLIMITS[];
extern char TEXT_KIDSLIMITS[];
extern char TEXT_DEFAULTPOTI[];
extern char TEXT_DEFAULTCONTROLLERHARDWARE[];
extern char TEXT_MOSFETSOFFCONTROLLERHARDWARE[];
extern char TEXT_SPINNERCONTROLLERHARDWARE[];
extern char TEXT_DEFAULTBOARDCOMPUTERHARDWARE[];
extern char TEXT_DEFAULTDEFAULTMODE[];
extern char TEXT_SINUSOIDALDEFAULTMODE[];
extern char TEXT_DEFAULTTEMPOMATMODE[];
extern char TEXT_DEFAULTLARSMMODE[];
extern char TEXT_STREET[];
extern char TEXT_SIDEWALK[];
extern char TEXT_POLICE[];
extern char TEXT_RACE[];

//SelectModeMenu
//extern char TEXT_SELECTMODE[];
extern char TEXT_DEFAULT[];
extern char TEXT_TEMPOMAT[];
extern char TEXT_LARSM[];
extern char TEXT_REMOTECONTROL[];
extern char TEXT_GAMETRAK[];
extern char TEXT_MOTORTEST[];
//extern char TEXT_BACK[];

//TimersMenu
//extern char TEXT_TIMERS[];
extern char TEXT_STATSUPDATERATE[];
//extern char TEXT_BACK[];

//TimeSettingsMenu
//extern char TEXT_TIME[];
extern char TEXT_OFFSET[];
extern char TEXT_DAYLIGHTSAVINGMODE[];
extern char TEXT_NTPENABLED[];
extern char TEXT_NTPSERVER[];
extern char TEXT_NTPMODE[];
extern char TEXT_NTPINTERVAL[];
//extern char TEXT_BACK[];

//ChangeValueDisplay<BluetoothMode>
extern char TEXT_OFF[];
extern char TEXT_MASTER[];
extern char TEXT_SLAVE[];

//ChangeValueDisplay<ControlMode>
extern char TEXT_OPENMODE[];
extern char TEXT_VOLTAGE[];
extern char TEXT_SPEED[];
extern char TEXT_TORQUE[];
//extern char TEXT_BACK[];

//ChangeValueDisplay<ControlType>
extern char TEXT_COMMUTATION[];
extern char TEXT_SINUSOIDAL[];
extern char TEXT_FIELDORIENTEDCONTROL[];
//extern char TEXT_BACK[];

//ChangeValueDisplay<HandbremseMode>
extern char TEXT_HANDBREMS_MOSFETS_OFF[];
extern char TEXT_HANDBREMS_OPENMODE[];
extern char TEXT_HANDBREMS_SPEED0[];

//ChangeValueDisplay<LarsmMode::Mode>
extern char TEXT_LARSMMODE1[];
extern char TEXT_LARSMMODE2[];
extern char TEXT_LARSMMODE3[];
extern char TEXT_LARSMMODE4[];
//extern char TEXT_BACK[];

//ChangeValueDisplay<UnifiedModelMode>
//extern char TEXT_COMMUTATION[];
//extern char TEXT_SINUSOIDAL[];
extern char TEXT_FOCVOLTAGE[];
extern char TEXT_FOCSPEED[];
extern char TEXT_FOCTORQUE[];
//extern char TEXT_BACK[];

//ChangeValueDisplay<wifi_mode_t>
extern char TEXT_WIFI_MODE_NULL[];
extern char TEXT_WIFI_MODE_STA[];
extern char TEXT_WIFI_MODE_AP[];
extern char TEXT_WIFI_MODE_APSTA[];
//extern char TEXT_BACK[];

//ChangeValueDisplay<wifi_power_t>
extern char TEXT_WIFI_POWER_19_5dBm[];
extern char TEXT_WIFI_POWER_19dBm[];
extern char TEXT_WIFI_POWER_18_5dBm[];
extern char TEXT_WIFI_POWER_17dBm[];
extern char TEXT_WIFI_POWER_15dBm[];
extern char TEXT_WIFI_POWER_13dBm[];
extern char TEXT_WIFI_POWER_11dBm[];
extern char TEXT_WIFI_POWER_8_5dBm[];
extern char TEXT_WIFI_POWER_7dBm[];
extern char TEXT_WIFI_POWER_5dBm[];
extern char TEXT_WIFI_POWER_2dBm[];
extern char TEXT_WIFI_POWER_MINUS_1dBm[];
//extern char TEXT_BACK[];

//Crashmenu
extern char TEXT_CRASHMENU[];
extern char TEXT_CRASH_ASSERT[];
extern char TEXT_CRASH_DIVZERO[];

//SelectBuildServerMenu
extern char TEXT_SELECTBUILDSERVERMENU[];
extern char TEXT_NOBUILDSERVERCONFIGURED[];

//Otamenu
extern char TEXT_UPDATENOW[];
extern char TEXT_SELECTBUILD[];
extern char TEXT_OTA_NOBUILDSERVERAVAILABLE[];
extern char TEXT_OTA_NOBUILDSERVERSELECTED[];
extern char TEXT_OTA_NOCONNECTION[];
extern char TEXT_OTA_WAITFORRESPONSE[];

//LedstripColorMenu
extern char TEXT_LEDSTRIPCOLORMENU[];

//StatisticsMenu
extern char TEXT_STATISTICSMENU[];
extern char TEXT_STATSSAVE[];
extern char TEXT_STATSCLEAR[];

#ifdef FEATURE_CAN
extern char TEXT_POWERSUPPLY[];
#endif
extern char TEXT_REENABLE_MENUITEMS[];

//SelectBuildserverBranchMenu
extern char TEXT_SELECT_BRANCH[];
extern char TEXT_SELECT_BRANCH_CLEAR[];

//QrCodeDebug
extern char TEXT_QRCODE_DEBUG[];

// TaskmanagerMenu
extern char TEXT_TASKMANAGER[];

//GreenPassMenu
extern char TEXT_GREENPASS[];
extern char TEXT_ADDCERT[];
extern char TEXT_DELCERT[];

//EspNowMenu
extern char TEXT_ESPNOW[];
extern char TEXT_ESPNOW_MENU[];
extern char TEXT_ESPNOW_RECEIVETS[];
extern char TEXT_ESPNOW_RECEIVETSFROMBOBBY[];
extern char TEXT_ESPNOW_SENDTSMSG[];

//EspNowSettingsMenu
extern char TEXT_ESPNOW_SETTINGS[];
extern char TEXT_ESPNOW_SYNCTIME[];
extern char TEXT_ESPNOW_SYNCWITHOTHERS[];
extern char TEXT_ESPNOW_SYNCBLINK[];
} // namespace

using namespace bobbytexts;
