#pragma once

// local includes
#include "menudisplay.h"
#include "utilities/utils.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"

using namespace espgui;

namespace {
#ifdef FEATURE_GAMETRAK
class ContainerModeSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_GAMETRAKMODESETTINGS>,
    public BackActionInterface<SwitchScreenAction<ModesSettingsMenu>>
{
public:
    ContainerModeSettingsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<ModesSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
    }
};
#endif
} // namespace
