#include "bmsutils.h"

// system includes
#include <iomanip>

// 3rdparty lib includes
#include <NimBLEDevice.h>

// local includes
#include "newsettings.h"

namespace bmsutils {

ANTBmsManager antBms;

void init() {}

void update()
{
    const auto initialized = antBms.isInitialized();

    if (configs.bmsEnabled.value() && !initialized && NimBLEDevice::getInitialized())
    {
        ESP_LOGI("bmsutils", "initializing bms");
        antBms.init();
    }
    else if ((!configs.bmsEnabled.value() || !configs.bleSettings.bleEnabled.value()) && initialized)
    {
        ESP_LOGI("bmsutils", "deinitializing bms");
        antBms.deinit();
    }

    if (initialized && NimBLEDevice::getInitialized())
    {
        antBms.update();
    }
}

void _notifyCB(NimBLERemoteCharacteristic *pRemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
{
    ESP_LOGI("bmsutils", "notifyCB: %s", bmsutils::bytesToHex(pData, length).c_str());
    antBms.notifyCB(pRemoteCharacteristic, pData, length, isNotify);
}

std::string bytesToHex(uint8_t *pData, size_t length)
{
    std::stringstream ss;

    for (size_t i = 0; i < length; i++)
    {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int)pData[i];
    }

    return ss.str();
}
} // namespace bmsutils
