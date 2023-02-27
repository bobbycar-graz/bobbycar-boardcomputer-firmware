#include "mainmenu.h"

// 3rdparty lib includes
#include <actions/pushscreenaction.h>
#include <actions/popscreenaction.h>

// local includes
#include "screens/selectmodemenu.h"
#include "screens/selectmodemenu.h"
#include "screens/ledstripmenu.h"
#include "actions/modesettingsaction.h"
#include "screens/profilesmenu.h"
#include "screens/graphsmenu.h"
#include "screens/batterymenu.h"
#include "screens/powersupplydisplay.h"
#include "screens/settingsmenu.h"
#include "screens/manageprofilesmenu.h"
#include "screens/mosfetsmenu.h"
#include "screens/demosmenu.h"
#include "screens/greenpassmenu.h"
#include "screens/lockscreen.h"
#include "screens/garagemenu.h"
#include "screens/otamenu.h"
#include "screens/poweroffdisplay.h"
#include "screens/statisticsmenu.h"
#include "screens/confiscationdisplay.h"
#include "screens/rebootscreen.h"
#include "screens/debugmenu.h"
#include "icons/battery.h"
#include "icons/modes.h"
#include "icons/presets.h"
#include "icons/graph.h"
#include "icons/settings.h"
#include "icons/lock.h"
#include "icons/demos.h"
#include "icons/update.h"
#include "icons/neopixel.h"
#include "icons/poweroff.h"
#include "icons/reboot.h"
#include "icons/statistics.h"
#include "icons/greenpass.h"
#include "icons/time.h"
#include "screens/statusdisplay.h"
#include "icons/back.h"


namespace bobby {

namespace {
constexpr char TAG[] = "BOBBY";

constexpr char TEXT_MAINMENU[] = "Main menu";
constexpr char TEXT_STATUS[] = "Status";
constexpr char TEXT_LEDSTRIP[] = "Ledstrip";
constexpr char TEXT_STATISTICSMENU[] = "Statistics";
constexpr char TEXT_SELECTMODE[] = "Select mode";
constexpr char TEXT_MODESETTINGS[] = "Mode settings";
constexpr char TEXT_PROFILES[] = "Profiles";
constexpr char TEXT_GRAPHS[] = "Graphs";
constexpr char TEXT_POWERSUPPLY[] = "Powersupply";
constexpr char TEXT_SETTINGS[] = "Settings";
constexpr char TEXT_GREENPASS[] = "Green Pass";
constexpr char TEXT_LOCKVEHICLE[] = "Lock vehicle";
constexpr char TEXT_MOSFETS[] = "Mosfets";
constexpr char TEXT_DEMOS[] = "Demos";
constexpr char TEXT_GARAGE[] = "Garage";
constexpr char TEXT_UPDATE[] = "Update";
constexpr char TEXT_POWEROFF[] = "Poweroff";
constexpr char TEXT_REBOOT[] = "Reboot";
constexpr char TEXT_DEBUG[] = "Debug";
constexpr char TEXT_BATTERY[] = "Battery";
constexpr char TEXT_BATTERYDEBUG[] = "Bat Debug Menu";
constexpr char TEXT_CONFISCATIONMODE[] = "Confiscation Mode";
constexpr char TEXT_TOGGLECLOUDDEBUG[] = "Cloud Debug";
constexpr char TEXT_MANAGEPROFILESMENU[] = "Manage Profiles";

} // namespace

MainMenu::MainMenu()
{
    using namespace espgui;

    // constructMenuItem<makeComponent<MenuItem, mainmenu::CurrentTimeText,     DummyAction, StaticMenuItemIcon<&bobbyicons::time>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STATUS>,       PushScreenAction<StatusDisplay>, StaticMenuItemIcon<&bobbyicons::back>>>();
    if (configs.feature.ledstrip.isEnabled.value())
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LEDSTRIP>,     PushScreenAction<LedstripMenu>,   StaticMenuItemIcon<&bobbyicons::neopixel>>>();
    }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STATISTICSMENU>, PushScreenAction<StatisticsMenu>, StaticMenuItemIcon<&bobbyicons::statistics>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTMODE>,   PushScreenAction<SelectModeMenu>, StaticMenuItemIcon<&bobbyicons::modes>>>();
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODESETTINGS>, ModeSettingsAction>>(); }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY>,      PushScreenAction<BatteryMenu>, StaticMenuItemIcon<&bobbyicons::battery>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETTINGS>,     PushScreenAction<SettingsMenu>, StaticMenuItemIcon<&bobbyicons::settings>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GREENPASS>,    PushScreenAction<GreenPassMenu>, StaticMenuItemIcon<&bobbyicons::greenpass>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LOCKVEHICLE>,  PushScreenAction<Lockscreen>, StaticMenuItemIcon<&bobbyicons::lock>>>();
    if (configs.feature.garage.isEnabled.value() && configs.feature.esp_now.isEnabled.value())
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GARAGE>,       PushScreenAction<GarageMenu>>>();
    }
    if (configs.feature.ota.isEnabled.value())
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UPDATE>,       PushScreenAction<OtaMenu>, StaticMenuItemIcon<&bobbyicons::update>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GRAPHS>,       PushScreenAction<GraphsMenu>, StaticMenuItemIcon<&bobbyicons::graph>>>();
#if defined(FEATURE_CAN) && defined(FEATURE_POWERSUPPLY)
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_POWERSUPPLY>,  PushScreenAction<PowerSupplyDisplay>>>();
#endif
#ifdef FEATURE_MOSFETS
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOSFETS>,      PushScreenAction<MosfetsMenu>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEMOS>,        PushScreenAction<DemosMenu>, StaticMenuItemIcon<&bobbyicons::demos>>>();
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_PROFILES>,     PushScreenAction<ProfilesMenu>, StaticMenuItemIcon<&bobbyicons::presets>>>(); }
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MANAGEPROFILESMENU>,PushScreenAction<ManageProfilesMenu>, StaticMenuItemIcon<&bobbyicons::presets>>>(); }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEBUG>,        PushScreenAction<DebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_POWEROFF>,     PushScreenAction<PoweroffDisplay>, StaticMenuItemIcon<&bobbyicons::poweroff>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CONFISCATIONMODE>, PushScreenAction<ConfiscationDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REBOOT>,       PushScreenAction<RebootScreen>, StaticMenuItemIcon<&bobbyicons::reboot>>>();
//#ifdef MAINMENU_PLUGIN
    // GMEN1
//#endif
}

std::string MainMenu::text() const
{
    return TEXT_MAINMENU;
}

void MainMenu::back()
{
    espgui::popScreen();
}
} // namespace bobby
