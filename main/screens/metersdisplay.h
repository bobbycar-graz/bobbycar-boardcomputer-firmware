#pragma once

// system includes
#include <array>

// 3rdparty lib includes
#include <actions/switchscreenaction.h>
#include <widgets/label.h>
#include <widgets/progressbar.h>
#include <widgets/reverseprogressbar.h>
#include <widgets/verticalmeter.h>
#include <widgets/vumeter.h>

// local includes
#include "guihelpers/bobbydisplay.h"

namespace bobby {

class MetersDisplay : public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;

private:
    espgui::VuMeter m_vuMeter;

    espgui::ReverseProgressBar m_dischargingBar{10, 135, 90, 15, 0, 40, TFT_GREEN};
    espgui::ProgressBar m_chargingBar{100, 135, 90, 15, 0, 40, TFT_RED};

    espgui::Label m_sumCurrentLabel{195,135};

    static constexpr auto x = 40;
    std::array<espgui::VerticalMeter, 6> meters{{
        espgui::VerticalMeter{"U f", "%.1f", 0*x, 160},
        espgui::VerticalMeter{"U b", "%.1f", 1*x, 160},
        espgui::VerticalMeter{"Ibl", "%.1f", 2*x, 160},
        espgui::VerticalMeter{"Ibr", "%.1f", 3*x, 160},
        espgui::VerticalMeter{"Ihl", "%.1f", 4*x, 160},
        espgui::VerticalMeter{"Ihr", "%.1f", 5*x, 160}
    }};
};
} // namespace bobby
