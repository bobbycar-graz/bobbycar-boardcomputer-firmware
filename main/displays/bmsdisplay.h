#pragma once

#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
// 3rdparty lib includes
#include <fmt/core.h>
#include <widgets/label.h>
#include <screenmanager.h>

// local includes
#include "globals.h"
#include "bmsutils.h"
#include "bobbydisplay.h"

class BmsDisplay :
    public BobbyDisplay,
    public espgui::DummyBack
{
    using Base = BobbyDisplay;

public:
    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;

    espgui::Label m_statusLabel{200, 0};

    espgui::Label m_voltageLabel{107, 0};
    espgui::Label m_capacityLabel{107, 25};
    espgui::Label m_socLabel{107, 50};
    espgui::Label m_powerLabel{107, 75};
    espgui::Label m_currentLabel{107, 100};
    espgui::Label m_speedLabel{107, 125};
    espgui::Label m_powerPerSpeedLabel{107, 150};

    std::array<espgui::Label, 12> m_battLabels{{
        espgui::Label{0, 225}, espgui::Label{60, 225}, espgui::Label{120, 225}, espgui::Label{180, 225},
        espgui::Label{0, 250}, espgui::Label{60, 250}, espgui::Label{120, 250}, espgui::Label{180, 250},
        espgui::Label{0, 275}, espgui::Label{60, 275}, espgui::Label{120, 275}, espgui::Label{180, 275}
    }};

    espgui::Label m_cycleLabel{105, 300};
};
#endif
