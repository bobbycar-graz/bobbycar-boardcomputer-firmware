#pragma once

#include <cstdint>

namespace {
#ifdef FEATURE_BLUETOOTH
enum class BluetoothMode : uint8_t
{
    Off,
    Master,
    Slave
};
#endif
}
