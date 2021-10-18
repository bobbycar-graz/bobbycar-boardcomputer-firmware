#pragma once

// local includes
#include "modeinterface.h"
#include "globals.h"
#include "utils.h"
#include "bobbycar-common.h"

class IgnoreInputMode : public ModeInterface
{
public:
    IgnoreInputMode(int16_t pwm, bobbycar::protocol::ControlType ctrlTyp, bobbycar::protocol::ControlMode ctrlMod);

    void update() override;

    const char *displayName() const override { return "IgnoreInput"; }

private:
    const int16_t m_pwm;
    const bobbycar::protocol::ControlType m_ctrlTyp;
    const bobbycar::protocol::ControlMode m_ctrlMod;
};
