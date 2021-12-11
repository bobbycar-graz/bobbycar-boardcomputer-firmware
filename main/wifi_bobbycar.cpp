#include "wifi_bobbycar.h"

// system includes
#include <optional>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <espwifistack.h>

// local includes
#include "globals.h"

namespace
{
wifi_stack::config wifi_create_config();
std::optional<wifi_stack::sta_config> wifi_create_sta_config();
std::optional<wifi_stack::ap_config> wifi_create_ap_config();
} // namespace

void wifi_begin()
{
    wifi_stack::init(wifi_create_config());
}

void wifi_update()
{
    wifi_stack::update(wifi_create_config());
}

esp_err_t wifi_scan()
{
    const auto &sta_config = wifi_create_sta_config();
    if (!sta_config)
    {
        ESP_LOGE("BOBBY", "no sta enabled");
        return ESP_FAIL;
    }

    if (const auto result = wifi_stack::begin_scan(*sta_config); !result)
    {
        ESP_LOGE("BOBBY", "begin_scan() failed with %.*s", result.error().size(), result.error().data());
        return ESP_FAIL;
    }

    return ESP_OK;
}

namespace
{
wifi_stack::config wifi_create_config()
{
    return wifi_stack::config {
        .sta     = wifi_create_sta_config(),
        .ap      = wifi_create_ap_config(),
        .country = wifi_country_t {
            .cc           = { 'A', 'T', '\0' },
            .schan        = 1,
            .nchan        = 13,
            .max_tx_power = 30,
            .policy       = WIFI_COUNTRY_POLICY_MANUAL }
    };
}

std::optional<wifi_stack::sta_config> wifi_create_sta_config()
{
    if (!settings.wifiSettings.wifiStaEnabled)
        return std::nullopt;

    return wifi_stack::sta_config {
        .hostname = deviceName,
        .wifis    = std::array<wifi_stack::wifi_entry, 10> {
            wifi_stack::wifi_entry { .ssid = stringSettings.wifis[0].ssid, .key = stringSettings.wifis[0].key },
            wifi_stack::wifi_entry { .ssid = stringSettings.wifis[1].ssid, .key = stringSettings.wifis[1].key },
            wifi_stack::wifi_entry { .ssid = stringSettings.wifis[2].ssid, .key = stringSettings.wifis[2].key },
            wifi_stack::wifi_entry { .ssid = stringSettings.wifis[3].ssid, .key = stringSettings.wifis[3].key },
            wifi_stack::wifi_entry { .ssid = stringSettings.wifis[4].ssid, .key = stringSettings.wifis[4].key },
            wifi_stack::wifi_entry { .ssid = stringSettings.wifis[5].ssid, .key = stringSettings.wifis[5].key },
            wifi_stack::wifi_entry { .ssid = stringSettings.wifis[6].ssid, .key = stringSettings.wifis[6].key },
            wifi_stack::wifi_entry { .ssid = stringSettings.wifis[7].ssid, .key = stringSettings.wifis[7].key },
            wifi_stack::wifi_entry { .ssid = stringSettings.wifis[8].ssid, .key = stringSettings.wifis[8].key },
            wifi_stack::wifi_entry { .ssid = stringSettings.wifis[9].ssid, .key = stringSettings.wifis[9].key } },
        .min_rssi = -90
    };
}

std::optional<wifi_stack::ap_config> wifi_create_ap_config()
{
    if (!settings.wifiSettings.wifiApEnabled)
        return std::nullopt;

    return wifi_stack::ap_config {
        .hostname  = deviceName,
        .ssid      = deviceName,
        .key       = stringSettings.ap_password,
        .static_ip = {
            .ip      = { 10, 0, 0, 1 },
            .subnet  = { 255, 255, 255, 0 },
            .gateway = { 10, 0, 0, 1 },
        },
        .channel         = 1,
        .authmode        = WIFI_AUTH_WPA2_PSK,
        .ssid_hidden     = false,
        .max_connection  = 4,
        .beacon_interval = 100
    };
}
} // namespace
