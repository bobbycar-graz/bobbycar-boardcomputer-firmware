#include "wifitexthelpers.h"

// 3rdparty lib includes
#include <esp_netif.h>
#include <espwifistack.h>
#include <fmt/core.h>
#include <lwip/dns.h>

std::string WifiStatusText::text() const
{
    return fmt::format("Status: {}", wifi_stack::toString(wifi_stack::get_sta_status()));
}

std::string WifiScanStatusText::text() const
{
    return fmt::format("ScanStatus: {}", wifi_stack::toString(wifi_stack::get_scan_status()));
}

std::string WifiHostnameText::text() const
{
    std::string text = "Hostname: ";
    const char *hostname {};
    if (const auto result = esp_netif_get_hostname(wifi_stack::esp_netifs[ESP_IF_WIFI_STA], &hostname); result == ESP_OK)
        if (hostname)
            text += hostname;
    return text;
}

std::string WifiMacText::text() const
{
    std::string text = "MAC: ";
    if (const auto result = wifi_stack::get_base_mac_addr())
        text += wifi_stack::toString(*result);
    return text;
}

std::string WifiSsidText::text() const
{
    std::string text = "SSID: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const auto &result = wifi_stack::get_sta_ap_info(); result)
            text += std::string_view { reinterpret_cast<const char *>(result->ssid) };
    return text;
}

std::string WifiBssidText::text() const
{
    std::string text = "BSSID: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const auto &result = wifi_stack::get_sta_ap_info(); result)
            text += wifi_stack::toString(wifi_stack::mac_t { result->bssid });
    return text;
}

std::string WifiRssiText::text() const
{
    std::string text = "RSSI: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const auto &result = wifi_stack::get_sta_ap_info(); result)
            text += fmt::format("{}dB", result->rssi);
    return text;
}

std::string WifiEncryptionTypeText::text() const
{
    std::string text = "encryptionType: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const auto &result = wifi_stack::get_sta_ap_info(); result)
            text += wifi_stack::toString(result->authmode);
    return text;
}

std::string WifiPairwiseCipherText::text() const
{
    std::string text = "pairwiseCipher: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const auto &result = wifi_stack::get_sta_ap_info(); result)
            text += wifi_stack::toString(result->pairwise_cipher);
    return text;
}

std::string WifiGroupCipherText::text() const
{
    std::string text = "groupCipher: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const auto &result = wifi_stack::get_sta_ap_info(); result)
            text += wifi_stack::toString(result->group_cipher);
    return text;
}

std::string WifiIpText::text() const
{
    std::string text = "ip: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const auto result = wifi_stack::get_ip_info(TCPIP_ADAPTER_IF_STA); result)
            text += wifi_stack::toString(result->ip);
    return text;
}

std::string WifiNetmaskText::text() const
{
    std::string text = "netmask: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const auto result = wifi_stack::get_ip_info(TCPIP_ADAPTER_IF_STA); result)
            text += wifi_stack::toString(result->netmask);
    return text;
}

std::string WifiGatewayText::text() const
{
    std::string text = "gateway: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const auto result = wifi_stack::get_ip_info(TCPIP_ADAPTER_IF_STA); result)
            text += wifi_stack::toString(result->gw);
    return text;
}

std::string WifiIpv6LinklocalText::text() const
{
    std::string text = "ipv6: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
    {
        esp_ip6_addr_t addr;
        if (const auto result = esp_netif_get_ip6_linklocal(wifi_stack::esp_netifs[ESP_IF_WIFI_STA], &addr); result == ESP_OK)
            text += wifi_stack::toString(addr);
    }
    return text;
}

std::string WifiIpv6GlobalText::text() const
{
    std::string text = "ipv6: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
    {
        esp_ip6_addr_t addr;
        if (const auto result = esp_netif_get_ip6_global(wifi_stack::esp_netifs[ESP_IF_WIFI_STA], &addr); result == ESP_OK)
            text += wifi_stack::toString(addr);
    }
    return text;
}

std::string WifiDns0Text::text() const
{
    std::string text = "dns0: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const ip_addr_t *dns_ip = dns_getserver(0))
            text += wifi_stack::toString(*dns_ip);
    return text;
}

std::string WifiDns1Text::text() const
{
    std::string text = "dns1: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const ip_addr_t *dns_ip = dns_getserver(1))
            text += wifi_stack::toString(*dns_ip);
    return text;
}

std::string WifiDns2Text::text() const
{
    std::string text = "dns2: ";
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const ip_addr_t *dns_ip = dns_getserver(2))
            text += wifi_stack::toString(*dns_ip);
    return text;
}
