#pragma once

#include "actioninterface.h"
#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"
#include "settings.h"
#include "presets.h"
#include "globals.h"

namespace {
class MainMenu;
}

namespace {
template<const Settings *preset>
class ApplyPresetAction : public virtual ActionInterface
{
public:
    void triggered() override { settings = *preset; saveSettings(); }
};

template<const Settings::Limits *preset>
class ApplyLimitsPresetAction : public virtual ActionInterface
{
public:
    void triggered() override { settings.limits = *preset; saveSettings(); }
};

template<const Settings::ControllerHardware *preset>
class ApplyControllerHardwarePresetAction : public virtual ActionInterface
{
public:
    void triggered() override { settings.controllerHardware = *preset; saveSettings(); }
};

template<const Settings::BoardcomputerHardware *preset>
class ApplyBoardcomputerHardwarePresetAction : public virtual ActionInterface
{
public:
    void triggered() override { settings.boardcomputerHardware = *preset; saveSettings(); }
};

template<const Settings::DefaultMode *preset>
class ApplyDefaultModePresetAction : public virtual ActionInterface
{
public:
    void triggered() override { settings.defaultMode = *preset; saveSettings(); }
};

class PresetsMenu :
    public MenuDisplay,
    public StaticText<TEXT_PRESETS>,
    public BackActionInterface<SwitchScreenAction<MainMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_DEFAULTEVERYTHING>,            ApplyPresetAction<&presets::defaultSettings>>,
        makeComponent<MenuItem, StaticText<TEXT_DEFAULTLIMITS>,                ApplyLimitsPresetAction<&presets::defaultLimits>>,
        makeComponent<MenuItem, StaticText<TEXT_KIDSLIMITS>,                   ApplyLimitsPresetAction<&presets::kidsLimits>>,
        makeComponent<MenuItem, StaticText<TEXT_DEFAULTCONTROLLERHARDWARE>,    ApplyControllerHardwarePresetAction<&presets::defaultControllerHardware>>,
        makeComponent<MenuItem, StaticText<TEXT_MOSFETSOFFCONTROLLERHARDWARE>, ApplyControllerHardwarePresetAction<&presets::mosfetsOffControllerHardware>>,
        makeComponent<MenuItem, StaticText<TEXT_SPINNERCONTROLLERHARDWARE>,    ApplyControllerHardwarePresetAction<&presets::spinnerControllerHardware>>,
        makeComponent<MenuItem, StaticText<TEXT_DEFAULTBOARDCOMPUTERHARDWARE>, ApplyBoardcomputerHardwarePresetAction<&presets::defaultBoardcomputerHardware>>,
        makeComponent<MenuItem, StaticText<TEXT_DEFAULTDEFAULTMODE>,           ApplyDefaultModePresetAction<&presets::defaultDefaultMode>>,
        makeComponent<MenuItem, StaticText<TEXT_SINUSODIALDEFAULTMODE>,        ApplyDefaultModePresetAction<&presets::sinusodialDefaultMode>>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,                         SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}
