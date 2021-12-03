#include "wifiscanmenu.h"

// system includes
#include <optional>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <espchrono.h>
#include <espwifistack.h>
#include <fmt/core.h>

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "actions/multiaction.h"
#include "actions/switchscreenaction.h"
#include "actions/dummyaction.h"
#include "icons/back.h"
#include "texts.h"
#include "wifi_bobbycar.h"

using namespace std::chrono_literals;
using namespace espgui;

WifiScanMenu::WifiScanMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<StationWifiSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string WifiScanMenu::text() const
{
    const auto scanStatus = wifi_stack::get_scan_status();
    auto text = wifi_stack::toString(scanStatus);

    if (scanStatus != wifi_stack::WiFiScanStatus::Scanning)
        if (const auto &result = wifi_stack::get_scan_result())
            text += fmt::format(" ({} found)", result->entries.size());

    return text;
}

void WifiScanMenu::start()
{
    Base::start();

    m_lastScanComplete = {};

    if (wifi_stack::get_scan_status() != wifi_stack::WiFiScanStatus::Scanning)
        if (const auto result = wifi_scan(); result != ESP_OK)
            ESP_LOGE("BOBBY", "wifi_scan() failed with %s", esp_err_to_name(result));
}

void WifiScanMenu::update()
{
    if (wifi_stack::get_scan_status() == wifi_stack::WiFiScanStatus::Scanning)
    {
        // TODO
    }
    else
    {
        const auto now = espchrono::millis_clock::now();

        if (!m_lastScanComplete)
        {
            const auto &result = wifi_stack::get_scan_result();

            auto backButton = takeLastMenuItem();

            for (std::size_t i = 0; i < (result ? result->entries.size() : 0); i++)
            {
                std::string ssid{reinterpret_cast<const char*>(result->entries[i].ssid)};
                if (menuItemCount() <= i)
                {
                    if (m_reusableItems.empty())
                    {
                        auto &item = constructMenuItem<makeComponent<MenuItem, ChangeableText, DummyAction>>();
                        item.setTitle(std::move(ssid));
                    }
                    else
                    {
                        std::unique_ptr<makeComponent<MenuItem, ChangeableText, DummyAction>> ptr = std::move(m_reusableItems.back());
                        m_reusableItems.pop_back();
                        ptr->setTitle(std::move(ssid));
                        emplaceMenuItem(std::move(ptr));
                    }
                }
                else
                {
                    auto &item = *(makeComponent<MenuItem, ChangeableText, DummyAction>*)(&getMenuItem(i));
                    item.setTitle(std::move(ssid));
                }
            }

            while (menuItemCount() > (result ? result->entries.size() : 0))
                m_reusableItems.emplace_back((makeComponent<MenuItem, ChangeableText, DummyAction>*)takeLastMenuItem().release());

            emplaceMenuItem(std::move(backButton));

            m_lastScanComplete = now;
        }
        else if (espchrono::ago(*m_lastScanComplete) >= 10s)
        {
            m_lastScanComplete = {};

            if (const auto result = wifi_scan(); result != ESP_OK)
                ESP_LOGE("BOBBY", "wifi_scan() failed with %s", esp_err_to_name(result));
        }
    }

    Base::update();
}

void WifiScanMenu::stop()
{
    wifi_stack::delete_scan_result();
}

void WifiScanMenu::back()
{
    switchScreen<StationWifiSettingsMenu>();
}
