#pragma once

// 3rdparty lib includes
#include <tftinterface.h>
#include <widgets/label.h>

// local includes
#include "guihelpers/bobbydisplay.h"

#if defined(FEATURE_CAN) && defined(FEATURE_POWERSUPPLY)

namespace bobby {

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

} // namespace bobby
#endif
