#pragma once

#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "menuitem.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"
#include "debugtexthelpers.h"

#ifdef GLUMP_CONTROLLER
namespace {
class DebugMenu;
}

namespace {
template<const char *Ttext, typename Ttexts>
class MotorStateDebugMenu :
    public MenuDisplay,
    public StaticText<Ttext>,
    public BackActionInterface<SwitchScreenAction<DebugMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, typename Ttexts::EnableText,       DisabledColor, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::PwmText,          DisabledColor, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::CtrlTypText,      DisabledColor, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::CtrlModText,      DisabledColor, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::IMotMaxText,      DisabledColor, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::IDcMaxText,       DisabledColor, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::NMotMaxText,      DisabledColor, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::FieldWeakMaxText, DisabledColor, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::PhaseAdvMaxText,  DisabledColor, DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,    SwitchScreenAction<DebugMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};

class FrontLeftMotorStateDebugMenu : public MotorStateDebugMenu<TEXT_FRONTLEFTCOMMAND, FrontTexts::LeftCommand> {};
class FrontRightMotorStateDebugMenu : public MotorStateDebugMenu<TEXT_FRONTRIGHTCOMMAND, FrontTexts::RightCommand> {};
class BackLeftMotorStateDebugMenu : public MotorStateDebugMenu<TEXT_BACKLEFTCOMMAND, BackTexts::LeftCommand> {};
class BackRightMotorStateDebugMenu : public MotorStateDebugMenu<TEXT_BACKRIGHTCOMMAND, BackTexts::RightCommand> {};
}
#endif