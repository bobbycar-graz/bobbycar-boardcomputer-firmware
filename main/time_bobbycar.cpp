#include "time_bobbycar.h"

// esp-idf includes
#include <esp_log.h>
#include <lwip/apps/snmp.h>
#include <esp_sntp.h>

// 3rdparty lib includes
#include <espstrutils.h>

// local includes
#include "newsettings.h"

espchrono::time_zone get_default_timezone() noexcept
{
    using namespace espchrono;
    return time_zone{configs.timezoneOffset.value, configs.timeDst.value};
}

#ifdef CONFIG_ESPCHRONO_SUPPORT_DEFAULT_TIMEZONE
espchrono::time_zone espchrono::get_default_timezone() noexcept
{
    return ::get_default_timezone();
}
#endif

namespace {
constexpr const char * const TAG = "BOBBYTIME";
} // namespace

void time_sync_notification_cb(struct timeval *tv);

void initTime()
{
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    static_assert(SNTP_MAX_SERVERS >= 1);
    sntp_setservername(0, configs.timeServer.value.c_str());
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_set_sync_mode(configs.timeSyncMode.value);
    sntp_set_sync_interval(espchrono::milliseconds32{configs.timeSyncInterval.value}.count());
    if (configs.timeServerEnabled.value)
    {
        ESP_LOGI("BOBBY", "sntp_init() ...");
        sntp_init();
        if (!sntp_enabled())
        {
            ESP_LOGE("BOBBY", "sntp_init() failed");
        }
    }
}

void updateTime()
{
    if (!configs.feature.ntp.value)
        return;

    if (bool(sntp_enabled()) != configs.timeServerEnabled.value)
    {
        if (configs.timeServerEnabled.value)
        {
            ESP_LOGD("BOBBY", "calling sntp_init()...");
            sntp_init();
            ESP_LOGI("BOBBY", "sntp_init() finished");
        }
        else
        {
            ESP_LOGD("BOBBY", "calling sntp_stop()...");
            sntp_stop();
            ESP_LOGI("BOBBY", "sntp_stop() finished");
        }
    }

    if (configs.timeServer.value != sntp_getservername(0))
    {
        ESP_LOGD("BOBBY", "calling sntp_getservername() with %s...", configs.timeServer.value.c_str());
        sntp_setservername(0, configs.timeServer.value.c_str());
        ESP_LOGI("BOBBY", "sntp_getservername() finished");
    }

    if (configs.timeSyncMode.value != sntp_get_sync_mode())
    {
        ESP_LOGD("BOBBY", "calling sntp_set_sync_mode() with %s...", espcpputils::toString(configs.timeSyncMode.value).c_str());
        sntp_set_sync_mode(configs.timeSyncMode.value);
        ESP_LOGI("BOBBY", "sntp_set_sync_mode() finished");
    }

    if (configs.timeSyncInterval.value != espchrono::milliseconds32{sntp_get_sync_interval()})
    {
        ESP_LOGD("BOBBY", "calling sntp_set_sync_interval() with %s...", espchrono::toString(configs.timeSyncInterval.value).c_str());
        sntp_set_sync_interval(espchrono::milliseconds32{configs.timeSyncInterval.value}.count());
        ESP_LOGI("BOBBY", "sntp_set_sync_interval() finished");
    }
}

tl::expected<void, std::string> time_requestSync()
{
    ESP_LOGI("BOBBY", "called");
    if (!sntp_restart())
        return tl::make_unexpected("sntp_restart() failed");
    return {};
}

void time_sync_notification_cb(struct timeval *tv)
{
    if (tv)
        ESP_LOGI("BOBBY", "%ld", tv->tv_sec);
    else
        ESP_LOGI("BOBBY", "nullptr");
}

void time_set_now(espchrono::utc_clock::time_point now)
{
    using namespace espchrono;
    // ESP_LOGI("BOBBY", "%s (%lld)%s", toString(toDateTime(now)).c_str(), std::chrono::floor<std::chrono::seconds>(now.time_since_epoch()).count(), time_valid(now) ? "":" (probably invalid)");
    const auto seconds = std::chrono::floor<std::chrono::seconds>(now.time_since_epoch());

    timeval ts {
        .tv_sec = (long int)seconds.count(),
        .tv_usec = (long int)std::chrono::floor<std::chrono::microseconds>(now.time_since_epoch() - seconds).count()
    };
    timezone tz {
        .tz_minuteswest = 0,
        .tz_dsttime = 0
    };
    settimeofday(&ts, &tz);
}
