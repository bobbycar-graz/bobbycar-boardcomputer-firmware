#include "presets.h"
#include "macros_bobbycar.h"

namespace presets {
StringSettings makeDefaultStringSettings()
{
    using ConfiguredWifi = StringSettings::ConfiguredWifi;
#ifdef FEATURE_OTA
    using ConfiguredOtaServer = StringSettings::ConfiguredOtaServer;
#endif

    return {
        .wifis = std::array<ConfiguredWifi, 10> {
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} },
            ConfiguredWifi { .ssid = {}, .key = {} }
        },
#ifdef FEATURE_CLOUD
        .cloudUrl = {},
#endif
#ifdef FEATURE_UDPCLOUD
        .udpCloudUrl = {},
#endif
#ifdef FEATURE_OTA
        .otaUrl = {},
#endif
#ifdef FEATURE_GARAGE
        .garageUrl = {},
#endif
#ifdef FEATURE_NTP
        .timeServer = "europe.pool.ntp.org",
#endif
#ifdef FEATURE_OTA
        .otaServers = std::array<ConfiguredOtaServer, 5> {
            ConfiguredOtaServer { .name = {}, .url = {} },
            ConfiguredOtaServer { .name = {}, .url = {} },
            ConfiguredOtaServer { .name = {}, .url = {} },
            ConfiguredOtaServer { .name = {}, .url = {} },
            ConfiguredOtaServer { .name = {}, .url = {} },
//            ConfiguredOtaServer { .name = {}, .url = {} },
//            ConfiguredOtaServer { .name = {}, .url = {} },
//            ConfiguredOtaServer { .name = {}, .url = {} },
//            ConfiguredOtaServer { .name = {}, .url = {} },
//            ConfiguredOtaServer { .name = {}, .url = {} },
        },
        .otaServerUrl = {},
#endif
#ifdef AP_PASSWORD
        .ap_password = STRING(AP_PASSWORD),
#else
        .ap_password = "Bobbycar_123",
#endif
#ifdef FEATURE_OTA
        .otaServerBranch = {},
#endif
        .webserver_password = {},
    };
}
} // namespace presets
