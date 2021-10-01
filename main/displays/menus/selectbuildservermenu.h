#pragma once

// Local includes
#include "menudisplay.h"
#include "utils.h"
#include "menuitem.h"
#include "icons/back.h"
#include "texts.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "actions/selectotaactions.h"
#include "otadefine.h"

using namespace espgui;

namespace  {
    class SettingsMenu;
}

namespace  {
    class BuildserverMenu :
            public MenuDisplay,
            public StaticText<TEXT_SELECTBUILDSERVER>,
            public BackActionInterface<SwitchScreenAction<SettingsMenu>>
    {
    public:
        BuildserverMenu()
        {
            constructMenuItem<makeComponent<MenuItem, ChangeableText,             SelectComrServerAction>>();
            constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                         SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
        }
    };
} // Namespace
