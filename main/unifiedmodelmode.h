#pragma once

// system includes
#include <utility>

// local includes
#include "bobbycar-common.h"

enum class UnifiedModelMode : uint8_t
{
    Commutation,
    Sinusoidal,
    FocVoltage,
    FocSpeed,
    FocTorque
};

std::pair<bobbycar::protocol::ControlType, bobbycar::protocol::ControlMode> split(UnifiedModelMode mode);
