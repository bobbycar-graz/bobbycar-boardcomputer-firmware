#pragma once

#include "actioninterface.h"
#include "globals.h"

namespace {
#ifdef FEATURE_BLUETOOTH
class BluetoothDisconnectAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        if (!bluetoothSerial.disconnect())
        {
            //Serial.println("Could not disconnect bluetooth");
            // TODO: better error handling
        }
    }
};
#endif
}
