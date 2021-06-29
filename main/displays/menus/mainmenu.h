#pragma once

// local includes
#include "menudisplay.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "actions/modesettingsaction.h"
#include "actions/rebootaction.h"
#include "texts.h"
#include "icons/back.h"
#include "icons/modes.h"
#include "icons/presets.h"
#include "icons/graph.h"
#include "icons/bms.h"
#include "icons/settings.h"
#include "icons/lock.h"
#include "icons/demos.h"
#include "icons/poweroff.h"
#include "icons/reboot.h"

// forward declares
namespace {
class StatusDisplay;
class SelectModeMenu;
class ProfilesMenu;
class PresetsMenu;
class GraphsMenu;
class PowerSupplyDisplay;
class BmsMenu;
class SettingsMenu;
class Lockscreen;
class MosfetsMenu;
class DemosMenu;
class PoweroffDisplay;
class DebugMenu;
} // namespace

namespace {
class MainMenu :
    public MenuDisplay,
    public StaticText<TEXT_MAINMENU>,
    public BackActionInterface<SwitchScreenAction<StatusDisplay>>
{
public:
    MainMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STATUS>,       SwitchScreenAction<StatusDisplay>, StaticMenuItemIcon<&icons::back>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTMODE>,   SwitchScreenAction<SelectModeMenu>, StaticMenuItemIcon<&icons::modes>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODESETTINGS>, ModeSettingsAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_PRESETS>,      SwitchScreenAction<PresetsMenu>, StaticMenuItemIcon<&icons::presets>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_PROFILES>,     SwitchScreenAction<ProfilesMenu>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GRAPHS>,       SwitchScreenAction<GraphsMenu>, StaticMenuItemIcon<&icons::graph>>>();
#ifdef FEATURE_CAN
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_POWERSUPPLY>,  SwitchScreenAction<PowerSupplyDisplay>>>();
#endif
#ifdef FEATURE_BMS
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BMS>,          SwitchScreenAction<BmsMenu>, StaticMenuItemIcon<&icons::bms>>>();
#endif
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETTINGS>,     SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&icons::settings>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LOCKVEHICLE>,  SwitchScreenAction<Lockscreen>, StaticMenuItemIcon<&icons::lock>>>();
#ifdef FEATURE_MOSFETS
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOSFETS>,      SwitchScreenAction<MosfetsMenu>>>();
#endif
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEMOS>,        SwitchScreenAction<DemosMenu>, StaticMenuItemIcon<&icons::demos>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_POWEROFF>,     SwitchScreenAction<PoweroffDisplay>, StaticMenuItemIcon<&icons::poweroff>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REBOOT>,       RebootAction, StaticMenuItemIcon<&icons::reboot>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEBUG>,        SwitchScreenAction<DebugMenu>>>();
    }
};
} // namespace
