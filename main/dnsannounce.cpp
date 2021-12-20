// 3rd party
#ifdef FEATURE_DNS_NS
#include <randomutils.h>
#include <esprandom.h>
#include <FastLED.h>

// local
#include "dnsannounce.h"
#include "espwifistack.h"
#include "cpputils.h"
#include "lwip/dns.h"
#include "globals.h"

void init_dns_announce()
{

}

void handle_dns_announce()
{
    const auto staStatus = wifi_stack::get_sta_status();
    const auto randDNSName = cpputils::randomNumber<uint16_t>(espcpputils::esp_random_device{});
    if (staStatus == wifi_stack::WiFiStaStatus::CONNECTED)
    {
        EVERY_N_SECONDS ( 2 )
        {
            // Get IPv4
            if (const auto result = wifi_stack::get_ip_info(TCPIP_ADAPTER_IF_STA); result)
            {
                std::string curIpAddress = wifi_stack::toString(result->ip);
                if (curIpAddress != "0.0.0.0")
                {
                    if (dns_lastIpAddress_v4 != curIpAddress)
                    {
                        dns_lastIpAddress_v4 = curIpAddress;
                        ip_addr_t tmpIpResolved;
                        std::string toLookup = fmt::format("{}__{}.{}.announce.bobbycar.cloud", randDNSName, curIpAddress, OTA_USERNAME);
                        ESP_LOGI("BOBBY", "Trying to look up %s", toLookup.c_str());
                        if (const auto err = dns_gethostbyname(toLookup.c_str(), &tmpIpResolved, NULL, NULL); err != ERR_OK && err != ERR_INPROGRESS)
                        {
                            ESP_LOGW("BOBBY", "There is a error in the matrix (dns ipv4 lookup failed) -> %d", err);
                            dns_lastIpAddress_v4 = "-";
                            dns_lastIpAddress_v6 = "-";
                            dns_lastIpAddress_v6_global = "-";
                        }
                    }
                }
            }
            else
            {
                ESP_LOGW("BOBBY", "get_ip_info() failed with %.*s", result.error().size(), result.error().data());
            }
            esp_ip6_addr_t tmpv6addr;
            if (const auto result = esp_netif_get_ip6_linklocal(wifi_stack::esp_netifs[ESP_IF_WIFI_STA], &tmpv6addr); result == ESP_OK)
            {
                std::string curIpV6Address = wifi_stack::toString(tmpv6addr);
                std::replace(curIpV6Address.begin(), curIpV6Address.end(), ':', '-');
                if (dns_lastIpAddress_v6 != curIpV6Address)
                {
                    dns_lastIpAddress_v6 = curIpV6Address;
                    ip_addr_t tmpIpResolved;
                    std::string toLookup = fmt::format("{}__{}.{}.announce6.bobbycar.cloud", randDNSName, curIpV6Address, OTA_USERNAME);
                    ESP_LOGI("BOBBY", "Trying to look up %s", toLookup.c_str());
                    if (const auto err = dns_gethostbyname(toLookup.c_str(), &tmpIpResolved, NULL, NULL); err != ERR_OK && err != ERR_INPROGRESS)
                    {
                        ESP_LOGW("BOBBY", "There is a error in the matrix (dns ipv6 local lookup failed) -> %d", err);
                        dns_lastIpAddress_v4 = "-";
                        dns_lastIpAddress_v6 = "-";
                        dns_lastIpAddress_v6_global = "-";
                    }
                }
            }

            if (const auto result = esp_netif_get_ip6_global(wifi_stack::esp_netifs[ESP_IF_WIFI_STA], &tmpv6addr); result == ESP_OK)
            {
                std::string curIpV6Address = wifi_stack::toString(tmpv6addr);
                if (dns_lastIpAddress_v6_global != curIpV6Address)
                {
                    dns_lastIpAddress_v6_global = curIpV6Address;
                    std::replace(curIpV6Address.begin(), curIpV6Address.end(), ':', '-');
                    ip_addr_t tmpIpResolved;
                    std::string toLookup = fmt::format("{}global__{}.{}.announce6.bobbycar.cloud", randDNSName, curIpV6Address, OTA_USERNAME);
                    ESP_LOGI("BOBBY", "Trying to look up %s", toLookup.c_str());
                    if (const auto err = dns_gethostbyname(toLookup.c_str(), &tmpIpResolved, NULL, NULL); err != ERR_OK && err != ERR_INPROGRESS)
                    {
                        ESP_LOGW("BOBBY", "There is a error in the matrix (dns ipv6 global lookup failed) -> %d", err);
                        dns_lastIpAddress_v4 = "-";
                        dns_lastIpAddress_v6 = "-";
                        dns_lastIpAddress_v6_global = "-";
                    }
                }
            }
        }

        EVERY_N_SECONDS( 120 ) {
            dns_lastIpAddress_v4 = "-";
            dns_lastIpAddress_v6 = "-";
            dns_lastIpAddress_v6_global = "-";
        }
    }
    else
    {
        dns_lastIpAddress_v4 = "-";
        dns_lastIpAddress_v6 = "-";
        dns_lastIpAddress_v6_global = "-";
    }
}
#endif
