#pragma once

// Arduino includes
#include <Arduino.h>

// local includes
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
#if defined(FEATURE_CAN) && defined(FEATURE_POWERSUPPLY)
class PowerSupplyDisplay : public Display, public DummyConfirm, public BackActionInterface<SwitchScreenAction<MainMenu>>
{
public:
    void initScreen() override;
    void redraw() override;

    void confirm() override;
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
    // TODO
}

void PowerSupplyDisplay::rotate(int offset)
{
    // TODO
}
#endif
}
