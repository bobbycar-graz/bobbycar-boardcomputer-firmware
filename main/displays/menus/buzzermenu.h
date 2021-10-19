#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "actions/toggleboolaction.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "texts.h"
#include "globals.h"
#include "accessors/settingsaccessors.h"

// forward declares
class BuzzerMenu;
class SettingsMenu;

class BuzzerMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_BUZZER>,
    public espgui::BackActionInterface<espgui::SwitchScreenAction<SettingsMenu>>
{
public:
    BuzzerMenu();
};
