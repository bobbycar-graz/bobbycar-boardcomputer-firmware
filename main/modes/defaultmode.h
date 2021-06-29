#pragma once

#include <cstdint>

#include <Arduino.h>

#include <espchrono.h>

#include "modeinterface.h"
#include "globals.h"
#include "utils.h"

namespace {
class DefaultMode : public ModeInterface
{
public:
    void update() override;

    const char *displayName() const override { return "Default"; }

    bool waitForGasLoslass{false};
    bool waitForBremsLoslass{false};

private:
    espchrono::millis_clock::time_point lastTime{espchrono::millis_clock::now()};
    float lastPwm{0};
};

namespace modes {
DefaultMode defaultMode;
}

void DefaultMode::update()
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
        float local_gas = *gas;
        float local_brems = *brems;

        if (waitForGasLoslass)
        {
            if (local_gas < 50)
                waitForGasLoslass = false;
            else
                local_gas = 0;
        }

        if (waitForBremsLoslass)
        {
            if (local_brems < 50)
                waitForBremsLoslass = false;
            else
                local_brems = 0;
        }

        const auto gas_processed = settings.defaultMode.squareGas ? (local_gas * local_gas) / 1000.f : local_gas;
        const auto brems_processed = settings.defaultMode.squareBrems ? (local_brems * local_brems) / 1000 : local_brems;

        const auto now = espchrono::millis_clock::now();

        float pwm;
        if (gas_processed >= settings.defaultMode.add_schwelle)
        {
            pwm = (gas_processed/1000.*settings.defaultMode.gas1_wert) + (brems_processed/1000.*settings.defaultMode.brems1_wert);

            if (settings.defaultMode.enableSmoothing && (pwm > 1000. || lastPwm > 1000.))
            {
                if (lastPwm < pwm)
                {
                    pwm = std::min(pwm, lastPwm + (settings.defaultMode.smoothing * std::chrono::milliseconds{now - lastTime}.count() / 100.f));
                    if (pwm < 1000.)
                        pwm = 1000.;
                }
                else if (lastPwm > pwm)
                {
                    pwm = std::max(pwm, lastPwm - (settings.defaultMode.smoothing * std::chrono::milliseconds{now - lastTime}.count() / 100.f));
                }
            }
        }
        else
            pwm = (gas_processed/1000.*settings.defaultMode.gas2_wert) - (brems_processed/1000.*settings.defaultMode.brems2_wert);

        lastPwm = pwm;
        lastTime = now;

        const auto pair = split(settings.defaultMode.modelMode);
        for (bobbycar::protocol::serial::MotorState &motor : motorsInController(controllers.front))
        {
            motor.ctrlTyp = pair.first;
            motor.ctrlMod = pair.second;
            motor.pwm = pwm / 100. * settings.defaultMode.frontPercentage;
        }
        for (bobbycar::protocol::serial::MotorState &motor : motorsInController(controllers.back))
        {
            motor.ctrlTyp = pair.first;
            motor.ctrlMod = pair.second;
            motor.pwm = pwm / 100. * settings.defaultMode.backPercentage;
        }
    }

    fixCommonParams();

    sendCommands();
}
}
