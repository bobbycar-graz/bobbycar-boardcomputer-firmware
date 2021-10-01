#pragma once

// system includes
#include <array>
#include <string>

namespace {
struct StringSettings
{
    struct ConfiguredWifi {
        std::string ssid;
        std::string key;
    };

    std::array<ConfiguredWifi, 10> wifis;

#ifdef FEATURE_CLOUD
    std::string cloudUrl;
#endif

#ifdef FEATURE_OTA
    std::string otaUrl;

    struct OtaServer {
        std::string serverName;
        std::string serverUrl;
    };

    std::array<OtaServer, 10> otaservers;
#endif

#ifdef FEATURE_GARAGE
    std::string garageUrl;
#endif

#ifdef FEATURE_NTP
    std::string timeServer;
#endif


    template<typename T>
    void executeForEveryCommonSetting(T &&callable);

    template<typename T>
    void executeForEveryProfileSetting(T &&callable);
};

template<typename T>
void StringSettings::executeForEveryCommonSetting(T &&callable)
{
    callable("ssid0", wifis[0].ssid);
    callable("key0", wifis[0].key);
    callable("ssid1", wifis[1].ssid);
    callable("key1", wifis[1].key);
    callable("ssid2", wifis[2].ssid);
    callable("key2", wifis[2].key);
    callable("ssid3", wifis[3].ssid);
    callable("key3", wifis[3].key);
    callable("ssid4", wifis[4].ssid);
    callable("key4", wifis[4].key);
    callable("ssid5", wifis[5].ssid);
    callable("key5", wifis[5].key);
    callable("ssid6", wifis[6].ssid);
    callable("key6", wifis[6].key);
    callable("ssid7", wifis[7].ssid);
    callable("key7", wifis[7].key);
    callable("ssid8", wifis[8].ssid);
    callable("key8", wifis[8].key);
    callable("ssid9", wifis[9].ssid);
    callable("key9", wifis[9].key);

#ifdef FEATURE_CLOUD
    callable("cloudUrl", cloudUrl);
#endif

#ifdef FEATURE_OTA
    callable("otaUrl", otaUrl);

    callable("otaser0name", otaservers[0].serverName);
    callable("otaser0url", otaservers[0].serverUrl);
    callable("otaser1name", otaservers[1].serverName);
    callable("otaser1url", otaservers[1].serverUrl);
    callable("otaser2name", otaservers[2].serverName);
    callable("otaser2url", otaservers[2].serverUrl);
    callable("otaser3name", otaservers[3].serverName);
    callable("otaser3url", otaservers[3].serverUrl);
    callable("otaser4name", otaservers[4].serverName);
    callable("otaser4url", otaservers[4].serverUrl);
    callable("otaser5name", otaservers[5].serverName);
    callable("otaser5url", otaservers[5].serverUrl);
    callable("otaser6name", otaservers[6].serverName);
    callable("otaser6url", otaservers[6].serverUrl);
    callable("otaser7name", otaservers[7].serverName);
    callable("otaser7url", otaservers[7].serverUrl);
    callable("otaser8name", otaservers[8].serverName);
    callable("otaser8url", otaservers[8].serverUrl);
    callable("otaser9name", otaservers[9].serverName);
    callable("otaser9url", otaservers[9].serverUrl);
#endif

#ifdef FEATURE_GARAGE
    callable("garageUrl", garageUrl);
#endif
#ifdef FEATURE_NTP
    callable("timeServer", timeServer);
#endif
}

template<typename T>
void StringSettings::executeForEveryProfileSetting(T &&callable)
{
}
}
