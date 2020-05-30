#pragma once

#include <cstdint>

#include "bobbycar-protocol/protocol.h"

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
    if (gas > 500. && brems > 500.)
    {
        pwm = 0;
        modes::defaultMode.waitForGasLoslass = true;
        modes::defaultMode.waitForBremsLoslass = true;
        currentMode = &modes::defaultMode;
        return;
    }

    pwm += (gas/1000.) - (brems/1000.);
#ifdef GLUMP_CONTROLLER
    for (MotorState &motor : motors())
    {
        motor.ctrlTyp = settings.tempomatMode.ctrlTyp;
        motor.ctrlMod = settings.tempomatMode.ctrlMod;
        motor.pwm = pwm;
    }

    fixCommonParams();
#endif

#ifdef VESC_CONTROLLER
    for (VescController &controller : controllers()) {
      controller.pwm = pwm;
    }
#endif

    sendCommands();
}
}
