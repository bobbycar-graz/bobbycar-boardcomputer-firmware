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
#include "displays/menus/selectbuildserverbranch.h"
#include "displays/menus/selectbuildservermenu.h"
#include "displays/menus/mainmenu.h"
#include "displays/updatedisplay.h"

namespace {
constexpr char TEXT_UPDATE[] = "Update";
constexpr char TEXT_UPDATENOW[] = "Update now";
constexpr char TEXT_SELECTBUILD[] = "Select build";
constexpr char TEXT_SELECT_BRANCH[] = "Select Branch";
constexpr char TEXT_SELECTBUILDSERVERMENU[] = "Select Buildserver";
constexpr char TEXT_BACK[] = "Back";
} // namespace

OtaMenu::OtaMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTBUILD>,             SwitchScreenAction<SelectBuildMenu>, StaticMenuItemIcon<&bobbyicons::presets>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECT_BRANCH>,           SwitchScreenAction<SelectBuildserverBranchMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UPDATENOW>,               SwitchScreenAction<UpdateDisplay>, StaticMenuItemIcon<&bobbyicons::update>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTBUILDSERVERMENU>,   SwitchScreenAction<SelectBuildServerMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                    SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string OtaMenu::text() const
{
    return TEXT_UPDATE;
}

void OtaMenu::back()
{
    espgui::switchScreen<MainMenu>();
}
