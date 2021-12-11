#pragma once

#include <cstdint>

#ifdef FEATURE_BLUETOOTH
enum class BluetoothMode : uint8_t
{
    Off,
    Master,
    Slave
};
#endif
