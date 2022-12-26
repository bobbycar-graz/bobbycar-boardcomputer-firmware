#pragma once

// local includes
#include "antbmsmanager.h"

namespace bmsutils {
extern ANTBmsManager antBms;

void init();

void update();

void _notifyCB(NimBLERemoteCharacteristic *pRemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify);

std::string bytesToHex(uint8_t *pData, size_t length);
} // namespace bmsutils
