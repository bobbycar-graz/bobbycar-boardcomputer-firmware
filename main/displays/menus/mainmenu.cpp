#include "mainmenu.h"

// 3rdparty lib includes
#include "actions/switchscreenaction.h"
#include "icons/back.h"

// local includes
#include "displays/statusdisplay.h"
#include "displays/menus/selectmodemenu.h"
#include "displays/menus/selectmodemenu.h"
#include "displays/menus/ledstripmenu.h"
#include "actions/modesettingsaction.h"
#include "displays/menus/presetsmenu.h"
#include "displays/menus/profilesmenu.h"
#include "displays/menus/graphsmenu.h"
#include "displays/menus/batterymenu.h"
#include "displays/powersupplydisplay.h"
#include "displays/menus/bmsmenu.h"
#include "displays/menus/settingsmenu.h"
#include "displays/menus/mosfetsmenu.h"
#include "displays/menus/demosmenu.h"
#include "displays/menus/greenpassmenu.h"
#include "displays/lockscreen.h"
#if defined(FEATURE_GARAGE) && defined (FEATURE_ESPNOW)
#include "displays/menus/garagenmenu.h"
#endif
#include "displays/menus/otamenu.h"
#include "displays/poweroffdisplay.h"
#include "displays/menus/statisticsmenu.h"
#include "actions/rebootaction.h"
#include "displays/menus/debugmenu.h"
#include "icons/battery.h"
#include "icons/modes.h"
#include "icons/presets.h"
#include "icons/graph.h"
#ifdef FEATURE_BMS
#include "icons/bms.h"
#endif
#include "icons/settings.h"
#include "icons/lock.h"
#include "icons/demos.h"
#ifdef FEATURE_OTA
#include "icons/update.h"
#endif
#ifdef FEATURE_LEDSTRIP
#include "icons/neopixel.h"
#endif
#include "icons/poweroff.h"
#include "icons/reboot.h"
#include "icons/statistics.h"
#include "icons/greenpass.h"
#include "icons/time.h"
#include "tftinstance.h"

using namespace espgui;

namespace mainmenu {
/*
class CurrentTimeText : public virtual TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("&7Time: {}", local_clock_string());
    }
};
*/
} // namespace

MainMenu::MainMenu()
{
    // constructMenuItem<makeComponent<MenuItem, mainmenu::CurrentTimeText,     DummyAction, StaticMenuItemIcon<&bobbyicons::time>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STATUS>,       SwitchScreenAction<StatusDisplay>, StaticMenuItemIcon<&espgui::icons::back>>>();
#ifdef FEATURE_LEDSTRIP
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LEDSTRIP>,     SwitchScreenAction<LedstripMenu>,   StaticMenuItemIcon<&bobbyicons::neopixel>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STATISTICSMENU>, SwitchScreenAction<StatisticsMenu>, StaticMenuItemIcon<&bobbyicons::statistics>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTMODE>,   SwitchScreenAction<SelectModeMenu>, StaticMenuItemIcon<&bobbyicons::modes>>>();
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODESETTINGS>, ModeSettingsAction>>(); }
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_PRESETS>,      SwitchScreenAction<PresetsMenu>, StaticMenuItemIcon<&bobbyicons::presets>>>(); }
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_PROFILES>,     SwitchScreenAction<ProfilesMenu>>>(); }
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GRAPHS>,       SwitchScreenAction<GraphsMenu>, StaticMenuItemIcon<&bobbyicons::graph>>>(); }
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY>,      SwitchScreenAction<BatteryMenu>, StaticMenuItemIcon<&bobbyicons::battery>>>(); }
#if defined(FEATURE_CAN) && defined(FEATURE_POWERSUPPLY)
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_POWERSUPPLY>,  SwitchScreenAction<PowerSupplyDisplay>>>(); }
#endif
#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BMS>,          SwitchScreenAction<BmsMenu>, StaticMenuItemIcon<&bobbyicons::bms>>>(); }
#endif
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETTINGS>,     SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&bobbyicons::settings>>>(); }
#ifdef FEATURE_MOSFETS
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOSFETS>,      SwitchScreenAction<MosfetsMenu>>>(); }
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEMOS>,        SwitchScreenAction<DemosMenu>, StaticMenuItemIcon<&bobbyicons::demos>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GREENPASS>,    SwitchScreenAction<GreenPassMenu>, StaticMenuItemIcon<&bobbyicons::greenpass>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LOCKVEHICLE>,  SwitchScreenAction<Lockscreen>, StaticMenuItemIcon<&bobbyicons::lock>>>();
#if defined(FEATURE_GARAGE) && defined (FEATURE_ESPNOW)
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GARAGE>,       SwitchScreenAction<GarageMenu>>>(); }
#endif
#ifdef FEATURE_OTA
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UPDATE>,       SwitchScreenAction<OtaMenu>, StaticMenuItemIcon<&bobbyicons::update>>>(); }
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_POWEROFF>,     SwitchScreenAction<PoweroffDisplay>, StaticMenuItemIcon<&bobbyicons::poweroff>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REBOOT>,       RebootAction, StaticMenuItemIcon<&bobbyicons::reboot>>>();
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEBUG>,        SwitchScreenAction<DebugMenu>>>(); }
#ifdef MAINMENU_PLUGIN
    GMEN1
#endif
}

void MainMenu::back()
{
    switchScreen<StatusDisplay>();
}
