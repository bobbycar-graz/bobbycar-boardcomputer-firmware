#pragma once

#include <cstdint>

#include <Arduino.h>

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
    unsigned long lastTime{millis()};
    float lastPwm{0};
};

namespace modes {
DefaultMode defaultMode;
}

void DefaultMode::update()
{
    if (waitForGasLoslass)
    {
        if (gas < 50)
            waitForGasLoslass = false;
        else
            gas = 0;
    }
    const auto gas_squared = (gas * gas) / 1000;

    if (waitForBremsLoslass)
    {
        if (brems < 50)
            waitForBremsLoslass = false;
        else
            brems = 0;
    }
    const auto brems_squared = (brems * brems) / 1000;

    const auto now = millis();

    float pwm;
    if (gas_squared >= settings.defaultMode.add_schwelle)
    {
        pwm = (gas_squared/1000.*settings.defaultMode.gas1_wert) + (brems_squared/1000.*settings.defaultMode.brems1_wert);

        if (settings.defaultMode.enableSmoothing && (pwm > 1000. || lastPwm > 1000.))
        {
            if (lastPwm < pwm)
            {
                pwm = std::min(pwm, lastPwm+(settings.defaultMode.smoothing*(now-lastTime)/100.f));
                if (pwm < 1000.)
                    pwm = 1000.;
            }
            else if (lastPwm > pwm)
            {
                pwm = std::max(pwm, lastPwm-(settings.defaultMode.smoothing*(now-lastTime)/100.f));
            }
        }
    }
    else
        pwm = (gas_squared/1000.*settings.defaultMode.gas2_wert) - (brems_squared/1000.*settings.defaultMode.brems2_wert);

    lastPwm = pwm;
    lastTime = now;

#ifdef GLUMP_CONTROLLER
    for (Controller &controller : controllers())
        for (MotorState &motor : motorsInController(controller))
        {
            motor.ctrlTyp = settings.defaultMode.ctrlTyp;
            motor.ctrlMod = settings.defaultMode.ctrlMod;
            motor.pwm = pwm / 100. * (&controller == &front ? settings.defaultMode.frontPercentage : settings.defaultMode.backPercentage);
        }
#endif

#ifdef VESC_CONTROLLER
    for (VescController &controller : controllers()) {
        controller.pwm = pwm;
      Serial.println(String{"new pwm "} + pwm);
    }
#endif

#ifdef GLUMP_CONTROLLER
    fixCommonParams();
#endif

    sendCommands();
}
}
