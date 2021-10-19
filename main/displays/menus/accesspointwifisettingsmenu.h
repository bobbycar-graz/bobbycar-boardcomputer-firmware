#pragma once

// local includes
#include "menudisplay.h"
#include "menuitem.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "wifitexthelpers.h"
#include "texts.h"

// forward declares
class WifiSettingsMenu;

class AccessPointWifiSettingsMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_ACCESSPOINTWIFISETTINGS>,
    public espgui::BackActionInterface<espgui::SwitchScreenAction<WifiSettingsMenu>>
{
public:
    AccessPointWifiSettingsMenu();
};
