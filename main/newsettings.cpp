#include "newsettings.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <configmanager_priv.h>
#include <fmt/core.h>
#include <espwifistack.h>

namespace {
//constexpr const char * const TAG = "CONFIG";
} // namespace

std::string defaultHostname()
{
    if (const auto result = wifi_stack::get_default_mac_addr())
        return fmt::format("bobby_{:02x}{:02x}{:02x}", result->at(3), result->at(4), result->at(5));
    else
        ESP_LOGE(TAG, "get_default_mac_addr() failed: %.*s", result.error().size(), result.error().data());
    return "bobby";
}

ConfigManager<ConfigContainer> configs;

INSTANTIATE_CONFIGMANAGER_TEMPLATES(ConfigContainer)
