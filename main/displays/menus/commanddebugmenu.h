#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "menuitem.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"
#include "debugtexthelpers.h"

// forward declares
class DebugMenu;

template<const char *Ttext, typename Ttexts>
class CommandDebugMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<Ttext>,
    public espgui::BackActionInterface<espgui::SwitchScreenAction<DebugMenu>>
{
public:
    CommandDebugMenu();
};

class FrontCommandDebugMenu : public CommandDebugMenu<TEXT_FRONTCOMMAND, FrontTexts> {};
class BackCommandDebugMenu : public CommandDebugMenu<TEXT_BACKCOMMAND, BackTexts> {};
