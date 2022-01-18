#include "settingsmenu.h"

// Arduino includes
#include <Arduino.h>

// 3rdparty lib includes
#include "actions/switchscreenaction.h"
#include "icons/back.h"

// local includes
#include "utils.h"
#include "icons/wifi.h"
#include "icons/bluetooth.h"
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
#include "displays/menus/cloudsettingsmenu.h"
#include "displays/menus/udpcloudsettingsmenu.h"
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
#include "displays/menus/featureflagsmenu.h"
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
constexpr char TEXT_FEATUREFLAGS[] = "Feature flags";
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
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LIMITSSETTINGS>,            SwitchScreenAction<LimitsSettingsMenu>>>();

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NETWORKSETTINGS>,               SwitchScreenAction<NetworkSettingsMenu>, StaticMenuItemIcon<&bobbyicons::wifi>>>();
#ifdef FEATURE_ESPNOW
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ESPNOW>,                        SwitchScreenAction<EspNowMenu>, StaticMenuItemIcon<&bobbyicons::wifi>>>();
#endif
#ifdef FEATURE_BLUETOOTH
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHSETTINGS>,             SwitchScreenAction<BluetoothSettingsMenu>, StaticMenuItemIcon<&bobbyicons::bluetooth>>>();
#endif
    if (configs.feature.ble.value)
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLESETTINGS>,                   SwitchScreenAction<BleSettingsMenu>, StaticMenuItemIcon<&bobbyicons::bluetooth>>>();
    if (configs.feature.cloud.value)
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDSETTINGS>,                 SwitchScreenAction<CloudSettingsMenu>>>();
    if (configs.feature.udpcloud.value)
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UDPCLOUDSETTINGS>,              SwitchScreenAction<UdpCloudSettingsMenu>>>();
#ifdef FEATURE_OTA
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTBUILDSERVERMENU>,         SwitchScreenAction<SelectBuildServerMenu>, StaticMenuItemIcon<&bobbyicons::update>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TIME>,                          SwitchScreenAction<TimeSettingsMenu>, StaticMenuItemIcon<&bobbyicons::time>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODESSETTINGS>,                 SwitchScreenAction<ModesSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CONTROLLERHARDWARESETTINGS>,    SwitchScreenAction<ControllerHardwareSettingsMenu>, StaticMenuItemIcon<&bobbyicons::hardware>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BOARDCOMPUTERHARDWARESETTINGS>, SwitchScreenAction<BoardcomputerHardwareSettingsMenu>, StaticMenuItemIcon<&bobbyicons::hardware>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FEATUREFLAGS>,                  SwitchScreenAction<FeatureFlagsMenu>, StaticMenuItemIcon<&bobbyicons::demos>>>();
//#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
//    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_AUTOCONNECTBMS>,                BobbyCheckbox, AutoConnectBmsAccessor>>();
//#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BUZZER>,                        SwitchScreenAction<BuzzerMenu>, StaticMenuItemIcon<&bobbyicons::buzzer>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTLED>,                      BobbyCheckbox, FrontLedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLED>,                       BobbyCheckbox, BackLedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CRASHMENU>,                     SwitchScreenAction<CrashMenu>,  StaticMenuItemIcon<&bobbyicons::demos>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ABOUT>,                         SwitchScreenAction<AboutMenu>, StaticMenuItemIcon<&bobbyicons::info>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                          SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string SettingsMenu::text() const
{
    return TEXT_SETTINGS;
}

void SettingsMenu::back()
{
    switchScreen<MainMenu>();
}
