#pragma once

// local includes
#include "modeinterface.h"
#include "globals.h"
#include "utils.h"
#include "bobbycar-common.h"

class LarsmMode : public ModeInterface
{
    using Base = ModeInterface;

public:
    void start() override;
    void update() override;

    const char *displayName() const override { return "Larsm"; }

private:
    float adc1_filtered;
    float adc2_filtered;
    int speed;
    float weak;
};

namespace modes {
extern LarsmMode larsmMode;
} // namespace modes
