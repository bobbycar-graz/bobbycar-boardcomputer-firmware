#pragma once

#include "actioninterface.h"
#include "globals.h"

namespace {
#ifdef FEATURE_BLUETOOTH
class BluetoothBeginAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        if (!bluetoothSerial.begin(deviceName))
        {
            //Serial.println("Could not begin bluetooth");
            // TODO: better error handling
        }
    }
};
#endif
}
