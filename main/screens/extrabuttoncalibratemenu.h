#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"
#include "modeinterface.h"
#include "modes/ignoreinputmode.h"

namespace bobby {

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
    WaitingProfile3,
    WaitingExtra1,
    WaitingExtra2,
    WaitingExtra3,
    WaitingExtra4,
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
    void stop() override;

    void rawButtonPressed(uint8_t button) override;
    void buttonPressed(espgui::Button button) override;

private:
    ModeInterface *m_oldMode;
    IgnoreInputMode m_mode{0, bobbycar::protocol::ControlType::FieldOrientedControl, bobbycar::protocol::ControlMode::Torque};
    bool validateNewButton(uint8_t button);
};

} // namespace bobby
