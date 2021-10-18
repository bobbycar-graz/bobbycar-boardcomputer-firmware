#include "wifiscanaction.h"

// esp-idf includes
#include <esp_log.h>

// local includes
#include "wifi_bobbycar.h"

void WifiScanAction::triggered()
{
    if (const auto result = wifi_scan(); result != ESP_OK)
    {
        ESP_LOGE("BOBBY", "wifi_scan() failed with %s", esp_err_to_name(result));
        return;
    }
}
