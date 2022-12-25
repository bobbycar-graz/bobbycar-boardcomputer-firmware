#include "settingsmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay_string.h>
#include <menuitem.h>
#include <textwithvaluehelper.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "globals.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"
#include "icons/bluetooth.h"
#include "icons/buzzer.h"
#include "icons/demos.h"
#include "icons/git.h"
#include "icons/hardware.h"
#include "icons/info.h"
#include "icons/time.h"
#include "icons/update.h"
#include "icons/wifi.h"
#include "screens/aboutmenu.h"
#include "screens/blesettingsmenu.h"
#include "screens/boardcomputerhardwaresettingsmenu.h"
#include "screens/buzzermenu.h"
#include "screens/cloudsettingsmenu.h"
#include "screens/controllerhardwaresettingsmenu.h"
#include "screens/crashmenu.h"
#include "screens/espnowmenu.h"
#include "screens/featureflagsmenu.h"
#include "screens/gitmenu.h"
#include "screens/limitssettingsmenu.h"
#include "screens/modessettingsmenu.h"
#include "screens/networksettingsmenu.h"
#include "screens/selectbuildservermenu.h"
#include "screens/timesettingsmenu.h"
#include "screens/udpcloudsettingsmenu.h"

namespace bobby {

namespace {
constexpr char TEXT_SETTINGS[] = "Settings";
constexpr char TEXT_BACKLIGHT[] = "Backlight";
constexpr char TEXT_LIMITSSETTINGS[] = "Limits settings";
constexpr char TEXT_NETWORKSETTINGS[] = "Network settings";
constexpr char TEXT_ESPNOW[] = "ESPNOW settings";
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
    espgui::StaticText<TEXT_ANHAENGER_ID>,
    AnhaengerIdAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using UsernameChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<std::string>,
    espgui::StaticText<TEXT_USERNAME>,
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
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BUZZER>,                        PushScreenAction<BuzzerMenu>, StaticMenuItemIcon<&bobbyicons::buzzer>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTLED>,                      BobbyCheckbox, FrontLedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLED>,                       BobbyCheckbox, BackLedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CRASHMENU>,                     PushScreenAction<CrashMenu>,  StaticMenuItemIcon<&bobbyicons::demos>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ABOUT>,                         PushScreenAction<AboutMenu>, StaticMenuItemIcon<&bobbyicons::info>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GIT>,                           PushScreenAction<GitMenu>, StaticMenuItemIcon<&bobbyicons::git>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                          PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string SettingsMenu::text() const
{
    return TEXT_SETTINGS;
}

void SettingsMenu::back()
{
    popScreen();
}
} // namespace bobby
