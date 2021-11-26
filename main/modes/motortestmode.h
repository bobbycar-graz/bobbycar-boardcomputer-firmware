#pragma once

// local includes
#include "bobbycar-common.h"
#include "modeinterface.h"

class MotortestMode : public ModeInterface
{
public:
    void update() override;
    void start() override;
    const char *displayName() const override { return "Motortest"; }
private:
    int16_t m_pwm{};
    int8_t m_dir{};
};

namespace modes {
extern MotortestMode motortestMode;
} // namespace modes
