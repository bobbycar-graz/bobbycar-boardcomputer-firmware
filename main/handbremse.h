#pragma once

// system
#include <cstdint>

// 3rdparty lib includes
#include <cpptypesafeenum.h>
#include <espchrono.h>

#define HandbremseModeValues(x) \
    x(MOSFETS_OFF)              \
        x(OPENMODE)             \
            x(SPEED_0)
DECLARE_TYPESAFE_ENUM(HandbremseMode,
                      : uint8_t, HandbremseModeValues)

namespace handbremse
{

extern bool angezogen;
extern bool finishedMotorUpdate;
extern std::optional<espchrono::millis_clock::time_point> releaseTimer;
extern std::optional<espchrono::millis_clock::time_point> wishTimer;
extern std::optional<espchrono::millis_clock::time_point> standStillFirstDetected;
extern std::optional<espchrono::millis_clock::time_point> lastAutoRelease;

enum StateWish
{
    none,
    release,
    brake
};

extern StateWish stateWish;
} // namespace handbremse
