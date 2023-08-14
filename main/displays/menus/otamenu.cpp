#include "otamenu.h"

// 3rdparty lib includes
#include "actioninterface.h"
#include "actions/dummyaction.h"
#include "actions/pushscreenaction.h"
#include "actions/popscreenaction.h"

// local includes
#include "icons/back.h"
#include "icons/presets.h"
#include "icons/update.h"
#include "displays/menus/selectotabuildmenu.h"
#include "displays/menus/selectbuildserverbranch.h"
#include "displays/menus/selectbuildservermenu.h"
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
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTBUILD>,             PushScreenAction<SelectBuildMenu>, StaticMenuItemIcon<&bobbyicons::presets>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECT_BRANCH>,           PushScreenAction<SelectBuildserverBranchMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UPDATENOW>,               PushScreenAction<UpdateDisplay>, StaticMenuItemIcon<&bobbyicons::update>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTBUILDSERVERMENU>,   PushScreenAction<SelectBuildServerMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                    PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string OtaMenu::title() const
{
    return TEXT_UPDATE;
}

void OtaMenu::back()
{
    espgui::popScreen();
}
