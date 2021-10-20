#pragma once

// local includes
#include "menudisplay.h"
#include "actioninterface.h"
#include "utils.h"
#include "actions/dummyaction.h"
#include "icons/back.h"
#include "icons/update.h"
#include "icons/presets.h"
#include "texts.h"
#include "buildserver.h"


// forward declares
namespace {
class MainMenu;
class UpdateDisplay;
class SelectBuildMenu;
class SelectBuildServerMenu;
} // namespace

using namespace espgui;

namespace {

class RedownloadJsonAction : public virtual ActionInterface {
public:
    void triggered() override {
        redownload = true;
    }
};

class OtaMenu :
    public MenuDisplay,
    public StaticText<TEXT_UPDATE>,
    public BackActionInterface<SwitchScreenAction<MainMenu>>
{
public:
    OtaMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTBUILD>,             SwitchScreenAction<SelectBuildMenu>, StaticMenuItemIcon<&bobbyicons::presets>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UPDATENOW>,               SwitchScreenAction<UpdateDisplay>, StaticMenuItemIcon<&bobbyicons::update>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTBUILDSERVERMENU>,   SwitchScreenAction<SelectBuildServerMenu>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REDOWNLOAD>,   RedownloadJsonAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                    SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
    }
};
} // namespace
