#include "handbremse.h"

#include "globals.h"

namespace handbremse
{
bool angezogen { false };
bool finishedMotorUpdate { false };
std::optional<espchrono::millis_clock::time_point> releaseTimer;
std::optional<espchrono::millis_clock::time_point> wishTimer;
std::optional<espchrono::millis_clock::time_point> standStillFirstDetected;
std::optional<espchrono::millis_clock::time_point> lastAutoRelease { espchrono::millis_clock::now() };

StateWish stateWish { none };

} // namespace handbremse
