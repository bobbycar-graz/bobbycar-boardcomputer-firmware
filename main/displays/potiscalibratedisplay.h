#pragma once

// system includes
#include <array>
#include <string>

// 3rdparty lib includes
#include <fmt/core.h>
#include <cpputils.h>
#include <actions/switchscreenaction.h>
#include <widgets/label.h>
#include <widgets/progressbar.h>

// local includes
#include "bobbydisplaywithtitle.h"
#include "modeinterface.h"
#include "modes/ignoreinputmode.h"

class PotisCalibrateDisplay : public BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;

public:
    PotisCalibrateDisplay() = default;
    explicit PotisCalibrateDisplay(bool bootup) : m_bootup{bootup} {}

    std::string text() const override;
    void start() override;
    void initScreen() override;
    void update() override;
    void redraw() override;
    void stop() override;

    void buttonPressed(espgui::Button button) override;

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
#ifdef FEATURE_JOYSTICK
        Mitte,
#endif
        GasMin,
        GasMax,
        BremsMin,
        BremsMax,
        Confirm
    };

    int8_t m_selectedButton, m_renderedButton;

    Status m_status;
    int16_t
        m_gasMin,
        m_gasMax,
        m_bremsMin,
        m_bremsMax
#ifdef FEATURE_JOYSTICK
        ,m_gasMitte
        ,m_bremsMitte
#endif
    ;
    std::optional<float> m_gas, m_brems;
};
