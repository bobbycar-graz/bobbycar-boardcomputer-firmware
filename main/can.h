#pragma once

// system includes
#include <cstring>
#include <optional>

// esp-idf includes
#include <driver/gpio.h>
#include <driver/twai.h>
#include <esp_log.h>

// Arduino includes
#include <Arduino.h>

// 3rdparty lib includes
#include <espchrono.h>
#include <tickchrono.h>

// local includes
#include "bobbycar-can.h"
#include "globals.h"
#include "buttons.h"

#ifdef CAN_PLUGIN
#pragma message "Activating Can Plugin"
#include CAN_PLUGIN
#endif

namespace can {
extern std::optional<int16_t> can_gas, can_brems;
extern espchrono::millis_clock::time_point last_can_gas, last_can_brems;

struct CanButtonsState
{
    bool up{};
    bool down{};
    bool confirm{};
    bool back{};
    bool profile0{};
    bool profile1{};
    bool profile2{};
    bool profile3{};
};
extern CanButtonsState lastButtonsState;

void initCan();
bool tryParseCanInput();
void parseCanInput();
void sendCanCommands();
} // namespace can
