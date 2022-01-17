#pragma once

#include "sdkconfig.h"

// system includes
#include <string>
#include <array>
#include <optional>

// esp-idf includes
#include <esp_sntp.h>

// 3rdparty lib includes
#include <fmt/core.h>
#include <configmanager.h>
#include <configconstraints_base.h>
#include <configconstraints_espchrono.h>
#include <configwrapper.h>
#include <espwifiutils.h>
#include <espchrono.h>
#include <makearray.h>

// local includes
#include "ledstrip.h"
#include "unifiedmodelmode.h"
#include "displays/lockscreen.h"
#include "handbremse.h"
#include "bobbyquickactions.h"

using namespace espconfig;

std::string defaultHostname();

constexpr const auto INPUT_MAPPING_NONE = std::numeric_limits<uint8_t>::max();

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

class WirelessDoorsConfig
{
public:
    WirelessDoorsConfig(const char *doorIdKey, const char *doorTokenKey) :
        doorId    {std::string{}, DoReset, StringMaxSize<24>, doorIdKey    },
        doorToken {std::string{}, DoReset, StringMaxSize<24>, doorTokenKey }
    {}

    ConfigWrapper<std::string> doorId;
    ConfigWrapper<std::string> doorToken;
};

class ConfiguredOtaServer
{
public:
    ConfiguredOtaServer(const char *nameKey, const char *urlKey) :
        name{std::string{}, DoReset, {},                                    nameKey },
        url {std::string{}, DoReset, StringOr<StringEmpty, StringValidUrl>, urlKey  }
    {}

    ConfigWrapper<std::string> name;
    ConfigWrapper<std::string> url;
};

class ConfigContainer
{
    using mac_t = wifi_stack::mac_t;

public:
    //                                            default                                 allowReset constraints                    nvsName
    ConfigWrapper<std::optional<mac_t>> baseMacAddressOverride{std::nullopt,              DoReset,   {},                            "baseMacAddrOver"     };
    ConfigWrapper<std::string> hostname           {defaultHostname,                       DoReset,   StringMinMaxSize<4, 32>,       "hostname"            };
    ConfigWrapper<bool>        wifiStaEnabled     {true,                                  DoReset,   {},                            "wifiStaEnabled"      };
    std::array<WiFiConfig, 10> wifi_configs {
        WiFiConfig {"wifi_ssid0", "wifi_key0", "wifi_usestatic0", "wifi_static_ip0", "wifi_stati_sub0", "wifi_stat_gate0", "wifi_usestadns0", "wifi_stat_dnsA0", "wifi_stat_dnsB0", "wifi_stat_dnsC0"},
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
    ConfigWrapper<int8_t>      wifiStaMinRssi     {-90,                                    DoReset,   {},                           "wifiStaMinRssi"      };

    ConfigWrapper<bool>        wifiApEnabled      {true,                                   DoReset,   {},                           "wifiApEnabled"       };
    ConfigWrapper<std::string> wifiApName         {defaultHostname,                        DoReset,   StringMinMaxSize<4, 32>,      "wifiApName"          };
    ConfigWrapper<std::string> wifiApKey          {"Passwort_123",                         DoReset,   StringOr<StringEmpty, StringMinMaxSize<8, 64>>, "wifiApKey" };
    ConfigWrapper<wifi_stack::ip_address_t> wifiApIp{wifi_stack::ip_address_t{10, 0, 0, 1},DoReset,   {},                           "wifiApIp"            };
    ConfigWrapper<wifi_stack::ip_address_t> wifiApMask{wifi_stack::ip_address_t{255, 255, 255, 0},DoReset, {},                      "wifiApMask"          };
    ConfigWrapper<uint8_t>     wifiApChannel      {1,                                      DoReset,   MinMaxValue<uint8_t, 1, 14>,  "wifiApChannel"       };
    ConfigWrapper<wifi_auth_mode_t> wifiApAuthmode{WIFI_AUTH_WPA2_PSK,                     DoReset,   {},                           "wifiApAuthmode"      };

    ConfigWrapper<bool>     timeServerEnabled     {true,                                   DoReset,   {},                           "timeServerEnabl"     };
    ConfigWrapper<std::string>   timeServer       {"europe.pool.ntp.org",                  DoReset,   StringMaxSize<64>,            "timeServer"          };
    ConfigWrapper<sntp_sync_mode_t> timeSyncMode  {SNTP_SYNC_MODE_IMMED,                   DoReset,   {},                           "timeSyncMode"        };
    ConfigWrapper<espchrono::milliseconds32> timeSyncInterval{espchrono::milliseconds32{CONFIG_LWIP_SNTP_UPDATE_DELAY}, DoReset, MinTimeSyncInterval, "timeSyncInterva" };
    ConfigWrapper<espchrono::minutes32> timezoneOffset{espchrono::minutes32{60},           DoReset,   {},                           "timezoneOffset"      }; // MinMaxValue<minutes32, -1440m, 1440m>
    ConfigWrapper<espchrono::DayLightSavingMode>timeDst{espchrono::DayLightSavingMode::EuropeanSummerTime, DoReset, {},             "time_dst"            };

    ConfigWrapper<bool>           canResetOnError {false,                                  DoReset,   {},                           "canBusRstErr"        };

    ConfigWrapper<int16_t>     sampleCount        {50,                                     DoReset,   {},                           "sampleCount"         };
    ConfigWrapper<int16_t>     gasMin             {0,                                      DoReset,  MinMaxValue<int16_t, 0, 4095>, "gasMin"              };
    ConfigWrapper<int16_t>     gasMax             {4095,                                   DoReset,  MinMaxValue<int16_t, 0, 4095>, "gasMax"              };
    ConfigWrapper<int16_t>     gasMitte           {2048,                                   DoReset,  MinMaxValue<int16_t, 0, 4095>, "gasMiddle"           };
    ConfigWrapper<int16_t>     bremsMin           {0,                                      DoReset,  MinMaxValue<int16_t, 0, 4095>, "bremsMin"            };
    ConfigWrapper<int16_t>     bremsMax           {4096,                                   DoReset,  MinMaxValue<int16_t, 0, 4095>, "bremsMax"            };
    ConfigWrapper<int16_t>     bremsMitte         {2048,                                   DoReset,  MinMaxValue<int16_t, 0, 4095>, "bremsMiddle"         };
    ConfigWrapper<uint16_t>    deadband           {20,                                     DoReset,  MinMaxValue<uint16_t, 0, 4095>,"deadband"            };

    ConfigWrapper<uint8_t>     dpadDebounce       {25,                                     DoReset,   {},                           "dpadDebounce"        };

    ConfigWrapper<uint8_t>     dpadMappingLeft    {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapLeft"         };
    ConfigWrapper<uint8_t>     dpadMappingRight   {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapRight"        };
    ConfigWrapper<uint8_t>     dpadMappingUp      {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapUp"           };
    ConfigWrapper<uint8_t>     dpadMappingDown    {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapDown"         };
    ConfigWrapper<uint8_t>     dpadMappingProfile0{INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapProfile0"     };
    ConfigWrapper<uint8_t>     dpadMappingProfile1{INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapProfile1"     };
    ConfigWrapper<uint8_t>     dpadMappingProfile2{INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapProfile2"     };
    ConfigWrapper<uint8_t>     dpadMappingProfile3{INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapProfile3"     };
    ConfigWrapper<uint8_t>     dpadMappingLeft2   {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapLeft2"        };
    ConfigWrapper<uint8_t>     dpadMappingRight2  {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapRight2"       };
    ConfigWrapper<uint8_t>     dpadMappingUp2     {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapUp2"          };
    ConfigWrapper<uint8_t>     dpadMappingDown2   {INPUT_MAPPING_NONE,                     DoReset,   {},                           "dpadMapDown2"        };

    ConfigWrapper<BobbyQuickActions> quickActionLeft2{  BobbyQuickActions::BLINK_LEFT,     DoReset,   {},                           "quickActleft2"       };
    ConfigWrapper<BobbyQuickActions> quickActionRight2{ BobbyQuickActions::BLINK_RIGHT,    DoReset,   {},                           "quickActright2"      };
    ConfigWrapper<BobbyQuickActions> quickActionUp2{    BobbyQuickActions::NONE,           DoReset,   {},                           "quickActup2"         };
    ConfigWrapper<BobbyQuickActions> quickActionDown2{  BobbyQuickActions::HANDBREMSE,     DoReset,   {},                           "quickActdown2"       };

    std::array<WirelessDoorsConfig, 5> wireless_door_configs {
        WirelessDoorsConfig { "door_id0", "door_token0" },
        WirelessDoorsConfig { "door_id1", "door_token1" },
        WirelessDoorsConfig { "door_id2", "door_token2" },
        WirelessDoorsConfig { "door_id3", "door_token3" },
        WirelessDoorsConfig { "door_id4", "door_token4" }
    };

    ConfigWrapper<std::string> bluetoothName      {defaultHostname,                        DoReset,   StringMinMaxSize<4, 32>,    "bluetoothName"       };

    ConfigWrapper<bool>        reverseBeep        {false,                                  DoReset,   {},                         "reverseBeep"         };
    ConfigWrapper<uint8_t>     reverseBeepFreq0   {3,                                      DoReset,   {},                         "revBeepFreq0"        };
    ConfigWrapper<uint8_t>     reverseBeepFreq1   {0,                                      DoReset,   {},                         "revBeepFreq1"        };
    ConfigWrapper<int16_t>     reverseBeepDuration0{500,                                   DoReset,   {},                         "revBeepDur0"         };
    ConfigWrapper<int16_t>     reverseBeepDuration1{500,                                   DoReset,   {},                         "revBeepDur1"         };

    ConfigWrapper<std::string> cloudUrl           {std::string{},                          DoReset,   StringOr<StringEmpty, StringValidUrl>, "cloudUrl" };
    ConfigWrapper<std::string> udpCloudHost       {std::string{},                          DoReset,   {},                         "udpCloudHost"        };

    ConfigWrapper<std::string> otaUrl             {std::string{},                          DoReset,   StringOr<StringEmpty, StringValidUrl>, "otaUrl"   };
    ConfigWrapper<std::string> otaUsername        {std::string{},                          DoReset,   {},                         "otaUsername"         };
    ConfigWrapper<std::string> otaServerUrl       {std::string{},                          DoReset,   StringOr<StringEmpty, StringValidUrl>, "otaServerUrl" };
    ConfigWrapper<std::string> otaServerBranch    {std::string{},                          DoReset,   {},                         "otaServerBranch"     };
    std::array<ConfiguredOtaServer, 5> otaServers {
        ConfiguredOtaServer { "otaName0", "otaUrl0" },
        ConfiguredOtaServer { "otaName1", "otaUrl1" },
        ConfiguredOtaServer { "otaName2", "otaUrl2" },
        ConfiguredOtaServer { "otaName3", "otaUrl3" },
        ConfiguredOtaServer { "otaName4", "otaUrl4" }
    };

    ConfigWrapper<bool>        dns_announce_enabled{true,                                  DoReset,   {},                         "dnsAnnounceEnab"     };
    ConfigWrapper<std::string> dns_announce_key   {std::string{},                          DoReset,   {},                         "dnsAnnounceKey"      };
    ConfigWrapper<std::string> webserverPassword  {std::string{},                          DoReset,   {},                         "websPassword"        };

    struct {
        ConfigWrapper<int16_t>  wheelDiameter     {DEFAULT_WHEELDIAMETER,                  DoReset,   {},                         "wheelDiameter"       };
        ConfigWrapper<int16_t> numMagnetPoles     {15,                                     DoReset,   {},                         "numMagnetPoles"      };
        ConfigWrapper<bool>     swapFrontBack     {false,                                  DoReset,   {},                         "swapFrontBack"       };
        ConfigWrapper<bool>   sendFrontCanCmd     {true,                                   DoReset,   {},                         "sendFrontCanCmd"     };
        ConfigWrapper<bool>    sendBackCanCmd     {true,                                   DoReset,   {},                         "sendBackCanCmd"      };
        ConfigWrapper<int16_t> canTransmitTimeout {200,                                    DoReset,   {},                         "canTransmitTime"     };
        ConfigWrapper<int16_t> canReceiveTimeout  {0,                                      DoReset,   {},                         "canReceiveTimeo"     };
    } controllerHardware;

    struct {
        ConfigWrapper<int16_t> gametrakXMin       {0,                                      DoReset,   {},                         "gametrakXMin"        };
        ConfigWrapper<int16_t> gametrakXMax       {4095,                                   DoReset,   {},                         "gametrakXMax"        };
        ConfigWrapper<int16_t> gametrakYMin       {0,                                      DoReset,   {},                         "gametrakYMin"        };
        ConfigWrapper<int16_t> gametrakYMax       {4095,                                   DoReset,   {},                         "gametrakYMax"        };
        ConfigWrapper<int16_t> gametrakDistMin    {0,                                      DoReset,   {},                         "gametrakDistMin"     };
        ConfigWrapper<int16_t> gametrakDistMax    {4095,                                   DoReset,   {},                         "gametrakDistMax"     };
        struct {
            ConfigWrapper<int16_t> statsUpdateRate{50,                                     DoReset,   {},                         "statsUpdateRate"     };
            ConfigWrapper<int16_t> cloudCollectRate{100,                                   DoReset,   {},                         "cloudCollectRat"     };
            ConfigWrapper<int16_t> cloudSendRate  {1,                                      DoReset,   {},                         "cloudSendRate"       };
            ConfigWrapper<int16_t> udpSendRateMs  {65,                                     DoReset,   {},                         "udpSendRate"         };
        } timersSettings;
    } boardcomputerHardware;

    struct {
        ConfigWrapper<bool> cloudEnabled          {false,                                  DoReset,   {},                         "cloudEnabled"        };
        ConfigWrapper<int16_t> cloudTransmitTimeout{10,                                    DoReset,   {},                         "clodTransmTmout"     };
    } cloudSettings;

    struct {
        ConfigWrapper<uint32_t> udpUid            {0,                                      DoReset,   {},                         "cloudUDPUid"         };
        ConfigWrapper<bool> udpCloudEnabled       {false,                                  DoReset,   {},                         "enUdpCloud"          };
        ConfigWrapper<bool> enableCloudDebug      {false,                                  DoReset,   {},                         "debugCloud"          };
        ConfigWrapper<bool> udpUseStdString       {false,                                  DoReset,   {},                         "udpusestdstr"        };
    } udpCloudSettings;

    struct {
        ConfigWrapper<bool> enableLedAnimation    {true,                                   DoReset,   {},                         "enableLedAnimat"     };
        ConfigWrapper<bool> enableBrakeLights     {true,                                   DoReset,   {},                         "enableBrakeLigh"     };
        ConfigWrapper<int16_t> ledsCount          {288,                                    DoReset,   {},                         "ledsCount"           };
        ConfigWrapper<int16_t> centerOffset       {1,                                      DoReset,   {},                         "centerOffset"        };
        ConfigWrapper<int16_t> smallOffset        {4,                                      DoReset,   {},                         "smallOffset"         };
        ConfigWrapper<int16_t> bigOffset          {10,                                     DoReset,   {},                         "bigOffset"           };
        ConfigWrapper<bool> enableBeepWhenBlink   {true,                                   DoReset,   {},                         "beepwhenblink"       };
        ConfigWrapper<LedstripAnimation> animationType{LedstripAnimation::DefaultRainbow,  DoReset,   {},                         "animationType"       };
        ConfigWrapper<bool> enableFullBlink       {true,                                   DoReset,   {},                         "fullblink"           };
        ConfigWrapper<bool> enableStVO            {true,                                   DoReset,   {},                         "ledstvo"             };
        ConfigWrapper<int16_t> stvoFrontOffset    {0,                                      DoReset,   {},                         "ledstvofoff"         };
        ConfigWrapper<int16_t> stvoFrontLength    {10,                                     DoReset,   {},                         "ledstvoflen"         };
        ConfigWrapper<bool> stvoFrontEnable       {false,                                  DoReset,   {},                         "ledstvoen"           };
        ConfigWrapper<int16_t> animationMultiplier{10,                                     DoReset,   {},                         "ledAnimMul"          };
        ConfigWrapper<uint8_t> brightness         {255,                                    DoReset,   {},                         "ledbrightness"       };
        ConfigWrapper<bool> enableAnimBlink       {false,                                  DoReset,   {},                         "enAnimBlink"         };
        ConfigWrapper<OtaAnimationModes> otaMode  {OtaAnimationModes::GreenProgressBar,    DoReset,   {},                         "ledOtaAnim"          };
        ConfigWrapper<uint32_t>     maxMilliamps  {3000,                                   DoReset,   {},                         "ledMaxMilliamps"     };
        ConfigWrapper<bool> enableVisualizeBlink  {false,                                  DoReset,   {},                         "enVisualBlink"       };
    } ledstrip;

    struct {
        ConfigWrapper<uint8_t> cellsSeries        {12,                                     DoReset,   {},                         "batteryCS"           };
        ConfigWrapper<uint8_t> cellsParallel      {10,                                     DoReset,   {},                         "batteryCP"           };
        ConfigWrapper<uint8_t> cellType           {0,                                      DoReset,   {},                         "batteryType"         };
        ConfigWrapper<uint16_t> watthoursPerKilometer{25,                                  DoReset,   {},                         "whkm"                };
        ConfigWrapper<int16_t> front30VoltCalibration{3000,                                DoReset,   {},                         "batF30VCal"          };
        ConfigWrapper<int16_t> back30VoltCalibration {3000,                                DoReset,   {},                         "batB30VCal"          };
        ConfigWrapper<int16_t> front50VoltCalibration{5000,                                DoReset,   {},                         "batF50VCal"          };
        ConfigWrapper<int16_t> back50VoltCalibration {5000,                                DoReset,   {},                         "batB50VCal"          };
        ConfigWrapper<bool> applyCalibration      {true,                                   DoReset,   {},                         "applyBatCal"         };
    } battery;

    struct {
        ConfigWrapper<bool> allowPresetSwitch     {true,                                   DoReset,   {},                         "lockAlwPresetSw"     };
        ConfigWrapper<bool> keepLockedAfterReboot {false,                                  DoReset,   {},                         "keepLocked"          };
        ConfigWrapper<bool> locked                {false,                                  DoReset,   {},                         "currentlyLocked"     };
        std::array<ConfigWrapper<int8_t>, 4> pin {
            ConfigWrapper<int8_t>                 {1,                                      DoReset,   MinMaxValue<int8_t, 0, 9>,  "lockscreenPin0"      },
            ConfigWrapper<int8_t>                 {2,                                      DoReset,   MinMaxValue<int8_t, 0, 9>,  "lockscreenPin1"      },
            ConfigWrapper<int8_t>                 {3,                                      DoReset,   MinMaxValue<int8_t, 0, 9>,  "lockscreenPin2"      },
            ConfigWrapper<int8_t>                 {4,                                      DoReset,   MinMaxValue<int8_t, 0, 9>,  "lockscreenPin3"      },
        };
    } lockscreen;

    struct {
        ConfigWrapper<uint32_t> totalCentimeters  {0,                                      DoReset,   {},                         "totalCentimeter"     };
    } savedStatistics;

    struct {
        ConfigWrapper<HandbremseMode> mode        {HandbremseMode::MOSFETS_OFF,            DoReset,   {},                         "handBremsM"          };
        ConfigWrapper<uint16_t> triggerTimeout    {10,                                     DoReset,   {},                         "handBremsT"          };
        ConfigWrapper<bool> automatic             {false,                                  DoReset,   {},                         "handBremsA"          };
        ConfigWrapper<bool> enable                {false,                                  DoReset,   {},                         "handBremsE"          };
        ConfigWrapper<bool> visualize             {false,                                  DoReset,   {},                         "handBremsV"          };
    } handbremse;

    struct {
        ConfigWrapper<bool> syncTime              {false,                                  DoReset,   {},                         "espnowSyncT"         };
        ConfigWrapper<bool> syncTimeWithOthers    {false,                                  DoReset,   {},                         "espnowSyncTWO"       };
        ConfigWrapper<bool> syncBlink             {false,                                  DoReset,   {},                         "espnowSyncBl"        };
    } espnow;

    struct {
        ConfigWrapper<bool> bleEnabled            {true,                                   DoReset,   {},                         "bleEnabled"          };
    } bleSettings;

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
    \
    x(timeServerEnabled) \
    x(timeServer) \
    x(timeSyncMode) \
    x(timeSyncInterval) \
    x(timezoneOffset) \
    x(timeDst) \
    \
    x(canResetOnError) \
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
    \
    x(quickActionLeft2) \
    x(quickActionRight2) \
    x(quickActionUp2) \
    x(quickActionDown2) \
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
    x(udpCloudHost) \
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
    x(cloudSettings.cloudEnabled) \
    x(cloudSettings.cloudTransmitTimeout) \
    \
    x(udpCloudSettings.udpUid) \
    x(udpCloudSettings.udpCloudEnabled) \
    x(udpCloudSettings.enableCloudDebug) \
    x(udpCloudSettings.udpUseStdString) \
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
    x(ledstrip.enableVisualizeBlink) \
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
    x(lockscreen.pin[3]) \
    \
    x(savedStatistics.totalCentimeters) \
    \
    x(handbremse.mode) \
    x(handbremse.triggerTimeout) \
    x(handbremse.automatic) \
    x(handbremse.enable) \
    x(handbremse.visualize) \
    \
    x(espnow.syncTime) \
    x(espnow.syncTimeWithOthers) \
    x(espnow.syncBlink)
    //x(bleSettings.bleEnabled)

    template<typename T>
    void callForEveryConfig(T &&callback)
    {
#define HELPER(x) callback(x);
        NEW_SETTINGS(HELPER)
#undef HELPER
        callback(bleSettings.bleEnabled);
    }

    auto getAllConfigParams()
    {
        return cpputils::make_array(
#define HELPER(x) std::ref<ConfigWrapperInterface>(x),
            NEW_SETTINGS(HELPER)
#undef HELPER
            std::ref<ConfigWrapperInterface>(bleSettings.bleEnabled)
        );
    }
};

extern ConfigManager<ConfigContainer> configs;
