#include "motortestmode.h"

// local includes
#include "globals.h"
#include "utils.h"

namespace modes {
MotortestMode motortestMode;
} // namespace modes

void MotortestMode::start()
{
    m_pwm = 0;
    m_dir = 1;
}

void MotortestMode::update()
{
    const auto m_pwmMax = settings.motortestMode.maxPwm;
    m_pwm += m_dir * settings.motortestMode.multiplikator;
    if (m_pwm > m_pwmMax) {
        m_pwm = m_pwmMax;
        m_dir = -m_dir;
    } else if (m_pwm < -m_pwmMax) {
        m_pwm = -m_pwmMax;
        m_dir = -m_dir;
    }

    for (bobbycar::protocol::serial::MotorState &motor : motors())
    {
        motor.ctrlTyp = bobbycar::protocol::ControlType::Sinusoidal;
        motor.ctrlMod = bobbycar::protocol::ControlMode::Voltage;
        motor.pwm = m_pwm;
        motor.cruiseCtrlEna = false;
        motor.nCruiseMotTgt = 0;
    }

    fixCommonParams();
    sendCommands();
}
