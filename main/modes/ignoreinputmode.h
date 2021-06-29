#pragma once

#include "modeinterface.h"
#include "globals.h"
#include "utils.h"

#include "bobbycar-common.h"

namespace {
class IgnoreInputMode : public ModeInterface
{
public:
    IgnoreInputMode(int16_t pwm, bobbycar::protocol::ControlType ctrlTyp, bobbycar::protocol::ControlMode ctrlMod) :
        m_pwm{pwm}, m_ctrlTyp{ctrlTyp}, m_ctrlMod{ctrlMod}
    {
    }

    void update() override;

    const char *displayName() const override { return "IgnoreInput"; }

private:
    const int16_t m_pwm;
    const bobbycar::protocol::ControlType m_ctrlTyp;
    const bobbycar::protocol::ControlMode m_ctrlMod;
};

void IgnoreInputMode::update()
{
    for (bobbycar::protocol::serial::MotorState &motor : motors())
    {
        motor.ctrlTyp = m_ctrlTyp;
        motor.ctrlMod = m_ctrlMod;
        motor.pwm = m_pwm;
    }

    fixCommonParams();

    sendCommands();
}
}
