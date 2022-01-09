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

class WheelchairMode : public ModeInterface
{
    using Base = ModeInterface;

public:
    void start() override;
    void update() override;

    const char *displayName() const override { return "Wheelchair"; }

    bool waitForGasLoslass{false};
    bool waitForBremsLoslass{false};

    void copyFromSettings();

private:
    espchrono::millis_clock::time_point m_lastTime{espchrono::millis_clock::now()};
    float m_x{0};
    float m_lastPwm{0};

    int16_t m_gasMin, m_gasMax, m_bremsMin, m_bremsMax, m_gasMitte, m_bremsMitte;
};

namespace modes {
extern WheelchairMode wheelchairMode;
} // namespace modes
