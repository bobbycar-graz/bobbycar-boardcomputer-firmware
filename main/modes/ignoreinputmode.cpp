#include "ignoreinputmode.h"

IgnoreInputMode::IgnoreInputMode(int16_t pwm, bobbycar::protocol::ControlType ctrlTyp, bobbycar::protocol::ControlMode ctrlMod) :
    m_pwm{pwm}, m_ctrlTyp{ctrlTyp}, m_ctrlMod{ctrlMod}
{
}

void IgnoreInputMode::update()
{
    for (bobbycar::protocol::serial::MotorState &motor : motors())
    {
        motor.ctrlTyp = m_ctrlTyp;
        motor.ctrlMod = m_ctrlMod;
        motor.pwm = m_pwm;
        motor.cruiseCtrlEna = false;
        motor.nCruiseMotTgt = 0;
    }

    fixCommonParams();

    sendCommands();
}
