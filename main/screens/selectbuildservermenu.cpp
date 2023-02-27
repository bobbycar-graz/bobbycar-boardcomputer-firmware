#include "selectbuildservermenu.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <fmt/core.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "buildserver.h"
#include "icons/back.h"
#include "newsettings.h"
#include "utils.h"

namespace bobby {

using namespace buildserver::SelectBuild;

namespace {
constexpr char TEXT_SELECTBUILDSERVERMENU[] = "Select Buildserver";
constexpr char TEXT_NOBUILDSERVERCONFIGURED[] = "Not configured";
constexpr char TEXT_BACK[] = "Back";

class BuildserverMenuItem : public espgui::MenuItem
{
public:
    std::string text() const override { return m_buildserver_name; }
    void setBuildserverName(std::string &&buildserver_name) { m_buildserver_name = std::move(buildserver_name); }
    void setBuildserverName(const std::string &buildserver_name) { m_buildserver_name = buildserver_name; }
    void setBuildserverUrl(std::string &&buildserver_url) { m_buildserver_url = std::move(buildserver_url); }
    void setBuildserverUrl(const std::string &buildserver_url) { m_buildserver_url = buildserver_url; }

    void triggered() override
    {
        ESP_LOGE("BOBBY", "Saving buildserver url: %s", m_buildserver_url.c_str());
        configs.write_config(configs.otaServerUrl, m_buildserver_url);

        url_for_latest.clear();
        url_for_hashes.clear();
        availableVersions = {};
    }

private:
    std::string m_buildserver_url;
    std::string m_buildserver_name;
};
} // namespace

SelectBuildServerMenu::SelectBuildServerMenu()
{
    using namespace espgui;

    for (const auto &otaServer : configs.otaServers)
    {
        std::string url = otaServer.url.value();
        std::string name = (otaServer.name.value().empty()) ? url : otaServer.name.value();

        if (!name.empty())
        {
            auto &menuitem = constructMenuItem<BuildserverMenuItem>();
            menuitem.setBuildserverName(name);
            menuitem.setBuildserverUrl(url);
        }
    }

    if (menuItemCount() < 1)
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NOBUILDSERVERCONFIGURED>, DefaultFont, StaticColor<espgui::TFT_RED>, DummyAction>>();
    }

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string SelectBuildServerMenu::text() const
{
    return TEXT_SELECTBUILDSERVERMENU;
}

void SelectBuildServerMenu::back()
{
    espgui::popScreen();
}
} // namespace bobby
