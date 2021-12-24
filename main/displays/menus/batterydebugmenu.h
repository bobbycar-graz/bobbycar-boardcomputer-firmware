#pragma once

// 3rdparty lib includes
#include <menudisplay.h>
#include <menuitem.h>
#include <icons/back.h>
#include <actions/dummyaction.h>
#include <actions/switchscreenaction.h>

// Local includes
#include "utilities/utils.h"
#include "icons/settings.h"
#include "texts.h"
#include "battery.h"

class BatteryDebugMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_BATTERYDEBUG>
{
public:
    BatteryDebugMenu();

    void back() override;
};
