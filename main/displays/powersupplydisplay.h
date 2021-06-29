#pragma once

#include <Arduino.h>

#include "display.h"
#include "actions/switchscreenaction.h"
#include "globals.h"
#include "widgets/label.h"

namespace {
class MainMenu;
class MetersDisplay;
class StatusDisplay;
}
namespace {
#ifdef FEATURE_CAN
class PowerSupplyDisplay : public Display
{
public:
    void initScreen() override;
    void redraw() override;

    void confirm() override;
    void back() override;
    void rotate(int offset) override;

    Label m_voltageLabel{120, 50};
    Label m_currentLabel{120, 75};
};

void PowerSupplyDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.setTextFont(4);

    tft.drawString("Voltage:", 0, m_voltageLabel.y());
    m_voltageLabel.start();
    tft.drawString("Current:", 0, m_currentLabel.y());
    m_currentLabel.start();
}

void PowerSupplyDisplay::redraw()
{
    m_voltageLabel.redraw(std::to_string(50.4) + 'V');
    m_currentLabel.redraw(std::to_string(15.1) + 'A');
}

void PowerSupplyDisplay::confirm()
{

}

void PowerSupplyDisplay::back()
{

}

void PowerSupplyDisplay::rotate(int offset)
{
//    if (offset < 0)
//        switchScreen<MetersDisplay>();
//    else if (offset > 0)
//        switchScreen<StatusDisplay>();
}
#endif
}
