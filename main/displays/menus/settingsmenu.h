#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "actions/toggleboolaction.h"
#include "actions/switchscreenaction.h"
#include "checkboxicon.h"
#include "icons/wifi.h"
#if defined(FEATURE_BLUETOOTH) || defined(FEATURE_BLE)
#include "icons/bluetooth.h"
#endif
#include "icons/time.h"
#include "icons/hardware.h"
#include "icons/buzzer.h"
#include "icons/info.h"
#include "icons/demos.h"
#include "icons/back.h"
#include "icons/update.h"
#include "texts.h"
#include "globals.h"
#include "accessors/settingsaccessors.h"

// forward declares
class LimitsSettingsMenu;
class WifiSettingsMenu;
class BluetoothSettingsMenu;
class BleSettingsMenu;
class CloudSettingsMenu;
class TimeSettingsMenu;
class ModesSettingsMenu;
class ControllerHardwareSettingsMenu;
class BoardcomputerHardwareSettingsMenu;
class BuzzerMenu;
class AboutMenu;
class MainMenu;
class CrashMenu;
class SelectBuildServerMenu;

class SettingsMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_SETTINGS>,
    public espgui::BackActionInterface<espgui::SwitchScreenAction<MainMenu>>
{
public:
    SettingsMenu();
};
