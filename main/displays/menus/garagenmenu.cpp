#if defined(FEATURE_GARAGE) && defined (FEATURE_ESPNOW)
#include "garagenmenu.h"

// 3rd party libs
#include <icons/back.h>
#include <menuitem.h>

// local includes
#include "actions/switchscreenaction.h"
#include "displays/menus/mainmenu.h"

using namespace espgui;

GarageMenu::GarageMenu()
{

}

void GarageMenu::back()
{
    switchScreen<MainMenu>();
}
#endif
