#pragma once

#include "sdkconfig.h"

// system includes
#include <array>
#include <optional>
#include <string>

// 3rdparty lib includes
#include <configconstraints_base.h>
#include <configmanager.h>
#include <configwrapper.h>
#include <espwifiutils.h>
#include <fmt/core.h>
#include <makearray.h>

using namespace espconfig;

std::string defaultHostname();

class WiFiConfig
{
public:
  WiFiConfig(const char *ssidNvsKey, const char *keyNvsKey,
             const char *useStaticIpKey, const char *staticIpKey, const char *staticSubnetKey, const char *staticGatewayKey,
             const char *useStaticDnsKey, const char *staticDns0Key, const char *staticDns1Key, const char *staticDns2Key) :
      ssid { std::string {}, DoReset, StringMaxSize<32>, ssidNvsKey },
      key { std::string {}, DoReset, StringOr<StringEmpty, StringMinMaxSize<8, 64>>, keyNvsKey },
      useStaticIp { false, DoReset, {}, useStaticIpKey },
      staticIp { wifi_stack::ip_address_t {}, DoReset, {}, staticIpKey },
      staticSubnet { wifi_stack::ip_address_t {}, DoReset, {}, staticSubnetKey },
      staticGateway { wifi_stack::ip_address_t {}, DoReset, {}, staticGatewayKey },
      useStaticDns { false, DoReset, {}, useStaticDnsKey },
      staticDns0 { wifi_stack::ip_address_t {}, DoReset, {}, staticDns0Key },
      staticDns1 { wifi_stack::ip_address_t {}, DoReset, {}, staticDns1Key },
      staticDns2 { wifi_stack::ip_address_t {}, DoReset, {}, staticDns2Key }
  {
  }

  ConfigWrapper<std::string> ssid;
  ConfigWrapper<std::string> key;
  ConfigWrapper<bool> useStaticIp;
  ConfigWrapper<wifi_stack::ip_address_t> staticIp;
  ConfigWrapper<wifi_stack::ip_address_t> staticSubnet;
  ConfigWrapper<wifi_stack::ip_address_t> staticGateway;
  ConfigWrapper<bool> useStaticDns;
  ConfigWrapper<wifi_stack::ip_address_t> staticDns0;
  ConfigWrapper<wifi_stack::ip_address_t> staticDns1;
  ConfigWrapper<wifi_stack::ip_address_t> staticDns2;
};

class ConfigContainer
{
  using mac_t = wifi_stack::mac_t;

public:
  //                                            default                                 allowReset constraints                 nvsName
  ConfigWrapper<std::optional<mac_t>> baseMacAddressOverride { std::nullopt, NoReset, {}, "baseMacAddrOver" };
  ConfigWrapper<std::string> hostname { defaultHostname, DoReset, StringMinMaxSize<4, 32>, "hostname" };
  ConfigWrapper<bool> wifiStaEnabled { true, DoReset, {}, "wifi_enabled" };
  std::array<WiFiConfig, 10> wifi_configs {
    WiFiConfig { "wifi_ssid0", "wifi_key0", "wifi_usestatic0", "wifi_static_ip0", "wifi_stati_sub0", "wifi_stat_gate0", "wifi_usestadns0", "wifi_stat_dnsA0", "wifi_stat_dnsB0", "wifi_stat_dnsC0" },
    WiFiConfig { "wifi_ssid1", "wifi_key1", "wifi_usestatic1", "wifi_static_ip1", "wifi_stati_sub1", "wifi_stat_gate1", "wifi_usestadns1", "wifi_stat_dnsA1", "wifi_stat_dnsB1", "wifi_stat_dnsC1" },
    WiFiConfig { "wifi_ssid2", "wifi_key2", "wifi_usestatic2", "wifi_static_ip2", "wifi_stati_sub2", "wifi_stat_gate2", "wifi_usestadns2", "wifi_stat_dnsA2", "wifi_stat_dnsB2", "wifi_stat_dnsC2" },
    WiFiConfig { "wifi_ssid3", "wifi_key3", "wifi_usestatic3", "wifi_static_ip3", "wifi_stati_sub3", "wifi_stat_gate3", "wifi_usestadns3", "wifi_stat_dnsA3", "wifi_stat_dnsB3", "wifi_stat_dnsC3" },
    WiFiConfig { "wifi_ssid4", "wifi_key4", "wifi_usestatic4", "wifi_static_ip4", "wifi_stati_sub4", "wifi_stat_gate4", "wifi_usestadns4", "wifi_stat_dnsA4", "wifi_stat_dnsB4", "wifi_stat_dnsC4" },
    WiFiConfig { "wifi_ssid5", "wifi_key5", "wifi_usestatic5", "wifi_static_ip5", "wifi_stati_sub5", "wifi_stat_gate5", "wifi_usestadns5", "wifi_stat_dnsA5", "wifi_stat_dnsB5", "wifi_stat_dnsC5" },
    WiFiConfig { "wifi_ssid6", "wifi_key6", "wifi_usestatic6", "wifi_static_ip6", "wifi_stati_sub6", "wifi_stat_gate6", "wifi_usestadns6", "wifi_stat_dnsA6", "wifi_stat_dnsB6", "wifi_stat_dnsC6" },
    WiFiConfig { "wifi_ssid7", "wifi_key7", "wifi_usestatic7", "wifi_static_ip7", "wifi_stati_sub7", "wifi_stat_gate7", "wifi_usestadns7", "wifi_stat_dnsA7", "wifi_stat_dnsB7", "wifi_stat_dnsC7" },
    WiFiConfig { "wifi_ssid8", "wifi_key8", "wifi_usestatic8", "wifi_static_ip8", "wifi_stati_sub8", "wifi_stat_gate8", "wifi_usestadns8", "wifi_stat_dnsA8", "wifi_stat_dnsB8", "wifi_stat_dnsC8" },
    WiFiConfig { "wifi_ssid9", "wifi_key9", "wifi_usestatic9", "wifi_static_ip9", "wifi_stati_sub9", "wifi_stat_gate9", "wifi_usestadns9", "wifi_stat_dnsA9", "wifi_stat_dnsB9", "wifi_stat_dnsC9" }
  };
  ConfigWrapper<int8_t> wifiStaMinRssi { -90, DoReset, {}, "wifiStaMinRssi" };

  ConfigWrapper<bool> wifiApEnabled { true, DoReset, {}, "wifiApEnabled" };
  ConfigWrapper<std::string> factoryWifiApName { defaultHostname, NoReset, StringMinMaxSize<4, 32>, "facWifiApName" };
  ConfigWrapper<std::string> wifiApName { factoryWifiApName, StringMinMaxSize<4, 32>, "wifiApName" };
  ConfigWrapper<std::string> factoryWifiApKey { "Passwort_123", NoReset, StringOr<StringEmpty, StringMinMaxSize<8, 64>>, "factWifiApKey" };
  ConfigWrapper<std::string> wifiApKey { factoryWifiApKey, StringOr<StringEmpty, StringMinMaxSize<8, 64>>, "wifiApKey" };
  ConfigWrapper<uint8_t> wifiApChannel { 1, DoReset, {}, "wifiApChannel" };
  ConfigWrapper<wifi_auth_mode_t> wifiApAuthmode { WIFI_AUTH_WPA2_PSK, DoReset, {}, "wifiApAuthmode" };

  auto getAllConfigParams()
  {
    return cpputils::make_array(
        std::ref<ConfigWrapperInterface>(baseMacAddressOverride),
        std::ref<ConfigWrapperInterface>(hostname),

        std::ref<ConfigWrapperInterface>(wifiStaEnabled),
        std::ref<ConfigWrapperInterface>(wifi_configs[0].ssid),
        std::ref<ConfigWrapperInterface>(wifi_configs[0].key),
        std::ref<ConfigWrapperInterface>(wifi_configs[0].useStaticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[0].staticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[0].staticSubnet),
        std::ref<ConfigWrapperInterface>(wifi_configs[0].staticGateway),
        std::ref<ConfigWrapperInterface>(wifi_configs[0].useStaticDns),
        std::ref<ConfigWrapperInterface>(wifi_configs[0].staticDns0),
        std::ref<ConfigWrapperInterface>(wifi_configs[0].staticDns1),
        std::ref<ConfigWrapperInterface>(wifi_configs[0].staticDns2),
        std::ref<ConfigWrapperInterface>(wifi_configs[1].ssid),
        std::ref<ConfigWrapperInterface>(wifi_configs[1].key),
        std::ref<ConfigWrapperInterface>(wifi_configs[1].useStaticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[1].staticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[1].staticSubnet),
        std::ref<ConfigWrapperInterface>(wifi_configs[1].staticGateway),
        std::ref<ConfigWrapperInterface>(wifi_configs[1].useStaticDns),
        std::ref<ConfigWrapperInterface>(wifi_configs[1].staticDns0),
        std::ref<ConfigWrapperInterface>(wifi_configs[1].staticDns1),
        std::ref<ConfigWrapperInterface>(wifi_configs[1].staticDns2),
        std::ref<ConfigWrapperInterface>(wifi_configs[2].ssid),
        std::ref<ConfigWrapperInterface>(wifi_configs[2].key),
        std::ref<ConfigWrapperInterface>(wifi_configs[2].useStaticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[2].staticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[2].staticSubnet),
        std::ref<ConfigWrapperInterface>(wifi_configs[2].staticGateway),
        std::ref<ConfigWrapperInterface>(wifi_configs[2].useStaticDns),
        std::ref<ConfigWrapperInterface>(wifi_configs[2].staticDns0),
        std::ref<ConfigWrapperInterface>(wifi_configs[2].staticDns1),
        std::ref<ConfigWrapperInterface>(wifi_configs[2].staticDns2),
        std::ref<ConfigWrapperInterface>(wifi_configs[3].ssid),
        std::ref<ConfigWrapperInterface>(wifi_configs[3].key),
        std::ref<ConfigWrapperInterface>(wifi_configs[3].useStaticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[3].staticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[3].staticSubnet),
        std::ref<ConfigWrapperInterface>(wifi_configs[3].staticGateway),
        std::ref<ConfigWrapperInterface>(wifi_configs[3].useStaticDns),
        std::ref<ConfigWrapperInterface>(wifi_configs[3].staticDns0),
        std::ref<ConfigWrapperInterface>(wifi_configs[3].staticDns1),
        std::ref<ConfigWrapperInterface>(wifi_configs[3].staticDns2),
        std::ref<ConfigWrapperInterface>(wifi_configs[4].ssid),
        std::ref<ConfigWrapperInterface>(wifi_configs[4].key),
        std::ref<ConfigWrapperInterface>(wifi_configs[4].useStaticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[4].staticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[4].staticSubnet),
        std::ref<ConfigWrapperInterface>(wifi_configs[4].staticGateway),
        std::ref<ConfigWrapperInterface>(wifi_configs[4].useStaticDns),
        std::ref<ConfigWrapperInterface>(wifi_configs[4].staticDns0),
        std::ref<ConfigWrapperInterface>(wifi_configs[4].staticDns1),
        std::ref<ConfigWrapperInterface>(wifi_configs[4].staticDns2),
        std::ref<ConfigWrapperInterface>(wifi_configs[5].ssid),
        std::ref<ConfigWrapperInterface>(wifi_configs[5].key),
        std::ref<ConfigWrapperInterface>(wifi_configs[5].useStaticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[5].staticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[5].staticSubnet),
        std::ref<ConfigWrapperInterface>(wifi_configs[5].staticGateway),
        std::ref<ConfigWrapperInterface>(wifi_configs[5].useStaticDns),
        std::ref<ConfigWrapperInterface>(wifi_configs[5].staticDns0),
        std::ref<ConfigWrapperInterface>(wifi_configs[5].staticDns1),
        std::ref<ConfigWrapperInterface>(wifi_configs[5].staticDns2),
        std::ref<ConfigWrapperInterface>(wifi_configs[6].ssid),
        std::ref<ConfigWrapperInterface>(wifi_configs[6].key),
        std::ref<ConfigWrapperInterface>(wifi_configs[6].useStaticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[6].staticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[6].staticSubnet),
        std::ref<ConfigWrapperInterface>(wifi_configs[6].staticGateway),
        std::ref<ConfigWrapperInterface>(wifi_configs[6].useStaticDns),
        std::ref<ConfigWrapperInterface>(wifi_configs[6].staticDns0),
        std::ref<ConfigWrapperInterface>(wifi_configs[6].staticDns1),
        std::ref<ConfigWrapperInterface>(wifi_configs[6].staticDns2),
        std::ref<ConfigWrapperInterface>(wifi_configs[7].ssid),
        std::ref<ConfigWrapperInterface>(wifi_configs[7].key),
        std::ref<ConfigWrapperInterface>(wifi_configs[7].useStaticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[7].staticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[7].staticSubnet),
        std::ref<ConfigWrapperInterface>(wifi_configs[7].staticGateway),
        std::ref<ConfigWrapperInterface>(wifi_configs[7].useStaticDns),
        std::ref<ConfigWrapperInterface>(wifi_configs[7].staticDns0),
        std::ref<ConfigWrapperInterface>(wifi_configs[7].staticDns1),
        std::ref<ConfigWrapperInterface>(wifi_configs[7].staticDns2),
        std::ref<ConfigWrapperInterface>(wifi_configs[8].ssid),
        std::ref<ConfigWrapperInterface>(wifi_configs[8].key),
        std::ref<ConfigWrapperInterface>(wifi_configs[8].useStaticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[8].staticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[8].staticSubnet),
        std::ref<ConfigWrapperInterface>(wifi_configs[8].staticGateway),
        std::ref<ConfigWrapperInterface>(wifi_configs[8].useStaticDns),
        std::ref<ConfigWrapperInterface>(wifi_configs[8].staticDns0),
        std::ref<ConfigWrapperInterface>(wifi_configs[8].staticDns1),
        std::ref<ConfigWrapperInterface>(wifi_configs[8].staticDns2),
        std::ref<ConfigWrapperInterface>(wifi_configs[9].ssid),
        std::ref<ConfigWrapperInterface>(wifi_configs[9].key),
        std::ref<ConfigWrapperInterface>(wifi_configs[9].useStaticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[9].staticIp),
        std::ref<ConfigWrapperInterface>(wifi_configs[9].staticSubnet),
        std::ref<ConfigWrapperInterface>(wifi_configs[9].staticGateway),
        std::ref<ConfigWrapperInterface>(wifi_configs[9].useStaticDns),
        std::ref<ConfigWrapperInterface>(wifi_configs[9].staticDns0),
        std::ref<ConfigWrapperInterface>(wifi_configs[9].staticDns1),
        std::ref<ConfigWrapperInterface>(wifi_configs[9].staticDns2),

        std::ref<ConfigWrapperInterface>(wifiStaMinRssi),

        std::ref<ConfigWrapperInterface>(wifiApEnabled),
        std::ref<ConfigWrapperInterface>(factoryWifiApName),
        std::ref<ConfigWrapperInterface>(wifiApName),
        std::ref<ConfigWrapperInterface>(factoryWifiApKey),
        std::ref<ConfigWrapperInterface>(wifiApKey),
        std::ref<ConfigWrapperInterface>(wifiApChannel),
        std::ref<ConfigWrapperInterface>(wifiApAuthmode));
  }
};

extern ConfigManager<ConfigContainer> configs;
