#include "defaultmode.h"

using namespace std::chrono_literals;

namespace modes {
DefaultMode defaultMode;
} // namespace modes

void DefaultMode::start()
{
    Base::start();
}

void DefaultMode::update()
{
    auto pair = split(settings.defaultMode.modelMode);
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

        auto gas_processed = settings.defaultMode.squareGas ? (local_gas * local_gas) / 1000.f : local_gas;
        auto brems_processed = settings.defaultMode.squareBrems ? (local_brems * local_brems) / 1000 : local_brems;

        const auto now = espchrono::millis_clock::now();

        float pwm;

        if (settings.handbremse.enable && handbremse::stateWish == handbremse::StateWish::brake)
        {
            using namespace handbremse;

            const auto speed = abs(avgSpeedKmh);
            const bool gas_und_brems_ist_null = (gas_processed < 1 && brems_processed < 1);
            if (speed < 2 && gas_und_brems_ist_null)
            {
                angezogen = true;
                stateWish = StateWish::none;
            }
        }

        if (settings.handbremse.enable && settings.handbremse.automatic && !handbremse::angezogen)
        {
            using namespace handbremse;
            const auto speed = abs(avgSpeedKmh);

            if (speed < 1)
            {
                if (!standStillFirstDetected)
                {
                    standStillFirstDetected = now;
                }
            }
            else
                standStillFirstDetected = std::nullopt;

            if (standStillFirstDetected && lastAutoRelease)
                if (espchrono::ago(*standStillFirstDetected) > 100ms * settings.handbremse.triggerTimeout && espchrono::ago(*lastAutoRelease) > 5s )
                {
                    angezogen = true;
                }
        }

        if (handbremse::wishTimer)
        {
            using namespace handbremse;
            if (espchrono::ago(*wishTimer) > 5s)
            {
                stateWish = StateWish::none;
                wishTimer = std::nullopt;
            }
        }

        if (handbremse::angezogen)
        {
            using namespace handbremse;

            standStillFirstDetected = std::nullopt;

            const bool valid = (controllers.front.feedbackValid && controllers.back.feedbackValid);
            const bool gas_oder_brems = (gas_processed > 10 || brems_processed > 10);
            fixCommonParams();

            if (stateWish == StateWish::release)
            {
                lastAutoRelease = now;
                releaseTimer = now;
                stateWish = StateWish::none;
                finishedMotorUpdate = false;
            }

            if (valid && gas_oder_brems)
            {
                if (!releaseTimer)
                {
                    lastAutoRelease = now;
                    releaseTimer = now;
                    finishedMotorUpdate = false;
                }
            }

            if (releaseTimer)
            {
                for (bobbycar::protocol::serial::MotorState &motor : motors())
                {
                    motor.pwm = 0;
                    motor.ctrlTyp = pair.first;
                    motor.ctrlMod = pair.second;
                }

                if (espchrono::ago(*releaseTimer) > 1s || finishedMotorUpdate)
                {
                    releaseTimer = std::nullopt;
                    angezogen = false;
                }
            }
            else
            {
                for (bobbycar::protocol::serial::MotorState &motor : motors())
                {
                    motor.ctrlTyp = bobbycar::protocol::ControlType::FieldOrientedControl;
                    switch (settings.handbremse.mode)
                    {
                    case HandbremseMode::MOSFETS_OFF:
                        motor.ctrlMod = bobbycar::protocol::ControlMode::Torque;
                        motor.enable = false;
                        break;
                    case HandbremseMode::OPENMODE:
                        motor.ctrlMod = bobbycar::protocol::ControlMode::OpenMode;
                        break;
                    case HandbremseMode::SPEED_0:
                        motor.ctrlMod = bobbycar::protocol::ControlMode::Speed;
                        break;
                    }
                    motor.pwm = 0;
                    motor.cruiseCtrlEna = false;
                    motor.nCruiseMotTgt = 0;
                }
            }
        }
        else
        {
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
            fixCommonParams();
        }
    }
    sendCommands();
}
