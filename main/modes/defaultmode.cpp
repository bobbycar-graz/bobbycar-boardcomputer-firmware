#include "defaultmode.h"

namespace modes {
DefaultMode defaultMode;
} // namespace modes

void DefaultMode::start()
{
    Base::start();
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
            motor.cruiseCtrlEna = false;
            motor.nCruiseMotTgt = 0;
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

            if ((settings.defaultMode.enableSmoothingUp || settings.defaultMode.enableSmoothingDown) && (pwm > 1000. || lastPwm > 1000.))
            {
                if (lastPwm < pwm && settings.defaultMode.enableSmoothingUp)
                {
                    pwm = std::min(pwm, lastPwm + (settings.defaultMode.smoothing * std::chrono::milliseconds{now - lastTime}.count() / 100.f));
                    if (pwm < 1000.)
                        pwm = 1000.;
                }
                else if (lastPwm > pwm && settings.defaultMode.enableSmoothingDown)
                {
                    pwm = std::max(pwm, lastPwm - (settings.defaultMode.smoothing * std::chrono::milliseconds{now - lastTime}.count() / 100.f));
                }
            }
        }
        else
        {
            pwm = (gas_processed/1000.*settings.defaultMode.gas2_wert) - (brems_processed/1000.*settings.defaultMode.brems2_wert);
            if (
                    (settings.defaultMode.enableFieldWeakSmoothingUp || settings.defaultMode.enableFieldWeakSmoothingDown) &&
                    (lastPwm > settings.defaultMode.fwSmoothLowerLimit) &&
                    brems_processed > 0)
            {
                if (lastPwm < pwm && settings.defaultMode.enableFieldWeakSmoothingUp)
                {
                    auto effective_smoothing = settings.defaultMode.smoothing;
                    auto difference_to_target = std::abs(pwm-lastPwm);
                    effective_smoothing *= std::max((difference_to_target / 500),0.5f);

                    pwm = std::min(pwm, lastPwm + (effective_smoothing * std::chrono::milliseconds{now - lastTime}.count() / 100.f));
                }
                else if (lastPwm > pwm && settings.defaultMode.enableFieldWeakSmoothingDown)
                {
                    auto effective_smoothing = settings.defaultMode.smoothing;
                    auto difference_to_target = std::abs(pwm-lastPwm);
                    effective_smoothing *= std::max((difference_to_target / 500),0.5f);

                    pwm = std::max(pwm, lastPwm - (effective_smoothing * std::chrono::milliseconds{now - lastTime}.count() / 100.f));
                }
            }
        }

        lastPwm = pwm;
        lastTime = now;

        const auto pair = split(settings.defaultMode.modelMode);
        for (bobbycar::protocol::serial::MotorState &motor : motorsInController(controllers.front))
        {
            motor.ctrlTyp = pair.first;
            motor.ctrlMod = pair.second;
            motor.pwm = pwm / 100. * settings.defaultMode.frontPercentage;
            motor.cruiseCtrlEna = false;
            motor.nCruiseMotTgt = 0;
        }
        for (bobbycar::protocol::serial::MotorState &motor : motorsInController(controllers.back))
        {
            motor.ctrlTyp = pair.first;
            motor.ctrlMod = pair.second;
            motor.pwm = pwm / 100. * settings.defaultMode.backPercentage;
            motor.cruiseCtrlEna = false;
            motor.nCruiseMotTgt = 0;
        }
    }

    fixCommonParams();

    sendCommands();
}
