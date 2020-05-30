#pragma once

#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "menuitem.h"
#include "actions/toggleboolaction.h"
#include "actions/switchscreenaction.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "texts.h"
#include "settingsaccessors.h"

namespace {
class ControllerHardwareSettingsMenu;
}

namespace {
class EnableMenu :
    public MenuDisplay,
    public StaticText<TEXT_SETENABLED>,
    public BackActionInterface<SwitchScreenAction<ControllerHardwareSettingsMenu>>,
    public StaticMenuDefinition<
#ifdef GLUMP_CONTROLLER
        makeComponent<MenuItem, StaticText<TEXT_ENABLEFRONTLEFT>,  ToggleBoolAction, CheckboxIcon, FrontLeftEnabledAccessor>,
        makeComponent<MenuItem, StaticText<TEXT_ENABLEFRONTRIGHT>, ToggleBoolAction, CheckboxIcon, FrontRightEnabledAccessor>,
        makeComponent<MenuItem, StaticText<TEXT_ENABLEBACKLEFT>,   ToggleBoolAction, CheckboxIcon, BackLeftEnabledAccessor>,
        makeComponent<MenuItem, StaticText<TEXT_ENABLEBACKRIGHT>,  ToggleBoolAction, CheckboxIcon, BackRightEnabledAccessor>,
#endif
#ifdef VESC_CONTROLLER
        makeComponent<MenuItem, StaticText<TEXT_ENABLEBACKRIGHT>,  ToggleBoolAction, CheckboxIcon, OneEnabledAccessor>,
        makeComponent<MenuItem, StaticText<TEXT_ENABLEBACKRIGHT>,  ToggleBoolAction, CheckboxIcon, TwoEnabledAccessor>,
#endif
        makeComponent<MenuItem, StaticText<TEXT_BACK>,             SwitchScreenAction<ControllerHardwareSettingsMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}