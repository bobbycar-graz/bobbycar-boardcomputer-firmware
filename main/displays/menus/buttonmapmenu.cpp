#include "buttonmapmenu.h"

// 3rd party includes
#include <menuitem.h>
#include <icons/back.h>

// local includes
#include "actions/switchscreenaction.h"
#include "displays/menus/boardcomputerhardwaresettingsmenu.h"

using namespace espgui;

ButtonMapMenu::ButtonMapMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<BoardcomputerHardwareSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void ButtonMapMenu::back()
{
    switchScreen<BoardcomputerHardwareSettingsMenu>();
}
