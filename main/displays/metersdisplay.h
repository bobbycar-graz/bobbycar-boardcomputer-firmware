#pragma once

#include <array>

#include <Arduino.h>

#include "display.h"
#include "actions/switchscreenaction.h"
#include "globals.h"
#include "utils.h"
#include "widgets/label.h"
#include "widgets/reverseprogressbar.h"
#include "widgets/progressbar.h"
#include "widgets/verticalmeter.h"
#include "widgets/vumeter.h"

namespace {
class MainMenu;
class StatusDisplay;
class BmsDisplay;
}

namespace {
class MetersDisplay : public Display, public ConfirmActionInterface<SwitchScreenAction<MainMenu>>, public BackActionInterface<SwitchScreenAction<MainMenu>>
{
public:
    void initScreen() override;
    void redraw() override;

    void rotate(int offset) override;

private:
    VuMeter m_vuMeter;

    ReverseProgressBar m_dischargingBar{10, 135, 90, 15, 0, 40, TFT_GREEN};
    ProgressBar m_chargingBar{100, 135, 90, 15, 0, 40, TFT_RED};

    Label m_sumCurrentLabel{195,135};

    static constexpr auto x = 40;
    std::array<VerticalMeter, 6> meters{{
        VerticalMeter{"U f", "%.1f", 0*x, 160},
        VerticalMeter{"U b", "%.1f", 1*x, 160},
        VerticalMeter{"Ibl", "%.1f", 2*x, 160},
        VerticalMeter{"Ibr", "%.1f", 3*x, 160},
        VerticalMeter{"Ihl", "%.1f", 4*x, 160},
        VerticalMeter{"Ihr", "%.1f", 5*x, 160}
    }};
};

void MetersDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);

    m_vuMeter.start();

    m_dischargingBar.start();
    m_chargingBar.start();

    m_sumCurrentLabel.start();

    for (auto &meter : meters)
        meter.start();
}

void MetersDisplay::redraw()
{
    m_vuMeter.redraw(avgSpeedKmh);

    m_dischargingBar.redraw(sumCurrent<0.f?(-sumCurrent):0.f);
    m_chargingBar.redraw(sumCurrent>0.f?sumCurrent:0.f);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextFont(2);
    m_sumCurrentLabel.redraw(std::to_string(sumCurrent) + 'A');

    meters[0].redraw(fixBatVoltage(controllers.front.feedback.batVoltage), 35, 50);
    meters[1].redraw(fixBatVoltage(controllers.back.feedback.batVoltage), 35, 50);
    meters[2].redraw(fixCurrent(controllers.front.feedback.left.dcLink), -10, 10);
    meters[3].redraw(fixCurrent(controllers.front.feedback.right.dcLink), -10, 10);
    meters[4].redraw(fixCurrent(controllers.back.feedback.left.dcLink), -10, 10);
    meters[5].redraw(fixCurrent(controllers.back.feedback.right.dcLink), -10, 10);
}

void MetersDisplay::rotate(int offset)
{
    if (offset < 0)
        switchScreen<StatusDisplay>();
    else if (offset > 0)
#ifdef FEATURE_BMS
        switchScreen<BmsDisplay>();
#else
        switchScreen<StatusDisplay>();
#endif
}
}
