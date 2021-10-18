#include "bluetoothflushaction.h"

// local includes
#include "globals.h"

#ifdef FEATURE_BLUETOOTH
void BluetoothFlushAction::triggered()
{
    bluetoothSerial.flush();
}
#endif
