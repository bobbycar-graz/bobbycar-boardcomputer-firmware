#pragma once

#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
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

namespace {
class StatusDisplay;
class SelectModeMenu;
class PresetsMenu;
class GraphsMenu;
class BmsMenu;
class SettingsMenu;
class Lockscreen;
class MosfetsMenu;
class DemosMenu;
class PoweroffDisplay;
class DebugMenu;
}

namespace {
class MainMenu :
    public MenuDisplay,
    public StaticText<TEXT_MAINMENU>,
    public BackActionInterface<SwitchScreenAction<StatusDisplay>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_STATUS>,      SwitchScreenAction<StatusDisplay>, StaticMenuItemIcon<&icons::back>>,
#ifdef GLUMP_CONTROLLER
        makeComponent<MenuItem, StaticText<TEXT_SELECTMODE>,  SwitchScreenAction<SelectModeMenu>, StaticMenuItemIcon<&icons::modes>>,
#endif
        makeComponent<MenuItem, StaticText<TEXT_PRESETS>,     SwitchScreenAction<PresetsMenu>, StaticMenuItemIcon<&icons::presets>>,
        makeComponent<MenuItem, StaticText<TEXT_GRAPHS>,      SwitchScreenAction<GraphsMenu>, StaticMenuItemIcon<&icons::graph>>,
#ifdef FEATURE_BMS
        makeComponent<MenuItem, StaticText<TEXT_BMS>,         SwitchScreenAction<BmsMenu>, StaticMenuItemIcon<&icons::bms>>,
#endif
        makeComponent<MenuItem, StaticText<TEXT_SETTINGS>,    SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&icons::settings>>,
        makeComponent<MenuItem, StaticText<TEXT_LOCKVEHICLE>, SwitchScreenAction<Lockscreen>, StaticMenuItemIcon<&icons::lock>>,
#ifdef FEATURE_MOSFETS
        makeComponent<MenuItem, StaticText<TEXT_MOSFETS>,     SwitchScreenAction<MosfetsMenu>, StaticMenuItemIcon<&icons::lock>>, // TODO icon
#endif
        makeComponent<MenuItem, StaticText<TEXT_DEMOS>,       SwitchScreenAction<DemosMenu>, StaticMenuItemIcon<&icons::demos>>,
#ifdef GLUMP_CONTROLLER
        makeComponent<MenuItem, StaticText<TEXT_POWEROFF>,    SwitchScreenAction<PoweroffDisplay>, StaticMenuItemIcon<&icons::poweroff>>,
#endif
        makeComponent<MenuItem, StaticText<TEXT_REBOOT>,      RebootAction, StaticMenuItemIcon<&icons::reboot>>,
        makeComponent<MenuItem, StaticText<TEXT_DEBUG>,       SwitchScreenAction<DebugMenu>>
    >
{};
};
