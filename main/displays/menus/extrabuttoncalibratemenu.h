#pragma once

// local includes
#include "displays/bobbymenudisplay.h"
#include "modeinterface.h"
#include "modes/ignoreinputmode.h"

namespace {
enum Status : uint8_t
{
    Idle,
    WaitingLeft2,
    WaitingRight2,
    WaitingUp2,
    WaitingDown2,
    WaitingProfile0,
    WaitingProfile1,
    WaitingProfile2,
    WaitingProfile3
};
} // namespace

class ExtraButtonCalibrateMenu :
        public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;
public:
    ExtraButtonCalibrateMenu();
    std::string text() const override;
    void back() override;

    void start() override;
    void update() override;
    void stop() override;

    void rawButtonPressed(uint8_t button) override;
    void rawButtonReleased(uint8_t button) override;
    void buttonPressed(espgui::Button button) override;
    void buttonReleased(espgui::Button button) override;

private:
    ModeInterface *m_oldMode;
    IgnoreInputMode m_mode{0, bobbycar::protocol::ControlType::FieldOrientedControl, bobbycar::protocol::ControlMode::Torque};
    bool validateNewButton(uint8_t button);
};
