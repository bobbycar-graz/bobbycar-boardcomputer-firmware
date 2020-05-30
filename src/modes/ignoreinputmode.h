#pragma once

#include "modeinterface.h"
#include "globals.h"
#include "utils.h"

#include "bobbycar-protocol/protocol.h"
#include "../controller.h"
#include "../utils.h"

namespace {
class IgnoreInputMode : public ModeInterface
{
public:
#ifdef GLUMP_CONTROLLER
    IgnoreInputMode(int16_t pwm, ControlType ctrlTyp, ControlMode ctrlMod) :
        m_pwm{pwm}, m_ctrlTyp{ctrlTyp}, m_ctrlMod{ctrlMod}
    {
    }
#endif

#ifdef VESC_CONTROLLER
  IgnoreInputMode(int16_t pwm) :
      m_pwm{pwm}
  {
  }
#endif

    void update() override;

    const char *displayName() const override { return "IgnoreInput"; }

private:
    const int16_t m_pwm;
#ifdef GLUMP_CONTROLLER
    const ControlType m_ctrlTyp;
    const ControlMode m_ctrlMod;
#endif
};

void IgnoreInputMode::update()
{
#ifdef GLUMP_CONTROLLER
    for (MotorState &motor : motors())
    {
        motor.ctrlTyp = m_ctrlTyp;
        motor.ctrlMod = m_ctrlMod;
        motor.pwm = m_pwm;
    }

    fixCommonParams();
#endif

#ifdef VESC_CONTROLLER
  for (VescController &controller : controllers()) {
    controller.pwm = m_pwm;
  }
#endif

    sendCommands();
}
}
