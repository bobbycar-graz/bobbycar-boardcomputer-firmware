#pragma once

// system includes
#include <array>
#include <string>

struct StringSettings
{
#ifdef FEATURE_CLOUD
    std::string cloudUrl;
#endif

#ifdef FEATURE_UDPCLOUD
    std::string udpCloudUrl;
#endif

#ifdef FEATURE_OTA
    std::string otaUrl;
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

#ifdef FEATURE_OTA
    struct ConfiguredOtaServer {
        std::string name;
        std::string url;
    };

    std::array<ConfiguredOtaServer, 5> otaServers;
    std::string otaServerUrl;
#endif
#ifdef FEATURE_DNS_NS
    std::string dns_key;
#endif
#ifdef FEATURE_OTA
    std::string otaServerBranch;
#endif
    std::string webserver_password;
};

template<typename T>
void StringSettings::executeForEveryCommonSetting(T &&callable)
{
#ifdef FEATURE_CLOUD
    callable("cloudUrl", cloudUrl);
#endif

#ifdef FEATURE_UDPCLOUD
    callable("udpUrl", udpCloudUrl);
#endif

#ifdef FEATURE_OTA
    callable("otaUrl", otaUrl);
#endif

#ifdef FEATURE_GARAGE
    callable("garageUrl", garageUrl);
#endif
#ifdef FEATURE_NTP
    callable("timeServer", timeServer);
#endif
#ifdef FEATURE_OTA
    callable("otaName0",    otaServers[0].name);
    callable("otaUrl0",     otaServers[0].url);
    callable("otaName1",    otaServers[1].name);
    callable("otaUrl1",     otaServers[1].url);
    callable("otaName2",    otaServers[2].name);
    callable("otaUrl2",     otaServers[2].url);
    callable("otaName3",    otaServers[3].name);
    callable("otaUrl3",     otaServers[3].url);
    callable("otaName4",    otaServers[4].name);
    callable("otaUrl4",     otaServers[4].url);
//    callable("otaName5",    otaServers[5].name);
//    callable("otaUrl5",     otaServers[5].url);
//    callable("otaName6",    otaServers[6].name);
//    callable("otaUrl6",     otaServers[6].url);
//    callable("otaName7",    otaServers[7].name);
//    callable("otaUrl7",     otaServers[7].url);
//    callable("otaName8",    otaServers[8].name);
//    callable("otaUrl8",     otaServers[8].url);
//    callable("otaName9",    otaServers[9].name);
//    callable("otaUrl9",     otaServers[9].url);

    callable("otaserver", otaServerUrl);
    callable("otaBranch", otaServerBranch);
#endif
#ifdef FEATURE_DNS_NS
    callable("dnskey", dns_key);
#endif
    callable("webpw", webserver_password);
}

template<typename T>
void StringSettings::executeForEveryProfileSetting(T &&callable)
{
}
