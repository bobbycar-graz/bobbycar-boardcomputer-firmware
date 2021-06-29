#pragma once

namespace {
template<class T>
constexpr const T& clamp( const T& v, const T& lo, const T& hi )
{
    assert( !(hi < lo) );
    return (v < lo) ? lo : (hi < v) ? hi : v;
}
}

#include "modeinterface.h"
#include "globals.h"
#include "utils.h"
#include "defaultmode.h"

#include "bobbycar-common.h"

namespace {
#ifdef FEATURE_GAMETRAK
class GametrakMode : public ModeInterface
{
public:
    void start() override;
    void update() override;

    const char *displayName() const override { return "Gametrak"; }

private:
    bool m_flag;
};

namespace modes {
GametrakMode gametrakMode;
}

void GametrakMode::start()
{
    m_flag = false;
}

void GametrakMode::update()
{
    if (!gas || !brems)
    {
        start();

        for (bobbycar::protocol::serial::MotorState &motor : motors())
        {
            motor.ctrlTyp = bobbycar::protocol::ControlType::FieldOrientedControl;
            motor.ctrlMod = bobbycar::protocol::ControlMode::OpenMode;
            motor.pwm = 0;
        }
    }
    else
    {
        if (*gas > 500. || *brems > 500.)
        {
            modes::defaultMode.waitForGasLoslass = true;
            modes::defaultMode.waitForBremsLoslass = true;
            currentMode = &modes::defaultMode;
            return;
        }

        int16_t pwm;
        if (gametrakDist < 150)
        {
            pwm = 0;
            m_flag = false;
        }
        else
        {
            if (m_flag || gametrakDist >= 400)
            {
                m_flag = true;
                pwm = clamp<int>((gametrakDist - 400) / 2, -200, 200);
            }
            else
                pwm = 0;
        }

        for (bobbycar::protocol::serial::MotorState &motor : motors())
        {
            motor.ctrlTyp = bobbycar::protocol::ControlType::FieldOrientedControl;
            motor.ctrlMod = bobbycar::protocol::ControlMode::Speed;
            motor.pwm = pwm;
        }
    }

    fixCommonParams();

    sendCommands();
}
#endif
}
