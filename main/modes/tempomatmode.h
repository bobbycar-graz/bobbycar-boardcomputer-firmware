#pragma once

#include <cstdint>

#include "bobbycar-common.h"

#include "modeinterface.h"
#include "globals.h"
#include "utils.h"
#include "defaultmode.h"

namespace {
class TempomatMode : public ModeInterface
{
public:
    void start() override;
    void update() override;

    const char *displayName() const override { return "Tempomat"; }

    int16_t pwm;
};

namespace modes {
TempomatMode tempomatMode;
}

void TempomatMode::start()
{
    pwm = 0;
}

void TempomatMode::update()
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
        if (*gas > 500. && *brems > 500.)
        {
            pwm = 0;
            modes::defaultMode.waitForGasLoslass = true;
            modes::defaultMode.waitForBremsLoslass = true;
            currentMode = &modes::defaultMode;
            return;
        }

        pwm += (*gas/1000.) - (*brems/1000.);

        for (bobbycar::protocol::serial::MotorState &motor : motors())
        {
            const auto pair = split(settings.tempomatMode.modelMode);
            motor.ctrlTyp = pair.first;
            motor.ctrlMod = pair.second;
            motor.pwm = pwm;
        }
    }

    fixCommonParams();

    sendCommands();
}
}
