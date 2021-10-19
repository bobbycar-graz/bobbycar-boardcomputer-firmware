#pragma once

// 3rdparty lib includes
#include <fmt/core.h>

// local includes
#include "menudisplay.h"
#include "menuitem.h"
#include "changevaluedisplay.h"
#include "actions/switchscreenaction.h"
#include "actions/toggleboolaction.h"
#include "checkboxicon.h"
#include "cloudtexthelpers.h"
#include "accessors/settingsaccessors.h"
#include "icons/back.h"
#include "texts.h"
#include "accessors/settingsaccessors.h"
#include "cloud.h"

// forward declares
class CloudSettingsMenu;
class SettingsMenu;

#ifdef FEATURE_CLOUD
class CloudSettingsMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_CLOUDSETTINGS>,
    public espgui::BackActionInterface<espgui::SwitchScreenAction<SettingsMenu>>
{
public:
    CloudSettingsMenu();
};
#endif
