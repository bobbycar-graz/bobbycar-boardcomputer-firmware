#include "settingsmenu.h"

// Arduino includes
#include <Arduino.h>

// 3rdparty lib includes
#include "actions/toggleboolaction.h"
#include "actions/switchscreenaction.h"
#include "checkboxicon.h"
#include "icons/back.h"

// local includes
#include "utils.h"
#include "icons/wifi.h"
#if defined(FEATURE_BLUETOOTH) || defined(FEATURE_BLE)
#include "icons/bluetooth.h"
#endif
#include "icons/time.h"
#include "icons/hardware.h"
#include "icons/buzzer.h"
#include "icons/info.h"
#include "icons/demos.h"
#include "icons/update.h"
#include "globals.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/limitssettingsmenu.h"
#include "displays/menus/wifisettingsmenu.h"
#include "displays/menus/bluetoothsettingsmenu.h"
#include "displays/menus/blesettingsmenu.h"
#ifdef FEATURE_CLOUD
#include "displays/menus/cloudsettingsmenu.h"
#endif
#ifdef FEATURE_UDPCLOUD
#include "displays/menus/udpcloudsettingsmenu.h"
#endif
#include "displays/menus/espnowmenu.h"
#include "displays/menus/selectbuildservermenu.h"
#include "displays/menus/timesettingsmenu.h"
#include "displays/menus/modessettingsmenu.h"
#include "displays/menus/controllerhardwaresettingsmenu.h"
#include "displays/menus/boardcomputerhardwaresettingsmenu.h"
#include "displays/menus/buzzermenu.h"
#include "displays/menus/crashmenu.h"
#include "displays/menus/aboutmenu.h"
#include "displays/menus/mainmenu.h"

namespace {
#ifdef FEATURE_LEDBACKLIGHT
struct BacklightAccessor : public virtual espgui::AccessorInterface<bool>
{
    bool getValue() const override { return digitalRead(PINS_LEDBACKLIGHT) != ledBacklightInverted; }
    void setValue(bool value) override { digitalWrite(PINS_LEDBACKLIGHT, value != ledBacklightInverted); }
};
#endif
struct FrontLedAccessor : public espgui::RefAccessor<bool> { bool &getRef() const override { return controllers.front.command.led; } };
struct BackLedAccessor : public espgui::RefAccessor<bool> { bool &getRef() const override { return controllers.back.command.led; } };
} // namespace

using namespace espgui;

SettingsMenu::SettingsMenu()
{
#ifdef FEATURE_LEDBACKLIGHT
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLIGHT>,                     ToggleBoolAction, CheckboxIcon, BacklightAccessor>>();
#endif
    if (!simplified)
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LIMITSSETTINGS>,            SwitchScreenAction<LimitsSettingsMenu>>>();

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFISETTINGS>,                  SwitchScreenAction<WifiSettingsMenu>, StaticMenuItemIcon<&bobbyicons::wifi>>>();
#ifdef FEATURE_ESPNOW
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ESPNOW>,                        SwitchScreenAction<EspNowMenu>, StaticMenuItemIcon<&bobbyicons::wifi>>>();
#endif
#ifdef FEATURE_BLUETOOTH
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHSETTINGS>,             SwitchScreenAction<BluetoothSettingsMenu>, StaticMenuItemIcon<&bobbyicons::bluetooth>>>();
#endif
#ifdef FEATURE_BLE
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLESETTINGS>,                   SwitchScreenAction<BleSettingsMenu>, StaticMenuItemIcon<&bobbyicons::bluetooth>>>();
#endif
#ifdef FEATURE_CLOUD
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDSETTINGS>,                 SwitchScreenAction<CloudSettingsMenu>>>();
#endif
#ifdef FEATURE_UDPCLOUD
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UDPCLOUDSETTINGS>,              SwitchScreenAction<UdpCloudSettingsMenu>>>();
#endif
#ifdef FEATURE_OTA
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTBUILDSERVERMENU>,         SwitchScreenAction<SelectBuildServerMenu>, StaticMenuItemIcon<&bobbyicons::update>>>();
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
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CRASHMENU>,                     SwitchScreenAction<CrashMenu>,  StaticMenuItemIcon<&bobbyicons::demos>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ABOUT>,                         SwitchScreenAction<AboutMenu>, StaticMenuItemIcon<&bobbyicons::info>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                          SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void SettingsMenu::back()
{
    switchScreen<MainMenu>();
}
