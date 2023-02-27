#pragma once

// 3rdparty lib includes
#include <tftinterface.h>

// local includes
#include "guihelpers/bobbydisplaywithtitle.h"

namespace bobby {

class BatteryGraphDisplay : public BobbyDisplayWithTitle {
    using Base = BobbyDisplayWithTitle;

public:
    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;

    std::string text() const override;

    void buttonPressed(espgui::Button button) override;

    static void drawBatteryCurve(espgui::TftInterface &tft);

private:
    float m_lastBatVoltage{0};
};

} // namespace bobby
