#pragma once

#include <espwifistack.h>
#include <TFT_eSPI.h>
#include "esp_log.h"
#include "fmt/core.h"

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "actions/dummyaction.h"
#include "icons/back.h"
#include "icons/update.h"
#include "texts.h"

#include "buildserver.h"
#include "globals.h"

#define MESSAGE(text) constructMenuItem<makeComponent<MenuItem, StaticText<text>, DefaultFont, StaticColor<TFT_RED>, DummyAction>>()

// forward declares
namespace {
class OtaMenu;
} // namespace

using namespace espgui;

namespace {

// ToDo: if (request_failed) => MESSAGE("An error occurred")

template<int item_color>
class VersionMenuItem : public MenuItem
{
public:
    std::string text() const override { return m_hash; }
    void setHash(std::string &&hash) { m_hash = std::move(hash); }
    void setHash(const std::string &hash) { m_hash = hash; }
    void setUrl(std::string &&url) { m_url = std::move(url); }
    void setUrl(const std::string &url) { m_url = url; }

    void triggered() override
    {
        stringSettings.otaUrl = m_url;
        saveSettings();
    }

    int color() const override
    {
        return item_color;
    }
private:
    std::string m_url;
    std::string m_hash;
};

class SelectBuildMenu :
    public MenuDisplay,
    public StaticText<TEXT_SELECTBUILD>,
    public BackActionInterface<SwitchScreenAction<OtaMenu>>
{
    using Base = MenuDisplay;
public:
    void update() override;
    void buildMenuFromJson();
    void buildMenuRequestError(std::string error);
    SelectBuildMenu()
    {
        if (count_available_buildserver() < 1)
        {
            MESSAGE(TEXT_OTA_NOBUILDSERVERAVAILABLE);
            constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                SwitchScreenAction<OtaMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
        }
        else if (stringSettings.otaServerUrl.empty())
        {
            MESSAGE(TEXT_OTA_NOBUILDSERVERSELECTED);
            constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                SwitchScreenAction<OtaMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
        }
        else
        {
            const auto staStatus = wifi_stack::get_sta_status();
            if (staStatus != wifi_stack::WiFiStaStatus::CONNECTED)
            {
                MESSAGE(TEXT_OTA_NOCONNECTION);
                constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                SwitchScreenAction<OtaMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
            }
            else
            {
                std::string serverUrl = stringSettings.otaServerUrl;
                if (serverUrl.substr(serverUrl.length() - 4) == ".bin")
                {
                    auto &menuitem = constructMenuItem<VersionMenuItem<TFT_WHITE>>();
                    std::size_t last_slash_index = serverUrl.find_last_of("/");
                    auto filename = serverUrl.substr(last_slash_index+1);
                    auto hash = filename.substr(0, filename.length() - 4);
                    menuitem.setHash(hash);
                    menuitem.setUrl(fix_url(serverUrl));
                    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                SwitchScreenAction<OtaMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
                }
                else
                {
                    setup_request();
                    start_descriptor_request(serverUrl);
                }
            }
        }
    }
};

void SelectBuildMenu::update()
{
    if(get_request_running())
    {
        check_descriptor_request();
        if (request_failed)
        {
            this->buildMenuRequestError(fmt::format("Error: {}", request_failed));
            request_failed = false;
        }
    }

    if (parsing_finished)
    {
        parsing_finished = false;
        if (!availableVersions.empty())
        {
            this->buildMenuFromJson();
        }
    }
    Base::update();
}

void SelectBuildMenu::buildMenuFromJson()
{
    auto &latest = constructMenuItem<VersionMenuItem<TFT_GREEN>>();
    latest.setHash("latest");
    latest.setUrl(url_for_latest);

    for (const std::string &hash : availableVersions)
    {
        auto &menuitem = constructMenuItem<VersionMenuItem<TFT_WHITE>>();
        menuitem.setHash(hash);
        menuitem.setUrl(fmt::format(url_for_hashes, hash));
    }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                SwitchScreenAction<OtaMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void SelectBuildMenu::buildMenuRequestError(std::string error)
{
    auto &item = constructMenuItem<makeComponent<MenuItem, ChangeableText, DefaultFont, StaticColor<TFT_RED>, DummyAction>>();
    item.setTitle(error);
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                SwitchScreenAction<OtaMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}
} // namespace
