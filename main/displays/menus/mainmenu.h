#pragma once

// local includes
#include "menudisplay.h"
#include "menuitem.h"
#include "batterymenu.h"
#include "actions/switchscreenaction.h"
#include "actions/modesettingsaction.h"
#include "actions/rebootaction.h"
#include "texts.h"
#include "icons/back.h"
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
#ifdef MAINMENU_PLUGIN
#include MAINMENU_PLUGIN
#endif

// forward declares
namespace {
class StatusDisplay;
class SelectModeMenu;
class LedstripMenu;
class ProfilesMenu;
class PresetsMenu;
class GraphsMenu;
class PowerSupplyDisplay;
class BmsMenu;
class SettingsMenu;
class Lockscreen;
class MosfetsMenu;
class DemosMenu;
class GarageDisplay;
class OtaMenu;
class PoweroffDisplay;
class DebugMenu;
class BatteryMenu;
#ifdef MAINMENU_PLUGIN
    GMEN2
#endif
#if !defined(MAINMENU_PLUGIN) and !defined(GMEN3)
#define SHOWITEM !simplified
#else
#define SHOWITEM GMEN3
#endif
} // namespace

using namespace espgui;

namespace {
class MainMenu :
    public MenuDisplay,
    public StaticText<TEXT_MAINMENU>,
    public BackActionInterface<SwitchScreenAction<StatusDisplay>>
{
public:
    MainMenu()
    {
                         constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STATUS>,       SwitchScreenAction<StatusDisplay>, StaticMenuItemIcon<&espgui::icons::back>>>();
                         constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTMODE>,   SwitchScreenAction<SelectModeMenu>, StaticMenuItemIcon<&bobbyicons::modes>>>();
#ifdef FEATURE_LEDSTRIP
                         constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LEDSTRIP>,     SwitchScreenAction<LedstripMenu>,   StaticMenuItemIcon<&bobbyicons::neopixel>>>();
#endif
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
                         constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LOCKVEHICLE>,  SwitchScreenAction<Lockscreen>, StaticMenuItemIcon<&bobbyicons::lock>>>();
#ifdef FEATURE_GARAGE
    if (SHOWITEM)   { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GARAGE>,       SwitchScreenAction<GarageDisplay>>>(); }
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
};
} // namespace
