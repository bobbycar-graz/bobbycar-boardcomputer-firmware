#pragma once

#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"

namespace {
class DefaultModeSettingsMenu;
class TempomatModeSettingsMenu;
#ifdef GLUMP_CONTROLLER
class LarsmModeSettingsMenu;
#endif
class GametrakModeSettingsMenu;
class SettingsMenu;
}

namespace {
class ModesSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_MODESSETTINGS>,
    public BackActionInterface<SwitchScreenAction<SettingsMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_DEFAULTMODESETTIGNS>,   SwitchScreenAction<DefaultModeSettingsMenu>>,
        makeComponent<MenuItem, StaticText<TEXT_TEMPOMATMODESETTINGS>,  SwitchScreenAction<TempomatModeSettingsMenu>>,
#ifdef GLUMP_CONTROLLER
        makeComponent<MenuItem, StaticText<TEXT_LARSMMODESETTINGS>,     SwitchScreenAction<LarsmModeSettingsMenu>>,
#endif
#ifdef FEATURE_GAMETRAK
        makeComponent<MenuItem, StaticText<TEXT_GAMETRAKMODESETTINGS>,  SwitchScreenAction<GametrakModeSettingsMenu>>,
#endif
        makeComponent<MenuItem, StaticText<TEXT_BACK>,                  SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}
