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

        if (gas_processed > 10 || (!overrideHandbremse && (abs(avgSpeedKmh) > 2 || !controllers.front.feedbackValid || !controllers.back.feedbackValid || settings.handbremse.triggerTimeout < 1 || !settings.handbremse.enable || !settings.handbremse.automatic)))
        {
            m_stillSince = std::nullopt;
            overrideHandbremse = false;
            goto hell;
        }
        else if (!overrideHandbremse && !m_stillSince)
        {
            m_stillSince = now;
            goto hell;
        }
        else if ((overrideHandbremse && abs(avgSpeedKmh) <= 2) && espchrono::ago(*m_stillSince) >= espchrono::milliseconds32(settings.handbremse.triggerTimeout))
        {
            fixCommonParams();

            for (bobbycar::protocol::serial::MotorState &motor : motors())
            {
                motor.ctrlTyp = bobbycar::protocol::ControlType::FieldOrientedControl;
                motor.ctrlMod = ((settings.handbremse.mode == HandbremseMode::SPEED_0) ? bobbycar::protocol::ControlMode::Speed : bobbycar::protocol::ControlMode::OpenMode);
                motor.pwm = 0;
                motor.cruiseCtrlEna = false;
                motor.nCruiseMotTgt = 0;
                if (settings.handbremse.mode == HandbremseMode::MOSFETS_OFF)
                    motor.enable = false;
            }
        }
        else
        {
hell:
            float pwm;
            if (gas_processed >= settings.defaultMode.add_schwelle)
            {
                pwm = (gas_processed/1000.*settings.defaultMode.gas1_wert) + (brems_processed/1000.*settings.defaultMode.brems1_wert);

                if ((settings.defaultMode.enableSmoothingUp || settings.defaultMode.enableSmoothingDown) && (pwm > 1000. || m_lastPwm > 1000.))
                {
                    if (m_lastPwm < pwm && settings.defaultMode.enableSmoothingUp)
                    {
                        pwm = std::min(pwm, m_lastPwm + (settings.defaultMode.smoothing * std::chrono::milliseconds{now - m_lastTime}.count() / 100.f));
                        if (pwm < 1000.)
                            pwm = 1000.;
                    }
                    else if (m_lastPwm > pwm && settings.defaultMode.enableSmoothingDown)
                    {
                        pwm = std::max(pwm, m_lastPwm - (settings.defaultMode.smoothing * std::chrono::milliseconds{now - m_lastTime}.count() / 100.f));
                    }
                }
            }
            else
            {
                pwm = (gas_processed/1000.*settings.defaultMode.gas2_wert) - (brems_processed/1000.*settings.defaultMode.brems2_wert);
                if (
                        (settings.defaultMode.enableFieldWeakSmoothingUp || settings.defaultMode.enableFieldWeakSmoothingDown) &&
                        (m_lastPwm > settings.defaultMode.fwSmoothLowerLimit) &&
                        brems_processed > 0)
                {
                    if (m_lastPwm < pwm && settings.defaultMode.enableFieldWeakSmoothingUp)
                    {
                        auto effective_smoothing = settings.defaultMode.smoothing;
                        auto difference_to_target = std::abs(pwm-m_lastPwm);
                        effective_smoothing *= std::max((difference_to_target / 500),0.5f);

                        pwm = std::min(pwm, m_lastPwm + (effective_smoothing * std::chrono::milliseconds{now - m_lastTime}.count() / 100.f));
                    }
                    else if (m_lastPwm > pwm && settings.defaultMode.enableFieldWeakSmoothingDown)
                    {
                        auto effective_smoothing = settings.defaultMode.smoothing;
                        auto difference_to_target = std::abs(pwm-m_lastPwm);
                        effective_smoothing *= std::max((difference_to_target / 500),0.5f);

                        pwm = std::max(pwm, m_lastPwm - (effective_smoothing * std::chrono::milliseconds{now - m_lastTime}.count() / 100.f));
                    }
                }
            }

            m_lastPwm = pwm;
            m_lastTime = now;

            auto pair = split(settings.defaultMode.modelMode);

            if (settings.hybrid.enable)
            {
                auto activationLimit = settings.hybrid.activationLimit;
                auto deactivationLimit = settings.hybrid.deactivationLimit;
                auto diff = std::abs(activationLimit - deactivationLimit);

                if (diff < 20)
                {
                    int half = (diff / 2) + 0.5;
                    deactivationLimit -= half;
                    activationLimit += half;
                }

                if (!hybridModeActivated && (pwm > activationLimit))
                {
                    hybridModeActivated = true;
                }
                else if (hybridModeActivated && (pwm < deactivationLimit))
                {
                    hybridModeActivated = false;
                }

                if (hybridModeActivated)
                {
                    pair = split(settings.hybrid.hybridMode);
                }
            }

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
    }

    sendCommands();
}
