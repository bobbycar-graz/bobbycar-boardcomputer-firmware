#pragma once

// 3rdparty lib includes
#include <widgets/label.h>
#include <widgets/progressbar.h>
#include <espchrono.h>

// local includes
#include "bobbydisplaywithtitle.h"
#include "modes/ignoreinputmode.h"

class ConfiscationDisplay : public BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;

public:
    void start() override;

    void initScreen() override;
    void redraw() override;
    void update() override;
    void stop() override;

    void buttonPressed(espgui::Button button) override;

    std::string text() const override;

private:
    espgui::ProgressBar m_progressBar{10, 210, 200, 10, 0, 500};

    espgui::Label m_label{10, 225};

    int m_progress;

    espchrono::millis_clock::time_point m_nextRestart;

    ModeInterface *m_oldMode;
    IgnoreInputMode m_mode{0, bobbycar::protocol::ControlType::FieldOrientedControl, bobbycar::protocol::ControlMode::Speed};
};
