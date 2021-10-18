#include "bluetoothendaction.h"

// local includes
#include "globals.h"

#ifdef FEATURE_BLUETOOTH
void BluetoothEndAction::triggered()
{
    bluetoothSerial.end();
}
#endif
