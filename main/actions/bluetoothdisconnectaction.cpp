#include "bluetoothdisconnectaction.h"

// local includes
#include "globals.h"

#ifdef FEATURE_BLUETOOTH
void BluetoothDisconnectAction::triggered()
{
    if (!bluetoothSerial.disconnect())
    {
        //Serial.println("Could not disconnect bluetooth");
        // TODO: better error handling
    }
}
#endif
