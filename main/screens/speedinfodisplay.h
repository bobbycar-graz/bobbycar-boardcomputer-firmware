#pragma once

// 3rdparty lib includes
#include <delayedconstruction.h>
#include <tftinterface.h>
#include <widgets/label.h>
#include <widgets/progressbar.h>
#include <widgets/reverseprogressbar.h>

// local includes
#include "guihelpers/bobbydisplay.h"

namespace bobby {

class SpeedInfoDisplay : public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;
private:
    espgui::Label m_labelSpeed{5, 5};

    cpputils::DelayedConstruction<espgui::ReverseProgressBar> m_dischargingBar;
    cpputils::DelayedConstruction<espgui::ProgressBar> m_chargingBar;

#define START_Y 150
    espgui::Label m_batteryPercentLabel{5, START_Y};
    espgui::Label m_voltageLabel{5, START_Y + 29 * 1};
    espgui::Label m_distanceLabel{5, START_Y + 29 * 2};
    espgui::Label m_currentPowerLabel{5, START_Y + 29 * 3};
};
} // namespace bobby
