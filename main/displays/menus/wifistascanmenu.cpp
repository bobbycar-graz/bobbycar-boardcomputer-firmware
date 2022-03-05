#include "wifistascanmenu.h"

// system includes
#include <algorithm>

// 3rdparty lib includes
#include <menuitem.h>
#include <actions/pushscreenaction.h>
#include <actions/popscreenaction.h>
#include <icons/back.h>
#include <screenmanager.h>
#include <fmt/format.h>
#include <espwifistack.h>
#include <espwifiutils.h>
#include <richtextrenderer.h>
#include <strutils.h>

// local includes
#include "actions/wifistascanaction.h"
#include "actions/wifistascanclearaction.h"
#include "wifiguiutils.h"
#include "wifistascanentrymenu.h"
#include "newsettings.h"

using namespace espgui;

namespace {
constexpr char TEXT_STARTSCAN[] = "Start scan";
constexpr char TEXT_CLEARRESULTS[] = "Clear results";
constexpr char TEXT_BACK[] = "Back";

class WifiStaScanMenuItem : public MenuItem
{
public:
    WifiStaScanMenuItem(const wifi_ap_record_t &info) : m_info{info} {}

    void setInfo(const wifi_ap_record_t &info) { m_info = info; }

    void triggered() override;
    std::string text() const override;

private:
    wifi_ap_record_t m_info;
};

constexpr const size_t extraItemsAtBeginning = 2; // Scan and clear
} // namespace

WifiStaScanMenu::WifiStaScanMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STARTSCAN>, WifiStaScanAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLEARRESULTS>, WifiStaScanClearAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&icons::back>>>();
}

std::string WifiStaScanMenu::text() const
{
    const auto scanStatus = wifi_stack::get_scan_status();
    auto text = fmt::format("Scan {}{}",
        [&](){
            switch (scanStatus)
            {
            case wifi_stack::WiFiScanStatus::Scanning: return "&4";
            case wifi_stack::WiFiScanStatus::Finished: return "&2";
            case wifi_stack::WiFiScanStatus::Failed: return "&1";
            default: return "";
            }
        }(),
        wifi_stack::toString(scanStatus));

    //if (scanStatus != wifi_stack::WiFiScanStatus::Scanning)
    if (const auto &result = wifi_stack::get_scan_result())
        text += fmt::format("&c ({} found)", result->entries.size());

    return text;
}

void WifiStaScanMenu::back()
{
    popScreen();
}

void WifiStaScanMenu::start()
{
    Base::start();

    m_wasScanning = true;
}

void WifiStaScanMenu::update()
{
    const auto scanStatus = wifi_stack::get_scan_status();

    if (scanStatus == wifi_stack::WiFiScanStatus::Scanning && !m_wasScanning)
    {
        m_wasScanning = true;
    }
    else if (scanStatus != wifi_stack::WiFiScanStatus::Scanning && m_wasScanning)
    {
        m_wasScanning = false;

        auto backButton = takeLastMenuItem();

        const auto &result = wifi_stack::get_scan_result();

        for (std::size_t i = 0; i < (result ? result->entries.size() : 0); i++)
        {
            if (menuItemCount() - extraItemsAtBeginning <= i)
                constructMenuItem<WifiStaScanMenuItem>(result->entries[i]);
            else
                ((WifiStaScanMenuItem*)(&getMenuItem(i + extraItemsAtBeginning)))->setInfo(result->entries[i]);
        }

        while (menuItemCount() - extraItemsAtBeginning > (result ? result->entries.size() : 0))
            takeLastMenuItem();

        emplaceMenuItem(std::move(backButton));
    }

    Base::update();
}

namespace {

void WifiStaScanMenuItem::triggered()
{
    pushScreen<WifiStaScanEntryMenu>(m_info);
}

std::string WifiStaScanMenuItem::text() const
{
    const std::string_view ssid{reinterpret_cast<const char*>(m_info.ssid)};

    bool connected{};
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
        if (const auto &result = wifi_stack::get_sta_ap_info(); result)
            if (wifi_stack::mac_t{result->bssid} == wifi_stack::mac_t{m_info.bssid})
                connected = true;

    const bool configured = std::any_of(std::begin(configs.wifi_configs), std::end(configs.wifi_configs),
                                        [&ssid](const WiFiConfig &config){ return cpputils::stringEqualsIgnoreCase(config.ssid.value, ssid); });

    return fmt::format("&s{}{} {}&f{}",
                       rssiToColor(m_info.rssi),
                       m_info.rssi,
                       connected ? "&2" : (configured ? "&4" : "&c"),
                       richTextEscape(ssid));
}

} // namespace
