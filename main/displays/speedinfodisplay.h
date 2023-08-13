#pragma once

// 3rdparty lib includes
#include <widgets/label.h>
#include <widgets/progressbar.h>
#include <widgets/reverseprogressbar.h>

// local includes
#include "bobbydisplay.h"

class SpeedInfoDisplay : public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;
private:
    espgui::Label m_labelSpeed{5, 5};

    espgui::ReverseProgressBar m_dischargingBar{10, 110, tft.width()/2 - 10, 25, 0, 40, TFT_GREEN};
    espgui::ProgressBar m_chargingBar{tft.width()/2, 110, tft.width()/2 - 10, 25, 0, 40, TFT_RED};

#define START_Y 150
    espgui::Label m_batteryPercentLabel{5, START_Y};
    espgui::Label m_voltageLabel{5, START_Y + 29 * 1};
    espgui::Label m_distanceLabel{5, START_Y + 29 * 2};
    espgui::Label m_currentPowerLabel{5, START_Y + 29 * 3};
};
