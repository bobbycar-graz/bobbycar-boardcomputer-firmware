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
#include "icons/back.h"
#include "texts.h"
#include "globals.h"
#include "accessors/settingsaccessors.h"

// forward declares
namespace {
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
}

using namespace espgui;

namespace {
#ifdef FEATURE_LEDBACKLIGHT
struct BacklightAccessor : public virtual AccessorInterface<bool>
{
    bool getValue() const override { return digitalRead(PINS_LEDBACKLIGHT) != ledBacklightInverted; }
    void setValue(bool value) override { digitalWrite(PINS_LEDBACKLIGHT, value != ledBacklightInverted); }
};
#endif
struct FrontLedAccessor : public RefAccessor<bool> { bool &getRef() const override { return controllers.front.command.led; } };
struct BackLedAccessor : public RefAccessor<bool> { bool &getRef() const override { return controllers.back.command.led; } };

class SettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_SETTINGS>,
    public BackActionInterface<SwitchScreenAction<MainMenu>>
{
public:
    SettingsMenu()
    {
#ifdef FEATURE_LEDBACKLIGHT
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLIGHT>,                     ToggleBoolAction, CheckboxIcon, BacklightAccessor>>();
#endif
        if (!simplified)
        {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LIMITSSETTINGS>,                SwitchScreenAction<LimitsSettingsMenu>>>();
        }
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFISETTINGS>,                  SwitchScreenAction<WifiSettingsMenu>, StaticMenuItemIcon<&bobbyicons::wifi>>>();
#ifdef FEATURE_BLUETOOTH
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHSETTINGS>,             SwitchScreenAction<BluetoothSettingsMenu>, StaticMenuItemIcon<&bobbyicons::bluetooth>>>();
#endif
#ifdef FEATURE_BLE
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLESETTINGS>,                   SwitchScreenAction<BleSettingsMenu>, StaticMenuItemIcon<&bobbyicons::bluetooth>>>();
#endif
#ifdef FEATURE_CLOUD
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDSETTINGS>,                 SwitchScreenAction<CloudSettingsMenu>>>();
#endif
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TIME>,                          SwitchScreenAction<TimeSettingsMenu>, StaticMenuItemIcon<&bobbyicons::time>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODESSETTINGS>,                 SwitchScreenAction<ModesSettingsMenu>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CONTROLLERHARDWARESETTINGS>,    SwitchScreenAction<ControllerHardwareSettingsMenu>, StaticMenuItemIcon<&bobbyicons::hardware>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BOARDCOMPUTERHARDWARESETTINGS>, SwitchScreenAction<BoardcomputerHardwareSettingsMenu>, StaticMenuItemIcon<&bobbyicons::hardware>>>();
#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_AUTOCONNECTBMS>,                ToggleBoolAction, CheckboxIcon, AutoConnectBmsAccessor>>();
#endif
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BUZZER>,                        SwitchScreenAction<BuzzerMenu>, StaticMenuItemIcon<&bobbyicons::buzzer>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTLED>,                      ToggleBoolAction, CheckboxIcon, FrontLedAccessor>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLED>,                       ToggleBoolAction, CheckboxIcon, BackLedAccessor>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ABOUT>,                         SwitchScreenAction<AboutMenu>, StaticMenuItemIcon<&bobbyicons::info>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                          SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
    }
};
} // namespace
