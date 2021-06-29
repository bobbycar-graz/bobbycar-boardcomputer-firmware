#pragma once

#include "actioninterface.h"
#include "globals.h"

namespace {
#ifdef FEATURE_BLUETOOTH
class BluetoothEndAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        bluetoothSerial.end();
    }
};
#endif
}
