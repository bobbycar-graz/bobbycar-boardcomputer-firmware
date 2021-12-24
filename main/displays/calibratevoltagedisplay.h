#pragma once

// Local includes
#include "menudisplay.h"
#include "utilities/utils.h"
#include "menuitem.h"
#include "icons/back.h"
#include "icons/settings.h"
#include "texts.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "battery.h"
#include "menus/batterymenu.h"
#include "widgets/label.h"
#include "globals.h"

class CalibrateVoltageDisplay :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_BATTERY_CALIBRATE>
{
public:
    CalibrateVoltageDisplay();

    void back() override;
};
