#pragma once

// local includes
#include "menudisplay.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "actions/toggleboolaction.h"
#include "checkboxicon.h"
#include "bletexthelpers.h"
#include "accessors/settingsaccessors.h"
#include "icons/back.h"
#include "texts.h"

// forward declares
class SettingsMenu;

#ifdef FEATURE_BLE
class BleSettingsMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_BLESETTINGS>,
    public espgui::BackActionInterface<espgui::SwitchScreenAction<SettingsMenu>>
{
public:
    BleSettingsMenu();
};
#endif
