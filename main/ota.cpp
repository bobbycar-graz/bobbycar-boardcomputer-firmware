#include "ota.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <espwifistack.h>

// local includes
#include "globals.h"

cpputils::DelayedConstruction<EspAsyncOta> asyncOta;
bool asyncOtaTaskStarted{};

namespace {
constexpr const char * const TAG = "BOBBYOTA";
} // namespace

void initOta()
{
}

void handleOta()
{
    if (asyncOta)
        asyncOta->update();
}

tl::expected<void, std::string> triggerOta(std::string_view url)
{
    if (!configs.feature.ota.value)
        return tl::make_unexpected("OTA is not enabled!");

    ESP_LOGI(TAG, "%.*s", url.size(), url.data());

    if (!asyncOta)
        asyncOta.construct();

    if (!asyncOtaTaskStarted)
    {
        if (const auto result = asyncOta->startTask(); !result)
        {
            ESP_LOGE(TAG, "starting OTA task failed: %.*s", result.error().size(), result.error().data());
            return tl::make_unexpected(fmt::format("starting OTA task failed: {}", result.error()));
        }

        asyncOtaTaskStarted = true;
    }

    if (const auto result = asyncOta->trigger(url, {}, {}, {}); !result)
        return tl::make_unexpected(std::move(result).error());

    wifi_stack::delete_scan_result();

    return {};
}
