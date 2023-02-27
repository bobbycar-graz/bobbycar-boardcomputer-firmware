#pragma once

// 3rdparty lib includes
#include <espchrono.h>
#include <tftinterface.h>
#include <widgets/label.h>
#include <widgets/progressbar.h>

// local includes
#include "guihelpers/bobbydisplaywithtitle.h"
#include "modes/ignoreinputmode.h"

namespace bobby {

class ConfiscationDisplay : public BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;

public:
    void start() override;

    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;
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

} // namespace bobby
