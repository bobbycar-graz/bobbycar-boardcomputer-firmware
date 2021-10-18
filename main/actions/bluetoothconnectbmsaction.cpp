#include "bluetoothconnectbmsaction.h"

// local includes
#include "globals.h"

#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
void BluetoothConnectBmsAction::triggered()
{
    uint8_t remoteAddress[ESP_BD_ADDR_LEN] = {0xAA, 0xBB, 0xCC, 0xA1, 0x23, 0x45};
    if (!bluetoothSerial.connect(remoteAddress))
    {
        //Serial.println("Could not connect bluetooth to bms");
        // TODO: better error handling
    }
}
#endif
