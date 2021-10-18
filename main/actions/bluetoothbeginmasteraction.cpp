#include "bluetoothbeginmasteraction.h"

// local includes
#include "globals.h"

#ifdef FEATURE_BLUETOOTH
void BluetoothBeginMasterAction::triggered()
{
    if (!bluetoothSerial.begin(deviceName, true))
    {
        //Serial.println("Could not begin bluetooth master");
        // TODO: better error handling
    }
}
#endif
