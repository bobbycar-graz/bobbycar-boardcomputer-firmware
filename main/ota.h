#pragma once

// 3rdparty lib includes
#ifdef FEATURE_OTA
#include <espasyncota.h>
#endif
#include <delayedconstruction.h>

// local includes

namespace {
#ifdef FEATURE_OTA
cpputils::DelayedConstruction<EspAsyncOta> asyncOta;
bool asyncOtaTaskStarted{};

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
#endif
}
