#include "wifiapclientsmenu.h"

// esp-idf includes
#include <apps/dhcpserver/dhcpserver.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <../lwip/esp_netif_lwip_internal.h>

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <espwifistack.h>
#include <espwifiutils.h>
#include <fmt/format.h>
#include <menuitem.h>
#include <screenmanager.h>

// local includes
#include "icons/back.h"
#include "wifiapsettingsmenu.h"
#include "wifiguiutils.h"

namespace bobby {

using namespace std::chrono_literals;
using namespace std::string_literals;
using namespace espgui;

namespace {
constexpr const char * const TAG = "BOBBY";

constexpr char TEXT_BACK[] = "Back";

class WifiApClientMenuItem : public MenuItem
{
public:
    WifiApClientMenuItem(const wifi_sta_info_t &info) : m_info{info} { updateIp(); }

    void setInfo(const wifi_sta_info_t &info) { m_info = info; updateIp(); }

    void triggered() override;
    std::string text() const override;

private:
    void updateIp();

private:
    wifi_sta_info_t m_info;
    ip4_addr_t m_ip;
};
} // namespace

WifiApClientsMenu::WifiApClientsMenu() :
    espgui::ChangeableText{"AP Clients"s}
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

void WifiApClientsMenu::back()
{
    popScreen();
}

void WifiApClientsMenu::start()
{
    Base::start();

    m_lastSync = espchrono::millis_clock::now();
    doSyncItems();
}

void WifiApClientsMenu::update()
{
    const auto now = espchrono::millis_clock::now();
    if (now - m_lastSync >= 100ms)
    {
        m_lastSync = now;
        doSyncItems();
    }

    Base::update();
}

void WifiApClientsMenu::doSyncItems()
{
    wifi_sta_list_t clients;
    if (const auto result = esp_wifi_ap_get_sta_list(&clients); result != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_wifi_ap_get_sta_list() failed with %s", esp_err_to_name(result));
        setTitle(fmt::format("AP Clients (&1{}&c)", esp_err_to_name(result)));
        clients.num = 0;
    }
    else
        setTitle(fmt::format("AP Clients ({})", clients.num));

    auto backButton = takeLastMenuItem();

    for (std::size_t i = 0; i < clients.num; i++)
    {
        if (menuItemCount() <= i)
            constructMenuItem<WifiApClientMenuItem>(clients.sta[i]);
        else
            ((WifiApClientMenuItem*)(&getMenuItem(i)))->setInfo(clients.sta[i]);
    }

    while (menuItemCount() > clients.num)
        takeLastMenuItem();

    emplaceMenuItem(std::move(backButton));
}

namespace {

void WifiApClientMenuItem::triggered()
{
    uint16_t aid{};
    if (const auto result = esp_wifi_ap_get_sta_aid(m_info.mac, &aid); result != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_wifi_ap_get_sta_aid() failed with %s", esp_err_to_name(result));
        return;
    }

    if (!aid)
    {
        ESP_LOGE(TAG, "aid is invalid");
        return;
    }

    if (const auto result = esp_wifi_deauth_sta(aid); result != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_wifi_deauth_sta() failed with %s", esp_err_to_name(result));
        return;
    }
}

std::string WifiApClientMenuItem::text() const
{
    return fmt::format("{}{}&c {}",
                       rssiToColor(m_info.rssi),
                       m_info.rssi,
                       (espchrono::millis_clock::now().time_since_epoch() % 4000ms) < 2000ms ?
                           wifi_stack::toString(wifi_stack::mac_t{m_info.mac}) :
                           wifi_stack::toString(m_ip)
                       );
}

void WifiApClientMenuItem::updateIp()
{
    if (!dhcp_search_ip_on_mac(wifi_stack::esp_netifs[ESP_IF_WIFI_AP]->dhcps, m_info.mac, &m_ip))
    {
        ESP_LOGE(TAG, "dhcp_search_ip_on_mac() failed");
        m_ip = { .addr = {} };
        return;
    }
}

} // namespace
} // namespace bobby
