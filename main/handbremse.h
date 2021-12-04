#pragma once

// system
#include <cstdint>

// 3rdparty lib includes
#include <cpptypesafeenum.h>

#define HandbremseModeValues(x) \
    x(MOSFETS_OFF) \
    x(OPENMODE) \
    x(SPEED_0)
DECLARE_TYPESAFE_ENUM(HandbremseMode, : uint8_t, HandbremseModeValues)
