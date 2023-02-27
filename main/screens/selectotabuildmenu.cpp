#include "selectotabuildmenu.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <espwifistack.h>
#include <fmt/core.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "buildserver.h"
#include "globals.h"
#include "guihelpers/bobbyerrorhandler.h"
#include "icons/back.h"
#include "newsettings.h"
#include "screens/otamenu.h"
#include "utils.h"

namespace bobby {

#define MESSAGE(text) constructMenuItem<makeComponent<MenuItem, StaticText<text>, DefaultFont, StaticColor<espgui::TFT_RED>, DummyAction>>()

using namespace espgui;
using namespace buildserver::SelectBuild;

namespace {
constexpr char TEXT_UPDATENOW[] = "Update now";
constexpr char TEXT_SELECTBUILD[] = "Select build";
constexpr char TEXT_OTA_NOBUILDSERVERAVAILABLE[] = "E:No server saved.";
constexpr char TEXT_OTA_NOBUILDSERVERSELECTED[] = "E:No server selected.";
constexpr char TEXT_OTA_NOCONNECTION[] = "E:No internet.";
constexpr char TEXT_OTA_WAITFORRESPONSE[] = "Wait for response...";
constexpr char TEXT_BACK[] = "Back";

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
        configs.write_config(configs.otaUrl, m_url); // mir egal ob succeeded
    }

    int color() const override
    {
        return item_color;
    }
private:
    std::string m_url;
    std::string m_hash;
};
}

SelectBuildMenu::SelectBuildMenu()
{
    if (buildserver::count_available_buildserver() < 1)
    {
        MESSAGE(TEXT_OTA_NOBUILDSERVERAVAILABLE);
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
    }
    else if (configs.otaServerUrl.value().empty())
    {
        MESSAGE(TEXT_OTA_NOBUILDSERVERSELECTED);
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
    }
    else
    {
        const auto staStatus = wifi_stack::get_sta_status();
        if (staStatus != wifi_stack::WiFiStaStatus::CONNECTED)
        {
            MESSAGE(TEXT_OTA_NOCONNECTION);
            constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
        }
        else
        {
            std::string serverUrl = configs.otaServerUrl.value();
            if (serverUrl.substr(serverUrl.length() - 4) == ".bin")
            {
                auto &menuitem = constructMenuItem<VersionMenuItem<espgui::TFT_WHITE>>();
                std::size_t last_slash_index = serverUrl.find_last_of("/");
                auto filename = serverUrl.substr(last_slash_index+1);
                auto hash = filename.substr(0, filename.length() - 4);
                menuitem.setHash(hash);
                menuitem.setUrl(serverUrl);
                constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
            }
            else
            {
                setup_request();
                start_descriptor_request(serverUrl);
            }
        }
    }
}

std::string SelectBuildMenu::text() const
{
    return TEXT_SELECTBUILD;
}

void SelectBuildMenu::update()
{
    if(get_request_running())
    {
        check_descriptor_request();
        if (!request_failed.empty())
        {
            BobbyErrorHandler{}.errorOccurred(fmt::format("Error: {}", request_failed));
            request_failed = {};
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
    auto &latest = constructMenuItem<VersionMenuItem<espgui::TFT_GREEN>>();
    latest.setHash("latest");
    latest.setUrl(url_for_latest);

    for (const std::string &hash : availableVersions)
    {
        auto &menuitem = constructMenuItem<VersionMenuItem<espgui::TFT_WHITE>>();
        menuitem.setHash(hash);
        menuitem.setUrl(fmt::format(fmt::runtime(url_for_hashes), hash));
    }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

void SelectBuildMenu::back()
{
    popScreen();
}

} // namespace bobby
