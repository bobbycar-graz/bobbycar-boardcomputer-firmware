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
class CommandDebugMenu :
    public MenuDisplay,
    public StaticText<Ttext>,
    public BackActionInterface<SwitchScreenAction<DebugMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, typename Ttexts::BuzzerFreqText,    DisabledColor, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::BuzzerPatternText, DisabledColor, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::PoweroffText,      DisabledColor, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::LedText,           DisabledColor, DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,              SwitchScreenAction<DebugMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};

class FrontCommandDebugMenu : public CommandDebugMenu<TEXT_FRONTCOMMAND, FrontTexts> {};
class BackCommandDebugMenu : public CommandDebugMenu<TEXT_BACKCOMMAND, BackTexts> {};
}
#endif
