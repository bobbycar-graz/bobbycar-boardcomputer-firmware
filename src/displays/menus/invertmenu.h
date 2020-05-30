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
class InvertMenu :
    public MenuDisplay,
    public StaticText<TEXT_SETINVERTED>,
    public BackActionInterface<SwitchScreenAction<ControllerHardwareSettingsMenu>>,
    public StaticMenuDefinition<
#ifdef GLUMP_CONTROLLER
        makeComponent<MenuItem, StaticText<TEXT_INVERTFRONTLEFT>,  ToggleBoolAction, CheckboxIcon, FrontLeftInvertedAccessor>,
        makeComponent<MenuItem, StaticText<TEXT_INVERTFRONTRIGHT>, ToggleBoolAction, CheckboxIcon, FrontRightInvertedAccessor>,
        makeComponent<MenuItem, StaticText<TEXT_INVERTBACKLEFT>,   ToggleBoolAction, CheckboxIcon, BackLeftInvertedAccessor>,
        makeComponent<MenuItem, StaticText<TEXT_INVERTBACKRIGHT>,  ToggleBoolAction, CheckboxIcon, BackRightInvertedAccessor>,
#endif
#ifdef VESC_CONTROLLER
        makeComponent<MenuItem, StaticText<TEXT_INVERTFRONTLEFT>,  ToggleBoolAction, CheckboxIcon, OneInvertedAccessor>,
        makeComponent<MenuItem, StaticText<TEXT_INVERTFRONTRIGHT>, ToggleBoolAction, CheckboxIcon, TwoInvertedAccessor>,
#endif
        makeComponent<MenuItem, StaticText<TEXT_BACK>,             SwitchScreenAction<ControllerHardwareSettingsMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}