#pragma once

#include "sdkconfig.h"

// system includes
#include <array>
#include <optional>
#include <string>

// esp-idf includes
#include <esp_sntp.h>

// 3rdparty lib includes
#include <configconstraints_base.h>
#include <configconstraints_espchrono.h>
#include <configmanager.h>
#include <configwrapper.h>
#include <espchrono.h>
#include <espwifiutils.h>
#include <fmt/core.h>
#include <makearray.h>

// local includes
#include "battery.h"
#include "bobbyquickactions.h"
#include "displays/lockscreen.h"
#include "handbremse.h"
#include "ledstrip.h"
#include "unifiedmodelmode.h"
#include "displays/lockscreen.h"
#include "handbremse.h"
#include "bobbyquickactions.h"
#include "cloud.h"

using namespace espconfig;

std::string defaultHostname();
std::string defaultUsername();

constexpr const auto INPUT_MAPPING_NONE = std::numeric_limits<uint8_t>::max();

enum class AllowReset { NoReset, DoReset };
constexpr auto NoReset = AllowReset::NoReset;
constexpr auto DoReset = AllowReset::DoReset;

template<typename T>
class ConfigWrapperLegacy final : public ConfigWrapper<T>
{
    CPP_DISABLE_COPY_MOVE(ConfigWrapperLegacy)
    using Base = ConfigWrapper<T>;

public:
    using value_t = typename Base::value_t;
    using ConstraintCallback = typename Base::ConstraintCallback;

    using DefaultValueCallbackRef = T(&)();
    using DefaultValueCallbackPtr = T(*)();

    ConfigWrapperLegacy(const T &defaultValue, AllowReset allowReset, ConstraintCallback constraintCallback, const char *nvsName) :
            m_allowReset{allowReset == AllowReset::DoReset},
            m_nvsName{nvsName},
            m_defaultType{DefaultByValue},
            m_defaultValue{defaultValue},
            m_constraintCallback{constraintCallback}
    {}

    ConfigWrapperLegacy(T &&defaultValue, AllowReset allowReset, ConstraintCallback constraintCallback, const char *nvsName) :
            m_allowReset{allowReset == AllowReset::DoReset},
            m_nvsName{nvsName},
            m_defaultType{DefaultByValue},
            m_defaultValue{std::move(defaultValue)},
            m_constraintCallback{constraintCallback}
    {}

    ConfigWrapperLegacy(const ConfigWrapper<T> &factoryConfig, ConstraintCallback constraintCallback, const char *nvsName) :
            m_allowReset{true},
            m_nvsName{nvsName},
            m_defaultType{DefaultByFactoryConfig},
            m_factoryConfig{&factoryConfig},
            m_constraintCallback{constraintCallback}
    {}

    ConfigWrapperLegacy(const DefaultValueCallbackRef &defaultCallback, AllowReset allowReset, ConstraintCallback constraintCallback, const char *nvsName) :
            m_allowReset{allowReset == AllowReset::DoReset},
            m_nvsName{nvsName},
            m_defaultType{DefaultByCallback},
            m_defaultCallback{&defaultCallback},
            m_constraintCallback{constraintCallback}
    {}

    ~ConfigWrapperLegacy()
    {
        switch (m_defaultType)
        {
            case DefaultByValue:         m_defaultValue.~T(); break;
            case DefaultByFactoryConfig: /*m_factoryConfig.~typeof(m_factoryConfig)();*/ break;
            case DefaultByCallback:      /*m_defaultCallback.~typeof(m_defaultCallback)();*/ break;
        }
    }

    const char *nvsName() const override final { return m_nvsName; }
    bool allowReset() const override final { return m_allowReset; }
    T defaultValue() const override final
    {
        switch (m_defaultType)
        {
            case DefaultByValue:         return m_defaultValue;
            case DefaultByFactoryConfig: assert(m_factoryConfig->loaded()); return m_factoryConfig->value();
            case DefaultByCallback:      return m_defaultCallback();
        }

        __builtin_unreachable();
    }

    ConfigConstraintReturnType checkValue(value_t value) const override final
    {
        if (!m_constraintCallback)
            return {};

        return m_constraintCallback(value);
    }

private:
    const bool m_allowReset;
    const char * const m_nvsName;

    const enum : uint8_t { DefaultByValue, DefaultByFactoryConfig, DefaultByCallback } m_defaultType;

    union
    {
        const T m_defaultValue;
        const ConfigWrapper<T> * const m_factoryConfig;
        const DefaultValueCallbackPtr m_defaultCallback;
    };

    const ConstraintCallback m_constraintCallback;
};

class WiFiConfig
{
public:
    WiFiConfig(const char *ssidNvsKey, const char *keyNvsKey,
               const char *useStaticIpKey, const char *staticIpKey, const char *staticSubnetKey, const char *staticGatewayKey,
               const char *useStaticDnsKey, const char *staticDns0Key, const char *staticDns1Key, const char *staticDns2Key) :
        ssid         {std::string{},              DoReset, StringMaxSize<32>,                              ssidNvsKey         },
        key          {std::string{},              DoReset, StringOr<StringEmpty, StringMinMaxSize<8, 64>>, keyNvsKey          },
        useStaticIp  {false,                      DoReset, {},                                             useStaticIpKey     },
        staticIp     {wifi_stack::ip_address_t{}, DoReset, {},                                             staticIpKey        },
        staticSubnet {wifi_stack::ip_address_t{}, DoReset, {},                                             staticSubnetKey    },
        staticGateway{wifi_stack::ip_address_t{}, DoReset, {},                                             staticGatewayKey   },
        useStaticDns {false,                      DoReset, {},                                             useStaticDnsKey    },
        staticDns0   {wifi_stack::ip_address_t{}, DoReset, {},                                             staticDns0Key      },
        staticDns1   {wifi_stack::ip_address_t{}, DoReset, {},                                             staticDns1Key      },
        staticDns2   {wifi_stack::ip_address_t{}, DoReset, {},                                             staticDns2Key      }
    {}

    WiFiConfig(const char *ssidNvsKey, const char *keyNvsKey,
               const char *useStaticIpKey, const char *staticIpKey, const char *staticSubnetKey, const char *staticGatewayKey,
               const char *useStaticDnsKey, const char *staticDns0Key, const char *staticDns1Key, const char *staticDns2Key, const char *default_ssid, const char *default_key) :
        ssid         {default_ssid,               DoReset, StringMaxSize<32>,                              ssidNvsKey         },
        key          {default_key,                DoReset, StringOr<StringEmpty, StringMinMaxSize<8, 64>>, keyNvsKey          },
        useStaticIp  {false,                      DoReset, {},                                             useStaticIpKey     },
        staticIp     {wifi_stack::ip_address_t{}, DoReset, {},                                             staticIpKey        },
        staticSubnet {wifi_stack::ip_address_t{}, DoReset, {},                                             staticSubnetKey    },
        staticGateway{wifi_stack::ip_address_t{}, DoReset, {},                                             staticGatewayKey   },
        useStaticDns {false,                      DoReset, {},                                             useStaticDnsKey    },
        staticDns0   {wifi_stack::ip_address_t{}, DoReset, {},                                             staticDns0Key      },
        staticDns1   {wifi_stack::ip_address_t{}, DoReset, {},                                             staticDns1Key      },
        staticDns2   {wifi_stack::ip_address_t{}, DoReset, {},                                             staticDns2Key      }
    {}

    ConfigWrapperLegacy<std::string> ssid;
    ConfigWrapperLegacy<std::string> key;
    ConfigWrapperLegacy<bool> useStaticIp;
    ConfigWrapperLegacy<wifi_stack::ip_address_t> staticIp;
    ConfigWrapperLegacy<wifi_stack::ip_address_t> staticSubnet;
    ConfigWrapperLegacy<wifi_stack::ip_address_t> staticGateway;
    ConfigWrapperLegacy<bool> useStaticDns;
    ConfigWrapperLegacy<wifi_stack::ip_address_t> staticDns0;
    ConfigWrapperLegacy<wifi_stack::ip_address_t> staticDns1;
    ConfigWrapperLegacy<wifi_stack::ip_address_t> staticDns2;
};

class WirelessDoorsConfig
{
public:
    WirelessDoorsConfig(const char *doorIdKey, const char *doorTokenKey) :
        doorId    {std::string{}, DoReset, StringMaxSize<24>, doorIdKey    },
        doorToken {std::string{}, DoReset, StringMaxSize<24>, doorTokenKey }
    {}

    ConfigWrapperLegacy<std::string> doorId;
    ConfigWrapperLegacy<std::string> doorToken;
};

class ConfiguredOtaServer
{
public:
    ConfiguredOtaServer(const char *nameKey, const char *urlKey) :
        name{std::string{}, DoReset, {},                                    nameKey },
        url {std::string{}, DoReset, StringOr<StringEmpty, StringValidUrl>, urlKey  }
    {}

    ConfiguredOtaServer(const char *nameKey, const char *urlKey, const char *default_name, const char *default_url) :
        name{default_name, DoReset, {},                                    nameKey },
        url {default_url,  DoReset, StringOr<StringEmpty, StringValidUrl>, urlKey  }
    {}

    ConfigWrapperLegacy<std::string> name;
    ConfigWrapperLegacy<std::string> url;
};

class ConfiguredFeatureFlag
{
public:
    ConfiguredFeatureFlag(const char *enabledKey, const bool default_enabled = false, const bool default_is_beta = false, const char* taskName = "") :
            isEnabled{default_enabled, DoReset, {}, enabledKey },
            m_isBeta{default_is_beta},
            m_taskName{taskName}
    {}

    ConfigWrapperLegacy<bool> isEnabled;
    bool isBeta() const { return m_isBeta; }
    std::string getTaskName() const { return m_taskName; }
private:
    const bool m_isBeta;
    const std::string m_taskName;
};

class ConfigContainer
{
    using mac_t = wifi_stack::mac_t;

public:
    //                                            default                                 allowReset constraints                    nvsName
    ConfigWrapperLegacy<std::optional<mac_t>> baseMacAddressOverride{std::nullopt,              DoReset,   {},                            "baseMacAddrOver"     };
    ConfigWrapperLegacy<std::string> hostname           {defaultUsername,                       DoReset,   StringMinMaxSize<4, 32>,       "hostname"            };
    ConfigWrapperLegacy<bool>        wifiStaEnabled     {true,                                  DoReset,   {},                            "wifiStaEnabled"      };
    std::array<WiFiConfig, 10> wifi_configs {
        WiFiConfig {"wifi_ssid0", "wifi_key0", "wifi_usestatic0", "wifi_static_ip0", "wifi_stati_sub0", "wifi_stat_gate0", "wifi_usestadns0", "wifi_stat_dnsA0", "wifi_stat_dnsB0", "wifi_stat_dnsC0", "bobbycar", "12345678"},
        WiFiConfig {"wifi_ssid1", "wifi_key1", "wifi_usestatic1", "wifi_static_ip1", "wifi_stati_sub1", "wifi_stat_gate1", "wifi_usestadns1", "wifi_stat_dnsA1", "wifi_stat_dnsB1", "wifi_stat_dnsC1"},
        WiFiConfig {"wifi_ssid2", "wifi_key2", "wifi_usestatic2", "wifi_static_ip2", "wifi_stati_sub2", "wifi_stat_gate2", "wifi_usestadns2", "wifi_stat_dnsA2", "wifi_stat_dnsB2", "wifi_stat_dnsC2"},
        WiFiConfig {"wifi_ssid3", "wifi_key3", "wifi_usestatic3", "wifi_static_ip3", "wifi_stati_sub3", "wifi_stat_gate3", "wifi_usestadns3", "wifi_stat_dnsA3", "wifi_stat_dnsB3", "wifi_stat_dnsC3"},
        WiFiConfig {"wifi_ssid4", "wifi_key4", "wifi_usestatic4", "wifi_static_ip4", "wifi_stati_sub4", "wifi_stat_gate4", "wifi_usestadns4", "wifi_stat_dnsA4", "wifi_stat_dnsB4", "wifi_stat_dnsC4"},
        WiFiConfig {"wifi_ssid5", "wifi_key5", "wifi_usestatic5", "wifi_static_ip5", "wifi_stati_sub5", "wifi_stat_gate5", "wifi_usestadns5", "wifi_stat_dnsA5", "wifi_stat_dnsB5", "wifi_stat_dnsC5"},
        WiFiConfig {"wifi_ssid6", "wifi_key6", "wifi_usestatic6", "wifi_static_ip6", "wifi_stati_sub6", "wifi_stat_gate6", "wifi_usestadns6", "wifi_stat_dnsA6", "wifi_stat_dnsB6", "wifi_stat_dnsC6"},
        WiFiConfig {"wifi_ssid7", "wifi_key7", "wifi_usestatic7", "wifi_static_ip7", "wifi_stati_sub7", "wifi_stat_gate7", "wifi_usestadns7", "wifi_stat_dnsA7", "wifi_stat_dnsB7", "wifi_stat_dnsC7"},
        WiFiConfig {"wifi_ssid8", "wifi_key8", "wifi_usestatic8", "wifi_static_ip8", "wifi_stati_sub8", "wifi_stat_gate8", "wifi_usestadns8", "wifi_stat_dnsA8", "wifi_stat_dnsB8", "wifi_stat_dnsC8"},
        WiFiConfig {"wifi_ssid9", "wifi_key9", "wifi_usestatic9", "wifi_static_ip9", "wifi_stati_sub9", "wifi_stat_gate9", "wifi_usestadns9", "wifi_stat_dnsA9", "wifi_stat_dnsB9", "wifi_stat_dnsC9"}
    };
    ConfigWrapperLegacy<int8_t>      wifiStaMinRssi     {-90,                                    DoReset,   {},                           "wifiStaMinRssi"      };

    ConfigWrapperLegacy<bool>        wifiApEnabled      {true,                                   DoReset,   {},                           "wifiApEnabled"       };
    ConfigWrapperLegacy<std::string> wifiApName         {defaultUsername,                        DoReset,   StringMinMaxSize<4, 32>,      "wifiApName"          };
    ConfigWrapperLegacy<std::string> wifiApKey          {"Passwort_123",                         DoReset,   StringOr<StringEmpty, StringMinMaxSize<8, 64>>, "wifiApKey" };
    ConfigWrapperLegacy<wifi_stack::ip_address_t> wifiApIp{wifi_stack::ip_address_t{10, 0, 0, 1},DoReset,   {},                           "wifiApIp"            };
    ConfigWrapperLegacy<wifi_stack::ip_address_t> wifiApMask{wifi_stack::ip_address_t{255, 255, 255, 0},DoReset, {},                      "wifiApMask"          };
    ConfigWrapperLegacy<uint8_t>     wifiApChannel      {1,                                      DoReset,   MinMaxValue<uint8_t, 1, 14>,  "wifiApChannel"       };
    ConfigWrapperLegacy<wifi_auth_mode_t> wifiApAuthmode{WIFI_AUTH_WPA2_PSK,                     DoReset,   {},                           "wifiApAuthmode"      };
    ConfigWrapperLegacy<bool>        wifiApHidden       {false,                                  DoReset,   {},                           "wifiApHidden"        };

    ConfigWrapperLegacy<bool>     timeServerEnabled     {true,                                   DoReset,   {},                           "timeServerEnabl"     };
    ConfigWrapperLegacy<std::string>   timeServer       {"europe.pool.ntp.org",                  DoReset,   StringMaxSize<64>,            "timeServer"          };
    ConfigWrapperLegacy<sntp_sync_mode_t> timeSyncMode  {SNTP_SYNC_MODE_IMMED,                   DoReset,   {},                           "timeSyncMode"        };
    ConfigWrapperLegacy<espchrono::milliseconds32> timeSyncInterval{espchrono::milliseconds32{CONFIG_LWIP_SNTP_UPDATE_DELAY}, DoReset, MinTimeSyncInterval, "timeSyncInterva" };
    ConfigWrapperLegacy<espchrono::minutes32> timezoneOffset{espchrono::minutes32{60},           DoReset,   {},                           "timezoneOffset"      }; // MinMaxValue<minutes32, -1440m, 1440m>
    ConfigWrapperLegacy<espchrono::DayLightSavingMode>timeDst{espchrono::DayLightSavingMode::EuropeanSummerTime, DoReset, {},             "time_dst"            };

    ConfigWrapperLegacy<bool>           canResetOnError {false,                                  DoReset,   {},                           "canBusRstErr"        };
    ConfigWrapperLegacy<bool>       canUninstallOnReset {false,                                  DoReset,   {},                           "canUnstlRstErr"      };

    ConfigWrapperLegacy<int16_t>     sampleCount        {50,                                     DoReset,   {},                           "sampleCount"         };
    ConfigWrapperLegacy<int16_t>     gasMin             {0,                                      DoReset,  MinMaxValue<int16_t, 0, 4095>, "gasMin"              };
    ConfigWrapperLegacy<int16_t>     gasMax             {4095,                                   DoReset,  MinMaxValue<int16_t, 0, 4095>, "gasMax"              };
    ConfigWrapperLegacy<int16_t>     gasMitte           {2048,                                   DoReset,  MinMaxValue<int16_t, 0, 4095>, "gasMiddle"           };
    ConfigWrapperLegacy<int16_t>     bremsMin           {0,                                      DoReset,  MinMaxValue<int16_t, 0, 4095>, "bremsMin"            };
    ConfigWrapperLegacy<int16_t>     bremsMax           {4095,                                   DoReset,  MinMaxValue<int16_t, 0, 4095>, "bremsMax"            };
    ConfigWrapperLegacy<int16_t>     bremsMitte         {2048,                                   DoReset,  MinMaxValue<int16_t, 0, 4095>, "bremsMiddle"         };
    ConfigWrapperLegacy<uint16_t>    deadband           {20,                                     DoReset,  MinMaxValue<uint16_t, 0, 4095>,"deadband"            };

    ConfigWrapperLegacy<uint8_t>     dpadDebounce       {25,                                     DoReset,   {},                           "dpadDebounce"        };
    ConfigWrapperLegacy<uint16_t>    buttonReadDelay    {20,                                      DoReset,   {},                           "buttonDelay"         };

    ConfigWrapperLegacy<uint8_t>     dpadMappingLeft    {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapLeft"         };
    ConfigWrapperLegacy<uint8_t>     dpadMappingRight   {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapRight"        };
    ConfigWrapperLegacy<uint8_t>     dpadMappingUp      {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapUp"           };
    ConfigWrapperLegacy<uint8_t>     dpadMappingDown    {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapDown"         };
    ConfigWrapperLegacy<uint8_t>     dpadMappingProfile0{INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapProfile0"     };
    ConfigWrapperLegacy<uint8_t>     dpadMappingProfile1{INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapProfile1"     };
    ConfigWrapperLegacy<uint8_t>     dpadMappingProfile2{INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapProfile2"     };
    ConfigWrapperLegacy<uint8_t>     dpadMappingProfile3{INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapProfile3"     };
    ConfigWrapperLegacy<uint8_t>     dpadMappingLeft2   {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapLeft2"        };
    ConfigWrapperLegacy<uint8_t>     dpadMappingRight2  {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapRight2"       };
    ConfigWrapperLegacy<uint8_t>     dpadMappingUp2     {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapUp2"          };
    ConfigWrapperLegacy<uint8_t>     dpadMappingDown2   {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapDown2"        };
    ConfigWrapperLegacy<uint8_t>     dpadMappingExtra1  {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapExtra1"       };
    ConfigWrapperLegacy<uint8_t>     dpadMappingExtra2  {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapExtra2"       };
    ConfigWrapperLegacy<uint8_t>     dpadMappingExtra3  {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapExtra3"       };
    ConfigWrapperLegacy<uint8_t>     dpadMappingExtra4  {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapExtra4"       };

    ConfigWrapperLegacy<BobbyQuickActions> quickActionLeft2 { BobbyQuickActions::BLINK_LEFT,     DoReset,   {},                           "quickActleft2"       };
    ConfigWrapperLegacy<BobbyQuickActions> quickActionRight2{ BobbyQuickActions::BLINK_RIGHT,    DoReset,   {},                           "quickActright2"      };
    ConfigWrapperLegacy<BobbyQuickActions> quickActionUp2   { BobbyQuickActions::NONE,           DoReset,   {},                           "quickActup2"         };
    ConfigWrapperLegacy<BobbyQuickActions> quickActionDown2 { BobbyQuickActions::HANDBREMSE,     DoReset,   {},                           "quickActdown2"       };
    ConfigWrapperLegacy<BobbyQuickActions> quickActionExtra1{ BobbyQuickActions::NONE,           DoReset,   {},                           "quickActextra1"      };
    ConfigWrapperLegacy<BobbyQuickActions> quickActionExtra2{ BobbyQuickActions::NONE,           DoReset,   {},                           "quickActextra2"      };
    ConfigWrapperLegacy<BobbyQuickActions> quickActionExtra3{ BobbyQuickActions::NONE,           DoReset,   {},                           "quickActextra3"      };
    ConfigWrapperLegacy<BobbyQuickActions> quickActionExtra4{ BobbyQuickActions::NONE,           DoReset,   {},                           "quickActextra4"      };

    std::array<WirelessDoorsConfig, 5> wireless_door_configs {
        WirelessDoorsConfig { "door_id0", "door_token0" },
        WirelessDoorsConfig { "door_id1", "door_token1" },
        WirelessDoorsConfig { "door_id2", "door_token2" },
        WirelessDoorsConfig { "door_id3", "door_token3" },
        WirelessDoorsConfig { "door_id4", "door_token4" }
    };

    ConfigWrapperLegacy<std::string> bluetoothName      {defaultUsername,                        DoReset,   StringMinMaxSize<4, 32>,    "bluetoothName"       };

    ConfigWrapperLegacy<bool>        reverseBeep        {false,                                  DoReset,   {},                         "reverseBeep"         };
    ConfigWrapperLegacy<uint8_t>     reverseBeepFreq0   {3,                                      DoReset,   {},                         "revBeepFreq0"        };
    ConfigWrapperLegacy<uint8_t>     reverseBeepFreq1   {0,                                      DoReset,   {},                         "revBeepFreq1"        };
    ConfigWrapperLegacy<int16_t>     reverseBeepDuration0{500,                                   DoReset,   {},                         "revBeepDur0"         };
    ConfigWrapperLegacy<int16_t>     reverseBeepDuration1{500,                                   DoReset,   {},                         "revBeepDur1"         };

    ConfigWrapperLegacy<std::string> cloudUrl           {"ws://api.bobbycar.cloud/ws",                          DoReset,   StringOr<StringEmpty, StringValidUrl>, "cloudUrl"     };

    ConfigWrapperLegacy<std::string> otaUrl             {std::string{},                          DoReset,   StringOr<StringEmpty, StringValidUrl>, "otaUrl"   };
    ConfigWrapperLegacy<std::string> otaUsername        {std::string{BOBBY_DEFAULT_USERNAME},    DoReset,   {},                                 "otaUsername" };
    ConfigWrapperLegacy<std::string> otaServerUrl       {std::string{},                          DoReset,   StringOr<StringEmpty, StringValidUrl>, "otaServerUrl" };
    ConfigWrapperLegacy<std::string> otaServerBranch    {std::string{},                          DoReset,   {},                         "otaServerBranch"     };
    std::array<ConfiguredOtaServer, 5> otaServers {
        ConfiguredOtaServer { "otaName0", "otaUrl0", "bobbycloud", "http://updates.bobbycar.cloud" },
        ConfiguredOtaServer { "otaName1", "otaUrl1" },
        ConfiguredOtaServer { "otaName2", "otaUrl2" },
        ConfiguredOtaServer { "otaName3", "otaUrl3" },
        ConfiguredOtaServer { "otaName4", "otaUrl4" }
    };

    ConfigWrapperLegacy<bool>        dns_announce_enabled{true,                                  DoReset,   {},                         "dnsAnnounceEnab"     };
    ConfigWrapperLegacy<std::string> dns_announce_key   {std::string{},                          DoReset,   {},                         "dnsAnnounceKey"      };
    ConfigWrapperLegacy<std::string> webserverPassword  {"Passwort_123",                         DoReset,   {},                         "websPassword"        };

    struct {
        ConfigWrapperLegacy<int16_t>  wheelDiameter     {DEFAULT_WHEELDIAMETER,                  DoReset,   {},                         "wheelDiameter"       };
        ConfigWrapperLegacy<int16_t> numMagnetPoles     {15,                                     DoReset,   {},                         "numMagnetPoles"      };
        ConfigWrapperLegacy<bool>     swapFrontBack     {false,                                  DoReset,   {},                         "swapFrontBack"       };
        ConfigWrapperLegacy<bool>   sendFrontCanCmd     {true,                                   DoReset,   {},                         "sendFrontCanCmd"     };
        ConfigWrapperLegacy<bool>    sendBackCanCmd     {true,                                   DoReset,   {},                         "sendBackCanCmd"      };
        ConfigWrapperLegacy<int16_t> canTransmitTimeout {200,                                    DoReset,   {},                         "canTransmitTime"     };
        ConfigWrapperLegacy<int16_t> canReceiveTimeout  {0,                                      DoReset,   {},                         "canReceiveTimeo"     };
    } controllerHardware;

    struct {
        ConfigWrapperLegacy<int16_t> gametrakXMin       {0,                                      DoReset,   {},                         "gametrakXMin"        };
        ConfigWrapperLegacy<int16_t> gametrakXMax       {4095,                                   DoReset,   {},                         "gametrakXMax"        };
        ConfigWrapperLegacy<int16_t> gametrakYMin       {0,                                      DoReset,   {},                         "gametrakYMin"        };
        ConfigWrapperLegacy<int16_t> gametrakYMax       {4095,                                   DoReset,   {},                         "gametrakYMax"        };
        ConfigWrapperLegacy<int16_t> gametrakDistMin    {0,                                      DoReset,   {},                         "gametrakDistMin"     };
        ConfigWrapperLegacy<int16_t> gametrakDistMax    {4095,                                   DoReset,   {},                         "gametrakDistMax"     };
        struct {
            ConfigWrapperLegacy<int16_t> statsUpdateRate{50,                                     DoReset,   {},                         "statsUpdateRate"     };
            ConfigWrapperLegacy<int16_t> cloudCollectRate{100,                                   DoReset,   {},                         "cloudCollectRat"     };
            ConfigWrapperLegacy<int16_t> cloudSendRate  {1,                                      DoReset,   {},                         "cloudSendRate"       };
            ConfigWrapperLegacy<int16_t> udpSendRateMs  {65,                                     DoReset,   {},                         "udpSendRate"         };
        } timersSettings;
        ConfigWrapperLegacy<bool> flipScreen            {false,                                  NoReset,   {},                         "flipScreen"          };
        ConfigWrapperLegacy<bool> setupFinished         {false,                                  DoReset,   {},                         "setupFinished"       };
    } boardcomputerHardware;

    struct {
        ConfigWrapperLegacy<bool> cloudEnabled          {false,                                  DoReset,   {},                         "cloudEnabled"        };
        ConfigWrapperLegacy<int16_t> cloudTransmitTimeout{10,                                    DoReset,   {},                         "clodTransmTmout"     };
        ConfigWrapperLegacy<std::string> cloudKey       {std::string{},                          DoReset,   {},                         "cloudKey"            };
        ConfigWrapperLegacy<bool> sendStatistic         {false,                                  DoReset,   {},                         "cloudSendStats"      };
    } cloudSettings;

    struct {
        ConfigWrapperLegacy<std::string> udpToken       {std::string{},                          DoReset,   {},                         "udpToken"            };
        ConfigWrapperLegacy<bool> udpCloudEnabled       {false,                                  DoReset,   {},                         "enUdpCloud"          };
        ConfigWrapperLegacy<bool> enableCloudDebug      {false,                                  DoReset,   {},                         "debugCloud"          };
        ConfigWrapperLegacy<uint16_t> udpCloudPort      {24243,                                  DoReset,   {},                         "udpCloudPort"        };
        ConfigWrapperLegacy<std::string> udpCloudHost   {"updates.bobbycar.cloud",               DoReset,   {},                         "udpCloudHost"        };
    } udpCloudSettings;

    struct {
        ConfigWrapperLegacy<bool> enableLedAnimation    {true,                                   DoReset,   {},                         "enableLedAnimat"     };
        ConfigWrapperLegacy<bool> enableBrakeLights     {true,                                   DoReset,   {},                         "enableBrakeLigh"     };
        ConfigWrapperLegacy<int16_t> ledsCount          {288,                                    DoReset,   {},                         "ledsCount"           };
        ConfigWrapperLegacy<int16_t> centerOffset       {1,                                      DoReset,   {},                         "centerOffset"        };
        ConfigWrapperLegacy<int16_t> smallOffset        {4,                                      DoReset,   {},                         "smallOffset"         };
        ConfigWrapperLegacy<int16_t> bigOffset          {10,                                     DoReset,   {},                         "bigOffset"           };
        ConfigWrapperLegacy<bool> enableBeepWhenBlink   {true,                                   DoReset,   {},                         "beepwhenblink"       };
        ConfigWrapperLegacy<LedstripAnimation> animationType{LedstripAnimation::DefaultRainbow,  DoReset,   {},                         "animationType"       };
        ConfigWrapperLegacy<bool> enableFullBlink       {true,                                   DoReset,   {},                         "fullblink"           };
        ConfigWrapperLegacy<bool> enableStVO            {true,                                   DoReset,   {},                         "ledstvo"             };
        ConfigWrapperLegacy<int16_t> stvoFrontOffset    {0,                                      DoReset,   {},                         "ledstvofoff"         };
        ConfigWrapperLegacy<int16_t> stvoFrontLength    {10,                                     DoReset,   {},                         "ledstvoflen"         };
        ConfigWrapperLegacy<bool> stvoFrontEnable       {false,                                  DoReset,   {},                         "ledstvoen"           };
        ConfigWrapperLegacy<int16_t> animationMultiplier{10,                                     DoReset,   {},                         "ledAnimMul"          };
        ConfigWrapperLegacy<uint8_t> brightness         {255,                                    DoReset,   {},                         "ledbrightness"       };
        ConfigWrapperLegacy<bool> enableAnimBlink       {false,                                  DoReset,   {},                         "enAnimBlink"         };
        ConfigWrapperLegacy<OtaAnimationModes> otaMode  {OtaAnimationModes::GreenProgressBar,    DoReset,   {},                         "ledOtaAnim"          };
        ConfigWrapperLegacy<uint32_t> maxMilliamps      {3000,                                   DoReset,   {},                         "ledMaxMilliamps"     };
        ConfigWrapperLegacy<bool> enableVisualizeBlink  {false,                                  DoReset,   {},                         "enVisualBlink"       };
        std::array<ConfigWrapperLegacy<uint32_t>, 8> custom_color {
            ConfigWrapperLegacy<uint32_t>                   {0,                                  DoReset,   {},                         "ledCustomCol1"       },
            ConfigWrapperLegacy<uint32_t>                   {0,                                  DoReset,   {},                         "ledCustomCol2"       },
            ConfigWrapperLegacy<uint32_t>                   {0,                                  DoReset,   {},                         "ledCustomCol3"       },
            ConfigWrapperLegacy<uint32_t>                   {0,                                  DoReset,   {},                         "ledCustomCol4"       },
            ConfigWrapperLegacy<uint32_t>                   {0,                                  DoReset,   {},                         "ledCustomCol5"       },
            ConfigWrapperLegacy<uint32_t>                   {0,                                  DoReset,   {},                         "ledCustomCol6"       },
            ConfigWrapperLegacy<uint32_t>                   {0,                                  DoReset,   {},                         "ledCustomCol7"       },
            ConfigWrapperLegacy<uint32_t>                   {0,                                  DoReset,   {},                         "ledCustomCol8"       },
        };
        ConfigWrapperLegacy<uint8_t> leds_per_meter     {144,                                    DoReset,   {},                         "ledsPerMeter"        };
        ConfigWrapperLegacy<bool> automaticLight        {false,                                  DoReset,   {},                         "nightLights"         };
        ConfigWrapperLegacy<bool> brakeLights_useAccel  {false,                                  DoReset,   {},                         "brakeLightsA"    };
        ConfigWrapperLegacy<bool> brakeLights_usePower  {false,                                  DoReset,   {},                         "brakeLightsP"    };
    } ledstrip;

    struct {
        ConfigWrapperLegacy<uint8_t> cellsSeries        {12,                                     DoReset,   {},                         "batteryCS"           };
        ConfigWrapperLegacy<uint8_t> cellsParallel      {10,                                     DoReset,   {},                         "batteryCP"           };
        ConfigWrapperLegacy<BatteryCellType> cellType   {BatteryCellType::_22P,                  DoReset,   {},                         "batteryType"         };
        ConfigWrapperLegacy<uint16_t> watthoursPerKilometer{25,                                  DoReset,   {},                         "whkm"                };
        ConfigWrapperLegacy<int16_t> front30VoltCalibration{3000,                                DoReset,   {},                         "batF30VCal"          };
        ConfigWrapperLegacy<int16_t> back30VoltCalibration {3000,                                DoReset,   {},                         "batB30VCal"          };
        ConfigWrapperLegacy<int16_t> front50VoltCalibration{5000,                                DoReset,   {},                         "batF50VCal"          };
        ConfigWrapperLegacy<int16_t> back50VoltCalibration {5000,                                DoReset,   {},                         "batB50VCal"          };
        ConfigWrapperLegacy<bool> applyCalibration      {true,                                   DoReset,   {},                         "applyBatCal"         };
    } battery;

    struct {
        ConfigWrapperLegacy<bool> allowPresetSwitch     {true,                                   DoReset,   {},                         "lockAlwPresetSw"     };
        ConfigWrapperLegacy<bool> keepLockedAfterReboot {false,                                  DoReset,   {},                         "keepLocked"          };
        ConfigWrapperLegacy<bool> locked                {false,                                  DoReset,   {},                         "currentlyLocked"     };
        std::array<ConfigWrapperLegacy<int8_t>, 4> pin {
            ConfigWrapperLegacy<int8_t>                 {1,                                      DoReset,   MinMaxValue<int8_t, 0, 9>,  "lockscreenPin0"      },
            ConfigWrapperLegacy<int8_t>                 {2,                                      DoReset,   MinMaxValue<int8_t, 0, 9>,  "lockscreenPin1"      },
            ConfigWrapperLegacy<int8_t>                 {3,                                      DoReset,   MinMaxValue<int8_t, 0, 9>,  "lockscreenPin2"      },
            ConfigWrapperLegacy<int8_t>                 {4,                                      DoReset,   MinMaxValue<int8_t, 0, 9>,  "lockscreenPin3"      },
        };
        std::array<ConfigWrapperLegacy<int8_t>, 4> pin2 {
            ConfigWrapperLegacy<int8_t>                 {0,                                      DoReset,   MinMaxValue<int8_t, 0, 9>,  "lockscrnPin1_0"      },
            ConfigWrapperLegacy<int8_t>                 {0,                                      DoReset,   MinMaxValue<int8_t, 0, 9>,  "lockscrnPin1_1"      },
            ConfigWrapperLegacy<int8_t>                 {0,                                      DoReset,   MinMaxValue<int8_t, 0, 9>,  "lockscrnPin1_2"      },
            ConfigWrapperLegacy<int8_t>                 {0,                                      DoReset,   MinMaxValue<int8_t, 0, 9>,  "lockscrnPin1_3"      },
        };
    } lockscreen;

    struct {
        ConfigWrapperLegacy<uint32_t> totalCentimeters  {0,                                      DoReset,   {},                         "totalCentimeter"     };
    } savedStatistics;

    struct {
        ConfigWrapperLegacy<uint8_t> measurementInterval{1,                                      DoReset,   {},                         "measurementiv"       };
    } measurementMode;

    struct {
        ConfigWrapperLegacy<HandbremseMode> mode        {HandbremseMode::MOSFETS_OFF,            DoReset,   {},                         "handBremsM"          };
        ConfigWrapperLegacy<uint16_t> triggerTimeout    {10,                                     DoReset,   {},                         "handBremsT"          };
        ConfigWrapperLegacy<bool> automatic             {false,                                  DoReset,   {},                         "handBremsA"          };
        ConfigWrapperLegacy<bool> enable                {false,                                  DoReset,   {},                         "handBremsE"          };
        ConfigWrapperLegacy<bool> visualize             {false,                                  DoReset,   {},                         "handBremsV"          };
    } handbremse;

    struct {
        ConfigWrapperLegacy<bool> syncTime              {false,                                  DoReset,   {},                         "espnowSyncT"         };
        ConfigWrapperLegacy<bool> syncTimeWithOthers    {false,                                  DoReset,   {},                         "espnowSyncTWO"       };
        ConfigWrapperLegacy<bool> syncBlink             {false,                                  DoReset,   {},                         "espnowSyncBl"        };
    } espnow;

    struct {
        ConfiguredFeatureFlag ledstrip              {"f_ledstrip", false, false, "ledstrip"};
        ConfiguredFeatureFlag webserver_disable_lock{"f_no_web_lock", false, true};
        ConfiguredFeatureFlag garage                {"f_garage" };
        ConfiguredFeatureFlag cloud                 {"f_cloud", false, false, "cloud"};
        ConfiguredFeatureFlag udpcloud              {"f_udpcloud", false, false, "udpcloud"};
        ConfiguredFeatureFlag dnsannounce           {"f_dnsannounce", false, false, "dnsannounce"};
        ConfiguredFeatureFlag ntp                   {"f_ntp", false, false, "time"};
        ConfiguredFeatureFlag ble                   {"f_ble", false, false, "ble"};
        ConfiguredFeatureFlag ota                   {"f_ota", false, false, "ota"};
        ConfiguredFeatureFlag webserver             {"featureWebserv", true, false, "webserver"};
        ConfiguredFeatureFlag gschissene_diode      {"featurDiodeHin"};
        ConfiguredFeatureFlag esp_now               {"featureEspNow", false, false, "espnow"};
    } feature;

    ConfigWrapperLegacy<uint16_t> anhaenger_id          {0,                                      DoReset,   {},                         "anhaenger_id"        };
    ConfigWrapperLegacy<bool> emulateFeedback           {false,                                  DoReset,   {},                         "emuFeedback"         };

    struct {
        ConfigWrapperLegacy<bool> bleEnabled            {true,                                   DoReset,   {},                         "bleEnabled"          };
        ConfigWrapperLegacy<bool> bleFenceEnabled       {false,                                  DoReset,   {},                         "bleFenceEnabled"     };
    } bleSettings;

    ConfigWrapperLegacy<bool> setupDone             {false,                                      DoReset,   {},                         "setupDone"           };

#define NEW_SETTINGS(x) \
    x(baseMacAddressOverride) \
    x(hostname) \
    \
    x(wifiStaEnabled) \
    x(wifi_configs[0].ssid) \
    x(wifi_configs[0].key) \
    x(wifi_configs[0].useStaticIp) \
    x(wifi_configs[0].staticIp) \
    x(wifi_configs[0].staticSubnet) \
    x(wifi_configs[0].staticGateway) \
    x(wifi_configs[0].useStaticDns) \
    x(wifi_configs[0].staticDns0) \
    x(wifi_configs[0].staticDns1) \
    x(wifi_configs[0].staticDns2) \
    x(wifi_configs[1].ssid) \
    x(wifi_configs[1].key) \
    x(wifi_configs[1].useStaticIp) \
    x(wifi_configs[1].staticIp) \
    x(wifi_configs[1].staticSubnet) \
    x(wifi_configs[1].staticGateway) \
    x(wifi_configs[1].useStaticDns) \
    x(wifi_configs[1].staticDns0) \
    x(wifi_configs[1].staticDns1) \
    x(wifi_configs[1].staticDns2) \
    x(wifi_configs[2].ssid) \
    x(wifi_configs[2].key) \
    x(wifi_configs[2].useStaticIp) \
    x(wifi_configs[2].staticIp) \
    x(wifi_configs[2].staticSubnet) \
    x(wifi_configs[2].staticGateway) \
    x(wifi_configs[2].useStaticDns) \
    x(wifi_configs[2].staticDns0) \
    x(wifi_configs[2].staticDns1) \
    x(wifi_configs[2].staticDns2) \
    x(wifi_configs[3].ssid) \
    x(wifi_configs[3].key) \
    x(wifi_configs[3].useStaticIp) \
    x(wifi_configs[3].staticIp) \
    x(wifi_configs[3].staticSubnet) \
    x(wifi_configs[3].staticGateway) \
    x(wifi_configs[3].useStaticDns) \
    x(wifi_configs[3].staticDns0) \
    x(wifi_configs[3].staticDns1) \
    x(wifi_configs[3].staticDns2) \
    x(wifi_configs[4].ssid) \
    x(wifi_configs[4].key) \
    x(wifi_configs[4].useStaticIp) \
    x(wifi_configs[4].staticIp) \
    x(wifi_configs[4].staticSubnet) \
    x(wifi_configs[4].staticGateway) \
    x(wifi_configs[4].useStaticDns) \
    x(wifi_configs[4].staticDns0) \
    x(wifi_configs[4].staticDns1) \
    x(wifi_configs[4].staticDns2) \
    x(wifi_configs[5].ssid) \
    x(wifi_configs[5].key) \
    x(wifi_configs[5].useStaticIp) \
    x(wifi_configs[5].staticIp) \
    x(wifi_configs[5].staticSubnet) \
    x(wifi_configs[5].staticGateway) \
    x(wifi_configs[5].useStaticDns) \
    x(wifi_configs[5].staticDns0) \
    x(wifi_configs[5].staticDns1) \
    x(wifi_configs[5].staticDns2) \
    x(wifi_configs[6].ssid) \
    x(wifi_configs[6].key) \
    x(wifi_configs[6].useStaticIp) \
    x(wifi_configs[6].staticIp) \
    x(wifi_configs[6].staticSubnet) \
    x(wifi_configs[6].staticGateway) \
    x(wifi_configs[6].useStaticDns) \
    x(wifi_configs[6].staticDns0) \
    x(wifi_configs[6].staticDns1) \
    x(wifi_configs[6].staticDns2) \
    x(wifi_configs[7].ssid) \
    x(wifi_configs[7].key) \
    x(wifi_configs[7].useStaticIp) \
    x(wifi_configs[7].staticIp) \
    x(wifi_configs[7].staticSubnet) \
    x(wifi_configs[7].staticGateway) \
    x(wifi_configs[7].useStaticDns) \
    x(wifi_configs[7].staticDns0) \
    x(wifi_configs[7].staticDns1) \
    x(wifi_configs[7].staticDns2) \
    x(wifi_configs[8].ssid) \
    x(wifi_configs[8].key) \
    x(wifi_configs[8].useStaticIp) \
    x(wifi_configs[8].staticIp) \
    x(wifi_configs[8].staticSubnet) \
    x(wifi_configs[8].staticGateway) \
    x(wifi_configs[8].useStaticDns) \
    x(wifi_configs[8].staticDns0) \
    x(wifi_configs[8].staticDns1) \
    x(wifi_configs[8].staticDns2) \
    x(wifi_configs[9].ssid) \
    x(wifi_configs[9].key) \
    x(wifi_configs[9].useStaticIp) \
    x(wifi_configs[9].staticIp) \
    x(wifi_configs[9].staticSubnet) \
    x(wifi_configs[9].staticGateway) \
    x(wifi_configs[9].useStaticDns) \
    x(wifi_configs[9].staticDns0) \
    x(wifi_configs[9].staticDns1) \
    x(wifi_configs[9].staticDns2) \
    \
    x(wifiStaMinRssi) \
    \
    x(wifiApEnabled) \
    x(wifiApName) \
    x(wifiApKey) \
    x(wifiApIp) \
    x(wifiApMask) \
    x(wifiApChannel) \
    x(wifiApAuthmode) \
    x(wifiApHidden) \
    \
    x(timeServerEnabled) \
    x(timeServer) \
    x(timeSyncMode) \
    x(timeSyncInterval) \
    x(timezoneOffset) \
    x(timeDst) \
    \
    x(canResetOnError)  \
    x(canUninstallOnReset)    \
    \
    x(sampleCount) \
    x(gasMin) \
    x(gasMax) \
    x(gasMitte) \
    x(bremsMin) \
    x(bremsMax) \
    x(bremsMitte) \
    x(deadband) \
    \
    x(dpadDebounce) \
    x(buttonReadDelay) \
    \
    x(dpadMappingLeft) \
    x(dpadMappingRight) \
    x(dpadMappingUp) \
    x(dpadMappingDown) \
    x(dpadMappingProfile0) \
    x(dpadMappingProfile1) \
    x(dpadMappingProfile2) \
    x(dpadMappingProfile3) \
    x(dpadMappingLeft2) \
    x(dpadMappingRight2) \
    x(dpadMappingUp2) \
    x(dpadMappingDown2) \
    x(dpadMappingExtra1) \
    x(dpadMappingExtra2) \
    x(dpadMappingExtra3) \
    x(dpadMappingExtra4) \
    \
    x(quickActionLeft2) \
    x(quickActionRight2) \
    x(quickActionUp2) \
    x(quickActionDown2) \
    x(quickActionExtra1) \
    x(quickActionExtra2) \
    x(quickActionExtra3) \
    x(quickActionExtra4) \
    \
    x(wireless_door_configs[0].doorId) \
    x(wireless_door_configs[0].doorToken) \
    x(wireless_door_configs[1].doorId) \
    x(wireless_door_configs[1].doorToken) \
    x(wireless_door_configs[2].doorId) \
    x(wireless_door_configs[2].doorToken) \
    x(wireless_door_configs[3].doorId) \
    x(wireless_door_configs[3].doorToken) \
    x(wireless_door_configs[4].doorId) \
    x(wireless_door_configs[4].doorToken) \
    \
    x(bluetoothName) \
    \
    x(reverseBeep) \
    x(reverseBeepFreq0) \
    x(reverseBeepFreq1) \
    x(reverseBeepDuration0) \
    x(reverseBeepDuration1) \
    \
    x(cloudUrl) \
    \
    x(otaUrl) \
    x(otaUsername) \
    x(otaServerUrl) \
    x(otaServerBranch) \
    x(otaServers[0].name) \
    x(otaServers[0].url) \
    x(otaServers[1].name) \
    x(otaServers[1].url) \
    x(otaServers[2].name) \
    x(otaServers[2].url) \
    x(otaServers[3].name) \
    x(otaServers[3].url) \
    x(otaServers[4].name) \
    x(otaServers[4].url) \
    \
    x(dns_announce_enabled) \
    x(dns_announce_key) \
    x(webserverPassword) \
    \
    x(controllerHardware.wheelDiameter) \
    x(controllerHardware.numMagnetPoles) \
    x(controllerHardware.swapFrontBack) \
    x(controllerHardware.sendFrontCanCmd) \
    x(controllerHardware.sendBackCanCmd) \
    x(controllerHardware.canTransmitTimeout) \
    x(controllerHardware.canReceiveTimeout) \
    \
    x(boardcomputerHardware.gametrakXMin) \
    x(boardcomputerHardware.gametrakYMin) \
    x(boardcomputerHardware.gametrakXMax) \
    x(boardcomputerHardware.gametrakYMax) \
    x(boardcomputerHardware.gametrakDistMin) \
    x(boardcomputerHardware.gametrakDistMax) \
    \
    x(boardcomputerHardware.timersSettings.statsUpdateRate) \
    x(boardcomputerHardware.timersSettings.cloudCollectRate) \
    x(boardcomputerHardware.timersSettings.cloudSendRate) \
    x(boardcomputerHardware.timersSettings.udpSendRateMs) \
    \
    x(boardcomputerHardware.flipScreen) \
    x(boardcomputerHardware.setupFinished) \
    \
    x(cloudSettings.cloudEnabled) \
    x(cloudSettings.cloudTransmitTimeout) \
    x(cloudSettings.cloudKey) \
    x(cloudSettings.sendStatistic) \
    \
    x(udpCloudSettings.udpToken) \
    x(udpCloudSettings.udpCloudEnabled) \
    x(udpCloudSettings.enableCloudDebug)  \
    x(udpCloudSettings.udpCloudPort) \
    x(udpCloudSettings.udpCloudHost) \
    \
    x(ledstrip.enableLedAnimation) \
    x(ledstrip.enableBrakeLights) \
    x(ledstrip.ledsCount) \
    x(ledstrip.centerOffset) \
    x(ledstrip.smallOffset) \
    x(ledstrip.bigOffset) \
    x(ledstrip.enableBeepWhenBlink) \
    x(ledstrip.animationType) \
    x(ledstrip.enableFullBlink) \
    x(ledstrip.enableStVO) \
    x(ledstrip.stvoFrontOffset) \
    x(ledstrip.stvoFrontLength) \
    x(ledstrip.stvoFrontEnable) \
    x(ledstrip.animationMultiplier) \
    x(ledstrip.brightness) \
    x(ledstrip.enableAnimBlink) \
    x(ledstrip.otaMode) \
    x(ledstrip.maxMilliamps) \
    x(ledstrip.enableVisualizeBlink)\
    x(ledstrip.automaticLight) \
    x(ledstrip.brakeLights_useAccel) \
    x(ledstrip.brakeLights_usePower) \
    \
    x(ledstrip.custom_color[0]) \
    x(ledstrip.custom_color[1]) \
    x(ledstrip.custom_color[2]) \
    x(ledstrip.custom_color[3]) \
    x(ledstrip.custom_color[4]) \
    x(ledstrip.custom_color[5]) \
    x(ledstrip.custom_color[6]) \
    x(ledstrip.custom_color[7]) \
    \
    x(ledstrip.leds_per_meter) \
    \
    x(battery.cellsSeries) \
    x(battery.cellsParallel) \
    x(battery.cellType) \
    x(battery.watthoursPerKilometer) \
    x(battery.front30VoltCalibration) \
    x(battery.back30VoltCalibration) \
    x(battery.front50VoltCalibration) \
    x(battery.back50VoltCalibration) \
    x(battery.applyCalibration) \
    \
    x(lockscreen.allowPresetSwitch) \
    x(lockscreen.keepLockedAfterReboot) \
    x(lockscreen.locked) \
    x(lockscreen.pin[0]) \
    x(lockscreen.pin[1]) \
    x(lockscreen.pin[2]) \
    x(lockscreen.pin[3])\
    x(lockscreen.pin2[0]) \
    x(lockscreen.pin2[1]) \
    x(lockscreen.pin2[2]) \
    x(lockscreen.pin2[3]) \
    \
    x(savedStatistics.totalCentimeters) \
    \
    x(measurementMode.measurementInterval) \
    \
    x(handbremse.mode) \
    x(handbremse.triggerTimeout) \
    x(handbremse.automatic) \
    x(handbremse.enable) \
    x(handbremse.visualize) \
    \
    x(espnow.syncTime) \
    x(espnow.syncTimeWithOthers) \
    x(espnow.syncBlink) \
    \
    x(feature.ble.isEnabled) \
    x(feature.cloud.isEnabled) \
    x(feature.dnsannounce.isEnabled)\
    x(feature.esp_now.isEnabled) \
    x(feature.garage.isEnabled)    \
    x(feature.gschissene_diode.isEnabled) \
    x(feature.ledstrip.isEnabled) \
    x(feature.ntp.isEnabled) \
    x(feature.ota.isEnabled) \
    x(feature.udpcloud.isEnabled) \
    x(feature.webserver.isEnabled) \
    x(feature.webserver_disable_lock.isEnabled) \
    x(bleSettings.bleEnabled) \
    x(emulateFeedback)

#define FEATURES(x) \
    x(feature.ble) \
    x(feature.cloud) \
    x(feature.dnsannounce)\
    x(feature.esp_now) \
    x(feature.garage)    \
    x(feature.gschissene_diode) \
    x(feature.ledstrip) \
    x(feature.ntp) \
    x(feature.ota) \
    x(feature.udpcloud) \
    x(feature.webserver) \
//    x(feature.webserver_disable_lock)

    template<typename T>
    void callForEveryConfig(T &&callback)
    {
#define HELPER(x) callback(x);
        NEW_SETTINGS(HELPER)
#undef HELPER
        callback(bleSettings.bleFenceEnabled);
    }

    auto getAllConfigParams()
    {
        return cpputils::make_array(
#define HELPER(x) std::ref<ConfigWrapperInterface>(x),
            NEW_SETTINGS(HELPER)
#undef HELPER
            std::ref<ConfigWrapperInterface>(bleSettings.bleFenceEnabled)
        );
    }

    template<typename T>
    void callForEveryFeature(T &&callback)
    {
#define HELPER(x) callback(x);
        FEATURES(HELPER)
#undef HELPER
        callback(feature.webserver_disable_lock);
    }

    auto getAllFeatureParams()
    {
        return cpputils::make_array(
#define HELPER(x) std::ref<ConfiguredFeatureFlag>(x),
            FEATURES(HELPER)
#undef HELPER
            std::ref<ConfiguredFeatureFlag>(feature.webserver_disable_lock)
        );
    }

    constexpr size_t getConfigCount() const
    {
        size_t count = 0;
#define HELPER(x) count++;
        NEW_SETTINGS(HELPER)
#undef HELPER
        return count;
    }
};

extern ConfigManager<ConfigContainer> configs;
