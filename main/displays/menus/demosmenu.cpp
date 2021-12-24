#include "demosmenu.h"

// 3rdparty lib includes
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "displays/starfielddisplay.h"
#include "displays/pingpongdisplay.h"
#include "displays/spirodisplay.h"
#include "displays/gameoflifedisplay.h"
#include "displays/menus/mainmenu.h"

// local includes
#include "utilities/utils.h"

using namespace espgui;

DemosMenu::DemosMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STARFIELD>,  SwitchScreenAction<StarfieldDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_PINGPONG>,   SwitchScreenAction<PingPongDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SPIRO>,      SwitchScreenAction<SpiroDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GAMEOFLIFE>, SwitchScreenAction<GameOfLifeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,       SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void DemosMenu::back()
{
    switchScreen<MainMenu>();
}
