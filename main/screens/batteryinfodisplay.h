#pragma once

// 3rdparty lib includes
#include <tftinterface.h>

// local includes
#include "guihelpers/bobbydisplay.h"

namespace bobby {

class BatteryInfoDisplay : public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;
private:
    static constexpr const auto m_offset = 40;
    uint16_t m_lastBarCount{0};
};

} // namespace bobby
