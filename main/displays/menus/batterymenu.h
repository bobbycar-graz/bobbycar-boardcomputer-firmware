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
#include "selectbatterytypemenu.h"
#include "widgets/doubleprogressbar.h"

class BatteryMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_BATTERY>
{
    using Base = espgui::MenuDisplay;
public:
    BatteryMenu();

    void start() override;
    void redraw() override;
    void back() override;
private:
    bobbygui::DoubleProgressBar m_doubleProgressBarBatPercentage{75, 68, 90, 24, 0, 100, TFT_RED, TFT_GREEN};
};
