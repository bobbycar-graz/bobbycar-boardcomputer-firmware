#pragma once

// local includes
#include "menudisplay.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"

// forward declares
class DefaultModeSettingsMenu;
class TempomatModeSettingsMenu;
class LarsmModeSettingsMenu;
class GametrakModeSettingsMenu;
class SettingsMenu;

class ModesSettingsMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_MODESSETTINGS>,
    public espgui::BackActionInterface<espgui::SwitchScreenAction<SettingsMenu>>
{
public:
    ModesSettingsMenu();
};
