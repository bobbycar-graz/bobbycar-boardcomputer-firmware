#pragma once

// 3rdparty lib includes
#include <widgets/label.h>

// local includes
#include "bobbydisplay.h"

#if defined(FEATURE_CAN) && defined(FEATURE_POWERSUPPLY)
class PowerSupplyDisplay : public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;

    espgui::Label m_voltageLabel{120, 50};
    espgui::Label m_currentLabel{120, 75};
};
#endif
