#include "otamenu.h"

// 3rdparty lib includes
#include "actioninterface.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"

// local includes
#include "icons/back.h"
#include "icons/presets.h"
#include "icons/update.h"
#include "displays/menus/selectotabuildmenu.h"
#include "displays/menus/selectbuildservermenu.h"
#include "displays/menus/mainmenu.h"
#include "displays/updatedisplay.h"

#ifdef FEATURE_OTA

using namespace espgui;

OtaMenu::OtaMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTBUILD>,             SwitchScreenAction<SelectBuildMenu>, StaticMenuItemIcon<&bobbyicons::presets>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UPDATENOW>,               SwitchScreenAction<UpdateDisplay>, StaticMenuItemIcon<&bobbyicons::update>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTBUILDSERVERMENU>,   SwitchScreenAction<SelectBuildServerMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                    SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void OtaMenu::back()
{
    switchScreen<MainMenu>();
}
#endif
