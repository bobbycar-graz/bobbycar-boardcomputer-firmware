#pragma once

// system includes
#include <cstdint>

// Arduino includes
#include <Arduino.h>

// 3rdparty lib includes
#include <espchrono.h>

// local includes
#include "modeinterface.h"
#include "globals.h"
#include "utils.h"

class DefaultMode : public ModeInterface
{
    using Base = ModeInterface;

public:
    void start() override;
    void update() override;

    const char *displayName() const override { return "Default"; }

    bool waitForGasLoslass{false};
    bool waitForBremsLoslass{false};

private:
    espchrono::millis_clock::time_point lastTime{espchrono::millis_clock::now()};
    float lastPwm{0};
};

namespace modes {
extern DefaultMode defaultMode;
} // namespace modes
