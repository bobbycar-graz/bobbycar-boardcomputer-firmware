#include "settingsmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay_string.h>
#include <icons/back.h>
#include <textwithvaluehelper.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "bobbycheckbox.h"
#include "displays/bobbychangevaluedisplay.h"
#include "displays/menus/aboutmenu.h"
#include "displays/menus/blesettingsmenu.h"
#include "displays/menus/bluetoothsettingsmenu.h"
#include "displays/menus/boardcomputerhardwaresettingsmenu.h"
#include "displays/menus/buzzermenu.h"
#include "displays/menus/cloudsettingsmenu.h"
#include "displays/menus/controllerhardwaresettingsmenu.h"
#include "displays/menus/crashmenu.h"
#include "displays/menus/espnowmenu.h"
#include "displays/menus/featureflagsmenu.h"
#include "displays/menus/gitmenu.h"
#include "displays/menus/limitssettingsmenu.h"
#include "displays/menus/modessettingsmenu.h"
#include "displays/menus/networksettingsmenu.h"
#include "displays/menus/selectbuildservermenu.h"
#include "displays/menus/timesettingsmenu.h"
#include "displays/menus/udpcloudsettingsmenu.h"
#include "globals.h"
#include "icons/bluetooth.h"
#include "icons/buzzer.h"
#include "icons/demos.h"
#include "icons/git.h"
#include "icons/hardware.h"
#include "icons/info.h"
#include "icons/time.h"
#include "icons/update.h"
#include "icons/wifi.h"

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
constexpr char TEXT_USERNAME[] = "Username"; // ota-name
constexpr char TEXT_ANHAENGER_ID[] = "Anhaenger ID";
constexpr char TEXT_AUTOCONNECTBMS[] = "Auto connect BMS";
constexpr char TEXT_BUZZER[] = "Buzzer";
constexpr char TEXT_FRONTLED[] = "Front LED";
constexpr char TEXT_BACKLED[] = "Back LED";
constexpr char TEXT_CRASHMENU[] = "Crash Menu";
constexpr char TEXT_ABOUT[] = "About";
constexpr char TEXT_GIT[] = "Git";
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

using AnhaengerIdChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint16_t>,
    espgui::StaticTitle<TEXT_ANHAENGER_ID>,
    AnhaengerIdAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using UsernameChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<std::string>,
    espgui::StaticTitle<TEXT_USERNAME>,
    UsernameAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
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
    if (configs.feature.esp_now.isEnabled.value())
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ESPNOW>,                    PushScreenAction<EspNowMenu>, StaticMenuItemIcon<&bobbyicons::wifi>>>();
#ifdef FEATURE_BLUETOOTH
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHSETTINGS>,             PushScreenAction<BluetoothSettingsMenu>, StaticMenuItemIcon<&bobbyicons::bluetooth>>>();
#endif
    if (configs.feature.ble.isEnabled.value())
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLESETTINGS>,               PushScreenAction<BleSettingsMenu>, StaticMenuItemIcon<&bobbyicons::bluetooth>>>();
    if (configs.feature.cloud.isEnabled.value())
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDSETTINGS>,             PushScreenAction<CloudSettingsMenu>>>();
    if (configs.feature.udpcloud.isEnabled.value())
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UDPCLOUDSETTINGS>,          PushScreenAction<UdpCloudSettingsMenu>>>();
    if (configs.feature.ota.isEnabled.value())
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTBUILDSERVERMENU>,     PushScreenAction<SelectBuildServerMenu>, StaticMenuItemIcon<&bobbyicons::update>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TIME>,                          PushScreenAction<TimeSettingsMenu>, StaticMenuItemIcon<&bobbyicons::time>>>();
    if (!simplified)
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODESSETTINGS>,              PushScreenAction<ModesSettingsMenu>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CONTROLLERHARDWARESETTINGS>, PushScreenAction<ControllerHardwareSettingsMenu>, StaticMenuItemIcon<&bobbyicons::hardware>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BOARDCOMPUTERHARDWARESETTINGS>, PushScreenAction<BoardcomputerHardwareSettingsMenu>, StaticMenuItemIcon<&bobbyicons::hardware>>>();
    }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FEATUREFLAGS>,                  PushScreenAction<FeatureFlagsMenu>, StaticMenuItemIcon<&bobbyicons::demos>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_USERNAME>,                      PushScreenAction<UsernameChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_ANHAENGER_ID, AnhaengerIdAccessor>, PushScreenAction<AnhaengerIdChangeScreen>>>();
//#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
//    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_AUTOCONNECTBMS>,                BobbyCheckbox, AutoConnectBmsAccessor>>();
//#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BUZZER>,                        PushScreenAction<BuzzerMenu>, StaticMenuItemIcon<&bobbyicons::buzzer>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTLED>,                      BobbyCheckbox, FrontLedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLED>,                       BobbyCheckbox, BackLedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CRASHMENU>,                     PushScreenAction<CrashMenu>,  StaticMenuItemIcon<&bobbyicons::demos>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ABOUT>,                         PushScreenAction<AboutMenu>, StaticMenuItemIcon<&bobbyicons::info>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GIT>,                           PushScreenAction<GitMenu>, StaticMenuItemIcon<&bobbyicons::git>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                          PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string SettingsMenu::title() const
{
    return TEXT_SETTINGS;
}

void SettingsMenu::back()
{
    popScreen();
}
