#pragma once

// local includes
#include "actioninterface.h"
#include "menudisplay.h"
#include "utils.h"
#include "actions/multiaction.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"
#include "settings.h"
#include "presets.h"
#include "globals.h"

// forward declares
namespace {
class MainMenu;
} // namespace

namespace {
class ApplyPresetAction : public virtual ActionInterface
{
public:
    void triggered() override { saveSettings(); switchScreen<MainMenu>(); }
};

template<const Settings *preset>
class ApplySettingsPresetAction : public virtual ApplyPresetAction
{
public:
    void triggered() override { settings = *preset; ApplyPresetAction::triggered(); }
};

template<const Settings::Limits *preset>
class ApplyLimitsPresetAction : public virtual ApplyPresetAction
{
public:
    void triggered() override { settings.limits = *preset; ApplyPresetAction::triggered(); }
};

template<const Settings::ControllerHardware *preset>
class ApplyControllerHardwarePresetAction : public virtual ApplyPresetAction
{
public:
    void triggered() override { settings.controllerHardware = *preset; ApplyPresetAction::triggered(); }
};

template<const Settings::BoardcomputerHardware *preset>
class ApplyBoardcomputerHardwarePresetAction : public virtual ApplyPresetAction
{
public:
    void triggered() override { settings.boardcomputerHardware = *preset; ApplyPresetAction::triggered(); }
};

template<const Settings::DefaultMode *preset>
class ApplyDefaultModePresetAction : public virtual ApplyPresetAction
{
public:
    void triggered() override { settings.defaultMode = *preset; ApplyPresetAction::triggered(); }
};

template<const Settings::TempomatMode *preset>
class ApplyTempomatModePresetAction : public virtual ApplyPresetAction
{
public:
    void triggered() override { settings.tempomatMode = *preset; ApplyPresetAction::triggered(); }
};

template<const Settings::LarsmMode *preset>
class ApplyLarsmModePresetAction : public virtual ApplyPresetAction
{
public:
    void triggered() override { settings.larsmMode = *preset; ApplyPresetAction::triggered(); }
};

class PresetsMenu :
    public MenuDisplay,
    public StaticText<TEXT_PRESETS>,
    public BackActionInterface<SwitchScreenAction<MainMenu>>
{
public:
    PresetsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULTEVERYTHING>,            MultiAction<ApplySettingsPresetAction<&presets::defaultSettings>, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULTLIMITS>,                MultiAction<ApplyLimitsPresetAction<&presets::defaultLimits>, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_KIDSLIMITS>,                   MultiAction<ApplyLimitsPresetAction<&presets::kidsLimits>, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULTCONTROLLERHARDWARE>,    MultiAction<ApplyControllerHardwarePresetAction<&presets::defaultControllerHardware>, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOSFETSOFFCONTROLLERHARDWARE>, MultiAction<ApplyControllerHardwarePresetAction<&presets::mosfetsOffControllerHardware>, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SPINNERCONTROLLERHARDWARE>,    MultiAction<ApplyControllerHardwarePresetAction<&presets::spinnerControllerHardware>, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULTBOARDCOMPUTERHARDWARE>, MultiAction<ApplyBoardcomputerHardwarePresetAction<&presets::defaultBoardcomputerHardware>, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULTDEFAULTMODE>,           MultiAction<ApplyDefaultModePresetAction<&presets::defaultDefaultMode>, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SINUSOIDALDEFAULTMODE>,        MultiAction<ApplyDefaultModePresetAction<&presets::sinusoidalDefaultMode>, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULTTEMPOMATMODE>,          MultiAction<ApplyTempomatModePresetAction<&presets::defaultTempomatMode>, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULTLARSMMODE>,             MultiAction<ApplyLarsmModePresetAction<&presets::defaultLarsmMode>, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                         SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
} // namespace
