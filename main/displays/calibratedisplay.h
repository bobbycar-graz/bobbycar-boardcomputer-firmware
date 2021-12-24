#pragma once

// system includes
#include <array>
#include <string>

// 3rdparty lib includes
#include <fmt/core.h>
#include <cpputils.h>
#include <displaywithtitle.h>
#include <actions/switchscreenaction.h>
#include <widgets/label.h>
#include <widgets/progressbar.h>

// local includes
#include "globals.h"
#include "utilities/utils.h"
#include "texts.h"
#include "modes/ignoreinputmode.h"

class CalibrateDisplay : public espgui::DisplayWithTitle
{
    using Base = espgui::DisplayWithTitle;

public:
    CalibrateDisplay() = default;
    CalibrateDisplay(bool bootup);

    std::string text() const override;
    void start() override;
    void initScreen() override;
    void update() override;
    void redraw() override;
    void stop() override;

    void rotate(int offset) override;

    void confirm() override;
    void back() override;

private:
    void copyFromSettings();
    void copyToSettings();

    const bool m_bootup{false};
    ModeInterface *m_oldMode;
    IgnoreInputMode m_mode{0, bobbycar::protocol::ControlType::FieldOrientedControl, bobbycar::protocol::ControlMode::Torque};

    std::array<espgui::Label, 11> m_labels {{
        espgui::Label{25, 72}, // 100, 23
        espgui::Label{145, 72}, // 100, 23
        espgui::Label{25, 97}, // 100, 23
        espgui::Label{145, 97}, // 100, 23

        espgui::Label{25, 172}, // 100, 23
        espgui::Label{145, 172}, // 100, 23
        espgui::Label{25, 197}, // 100, 23
        espgui::Label{145, 197}, // 100, 23

        espgui::Label{25, 247}, // 190, 23

        espgui::Label{25, 277}, // 100, 23
        espgui::Label{145, 277}, // 100, 23
    }};

    std::array<espgui::ProgressBar, 2> m_progressBars {{
        espgui::ProgressBar{20, 129, 200, 10, 0, 1000},
        espgui::ProgressBar{20, 229, 200, 10, 0, 1000}
    }};

    enum Status {
        Begin,
        GasMin,
        GasMax,
        BremsMin,
        BremsMax,
        Confirm
    };

    int8_t m_selectedButton, m_renderedButton;

    Status m_status;
    int16_t m_gasMin, m_gasMax, m_bremsMin, m_bremsMax;
    std::optional<float> m_gas, m_brems;
};
