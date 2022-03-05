#include "settingsmenu.h"

// Arduino includes
#include <Arduino.h>

// 3rdparty lib includes
#include "actions/pushscreenaction.h"
#include "actions/popscreenaction.h"
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
#include "displays/menus/networksettingsmenu.h"
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
#include "bobbycheckbox.h"

namespace {
constexpr char TEXT_SETTINGS[] = "Settings";
constexpr char TEXT_BACKLIGHT[] = "Backlight";
constexpr char TEXT_LIMITSSETTINGS[] = "Limits settings";
constexpr char TEXT_NETWORKSETTINGS[] = "Network settings";
constexpr char TEXT_ESPNOW[] = "ESPNOW settings";
constexpr char TEXT_BLUETOOTHSETTINGS[] = "Bluetooth settings";
constexpr char TEXT_BLESETTINGS[] = "BLE settings";
constexpr char TEXT_CLOUDSETTINGS[] = "Cloud settings";
constexpr char TEXT_UDPCLOUDSETTINGS[] = "UDP Cloud settings";
constexpr char TEXT_SELECTBUILDSERVERMENU[] = "Select Buildserver";
constexpr char TEXT_TIME[] = "Time";
constexpr char TEXT_MODESSETTINGS[] = "Modes settings";
constexpr char TEXT_CONTROLLERHARDWARESETTINGS[] = "Controller H/W settings";
constexpr char TEXT_BOARDCOMPUTERHARDWARESETTINGS[] = "Boardcomputer H/W settings";
constexpr char TEXT_AUTOCONNECTBMS[] = "Auto connect BMS";
constexpr char TEXT_BUZZER[] = "Buzzer";
constexpr char TEXT_FRONTLED[] = "Front LED";
constexpr char TEXT_BACKLED[] = "Back LED";
constexpr char TEXT_CRASHMENU[] = "Crash Menu";
constexpr char TEXT_ABOUT[] = "About";
constexpr char TEXT_BACK[] = "Back";

#ifdef FEATURE_LEDBACKLIGHT
struct BacklightAccessor : public virtual espgui::AccessorInterface<bool>
{
    bool getValue() const override { return digitalRead(PINS_LEDBACKLIGHT) != ledBacklightInverted; }
    espgui::AccessorInterface<bool>::setter_result_t setValue(bool value) override { digitalWrite(PINS_LEDBACKLIGHT, value != ledBacklightInverted); return {}; }
};
#endif
struct FrontLedAccessor : public espgui::RefAccessor<bool> { bool &getRef() const override { return controllers.front.command.led; } };
struct BackLedAccessor : public espgui::RefAccessor<bool> { bool &getRef() const override { return controllers.back.command.led; } };
} // namespace

using namespace espgui;

SettingsMenu::SettingsMenu()
{
#ifdef FEATURE_LEDBACKLIGHT
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLIGHT>,                     BobbyCheckbox, BacklightAccessor>>();
#endif
    if (!simplified)
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LIMITSSETTINGS>,            PushScreenAction<LimitsSettingsMenu>>>();

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NETWORKSETTINGS>,               PushScreenAction<NetworkSettingsMenu>, StaticMenuItemIcon<&bobbyicons::wifi>>>();
#ifdef FEATURE_ESPNOW
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ESPNOW>,                        PushScreenAction<EspNowMenu>, StaticMenuItemIcon<&bobbyicons::wifi>>>();
#endif
#ifdef FEATURE_BLUETOOTH
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHSETTINGS>,             PushScreenAction<BluetoothSettingsMenu>, StaticMenuItemIcon<&bobbyicons::bluetooth>>>();
#endif
#ifdef FEATURE_BLE
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLESETTINGS>,                   PushScreenAction<BleSettingsMenu>, StaticMenuItemIcon<&bobbyicons::bluetooth>>>();
#endif
#ifdef FEATURE_CLOUD
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDSETTINGS>,                 PushScreenAction<CloudSettingsMenu>>>();
#endif
#ifdef FEATURE_UDPCLOUD
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UDPCLOUDSETTINGS>,              PushScreenAction<UdpCloudSettingsMenu>>>();
#endif
#ifdef FEATURE_OTA
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTBUILDSERVERMENU>,         PushScreenAction<SelectBuildServerMenu>, StaticMenuItemIcon<&bobbyicons::update>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TIME>,                          PushScreenAction<TimeSettingsMenu>, StaticMenuItemIcon<&bobbyicons::time>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODESSETTINGS>,                 PushScreenAction<ModesSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CONTROLLERHARDWARESETTINGS>,    PushScreenAction<ControllerHardwareSettingsMenu>, StaticMenuItemIcon<&bobbyicons::hardware>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BOARDCOMPUTERHARDWARESETTINGS>, PushScreenAction<BoardcomputerHardwareSettingsMenu>, StaticMenuItemIcon<&bobbyicons::hardware>>>();
//#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
//    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_AUTOCONNECTBMS>,                BobbyCheckbox, AutoConnectBmsAccessor>>();
//#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BUZZER>,                        PushScreenAction<BuzzerMenu>, StaticMenuItemIcon<&bobbyicons::buzzer>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTLED>,                      BobbyCheckbox, FrontLedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLED>,                       BobbyCheckbox, BackLedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CRASHMENU>,                     PushScreenAction<CrashMenu>,  StaticMenuItemIcon<&bobbyicons::demos>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ABOUT>,                         PushScreenAction<AboutMenu>, StaticMenuItemIcon<&bobbyicons::info>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                          PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string SettingsMenu::text() const
{
    return TEXT_SETTINGS;
}

void SettingsMenu::back()
{
    popScreen();
}
