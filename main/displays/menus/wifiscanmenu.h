#pragma once

#include <espchrono.h>

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "actions/multiaction.h"
#include "actions/switchscreenaction.h"
#include "actions/dummyaction.h"
#include "icons/back.h"
#include "texts.h"

// forward declares
namespace {
class WifiSettingsMenu;
} // namespace

namespace {
class WifiScanMenu : public MenuDisplay, public BackActionInterface<SwitchScreenAction<WifiSettingsMenu>>
{
    using Base = MenuDisplay;

public:
    WifiScanMenu();

    std::string text() const override;

    void start() override;
    void update() override;
    void stop() override;

private:
    espchrono::millis_clock::time_point m_lastScanComplete;

    std::vector<std::unique_ptr<makeComponent<MenuItem, ChangeableText, DummyAction>>> m_reusableItems;
};

WifiScanMenu::WifiScanMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<WifiSettingsMenu>, StaticMenuItemIcon<&icons::back>>>();
}

std::string WifiScanMenu::text() const
{
    auto text = std::to_string(menuItemCount()-1) + " found";
    //switch (WiFi.scanComplete())
    //{
    //case WIFI_SCAN_RUNNING: text += " (scanning)"; break;
    //case WIFI_SCAN_FAILED: text += " (error)"; break;
    //}
    text += " (not implemented)";
    return text;
}

void WifiScanMenu::start()
{
    Base::start();

    m_lastScanComplete = {};

    //WiFi.scanNetworks(true);
}

void WifiScanMenu::update()
{
    //const auto n = WiFi.scanComplete();
    //if (n >= 0)
    //{
    //    const auto now = espchrono::millis_clock::now();
    //    if (!m_lastScanComplete)
    //    {
    //        auto backButton = takeLastMenuItem();
    //
    //        for (std::size_t i = 0; i < n; i++)
    //        {
    //            const auto ssid = to_string(WiFi.SSID(i));
    //            if (menuItemCount() <= i)
    //            {
    //                if (m_reusableItems.empty())
    //                {
    //                    auto &item = constructMenuItem<makeComponent<MenuItem, ChangeableText, DummyAction>>();
    //                    item.setTitle(ssid);
    //                }
    //                else
    //                {
    //                    std::unique_ptr<makeComponent<MenuItem, ChangeableText, DummyAction>> ptr = std::move(m_reusableItems.back());
    //                    m_reusableItems.pop_back();
    //                    ptr->setTitle(ssid);
    //                    emplaceMenuItem(std::move(ptr));
    //                }
    //            }
    //            else
    //            {
    //                auto &item = *(makeComponent<MenuItem, ChangeableText, DummyAction>*)(&getMenuItem(i));
    //                item.setTitle(ssid);
    //            }
    //        }
    //
    //        while (menuItemCount() > n)
    //            m_reusableItems.emplace_back((makeComponent<MenuItem, ChangeableText, DummyAction>*)takeLastMenuItem().release());
    //
    //        emplaceMenuItem(std::move(backButton));
    //
    //        m_lastScanComplete = now;
    //    }
    //    else if (now - m_lastScanComplete >= 2000)
    //    {
    //        m_lastScanComplete = 0;
    //        WiFi.scanNetworks(true);
    //    }
    //}

    Base::update();
}

void WifiScanMenu::stop()
{
    //WiFi.scanDelete();
}
} // namespace
