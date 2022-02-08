#pragma once
#ifdef FEATURE_CAN

// system includes
#include <optional>
#include <cstdint>

// 3rdparty lib includes
#include <espchrono.h>

#ifdef CAN_PLUGIN
#include CAN_PLUGIN
#endif

namespace can {
extern uint32_t can_total_error_cnt;
extern std::optional<int16_t> can_gas, can_brems;
extern espchrono::millis_clock::time_point last_can_gas, last_can_brems;

void initCan();
void updateCan();

void sendCanCommands();
} // namespace can
#endif
