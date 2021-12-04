#pragma once

// system includes
#include <cstdint>
#include <optional>

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
    bool hybridModeActivated{false};
    bool overrideHandbremse{false};

private:
    espchrono::millis_clock::time_point m_lastTime{espchrono::millis_clock::now()};
    float m_lastPwm{0};
    std::optional<espchrono::millis_clock::time_point> m_stillSince;
};

namespace modes {
extern DefaultMode defaultMode;
} // namespace modes
