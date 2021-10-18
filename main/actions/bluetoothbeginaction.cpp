#include "bluetoothbeginaction.h"

// local includes
#include "globals.h"

#ifdef FEATURE_BLUETOOTH
void BluetoothBeginAction::triggered()
{
    if (!bluetoothSerial.begin(deviceName))
    {
        //Serial.println("Could not begin bluetooth");
        // TODO: better error handling
    }
}
#endif
