#pragma once

#include "actioninterface.h"
#include "globals.h"

namespace {
#ifdef FEATURE_BLUETOOTH
class BluetoothBeginMasterAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        if (!bluetoothSerial.begin(deviceName, true))
        {
            //Serial.println("Could not begin bluetooth master");
            // TODO: better error handling
        }
    }
};
#endif
}
