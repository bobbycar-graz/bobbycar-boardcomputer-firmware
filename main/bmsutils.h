#pragma once

// local includes
#include "antbms.h"

namespace bmsutils {
extern ANTBms antBms;

void init();

void update();

void _notifyCB(NimBLERemoteCharacteristic *pRemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify);

std::string bytesToHex(uint8_t *pData, size_t length);
} // namespace bmsutils
