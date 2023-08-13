#pragma once

// local includes
#include "bobbydisplaywithtitle.h"

class BatteryGraphDisplay : public BobbyDisplayWithTitle {
    using Base = BobbyDisplayWithTitle;

public:
    std::string title() const override;
    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;

    static void drawBatteryCurve();

private:
    float m_lastBatVoltage{0};
};
