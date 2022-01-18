#include "selectbuildservermenu.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <actions/switchscreenaction.h>
#include <fmt/core.h>
#include <icons/back.h>
#include <actions/dummyaction.h>

// local includes
#include "buildserver.h"
#include "displays/menus/otamenu.h"
#include "utils.h"
#include "newsettings.h"

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
        configs.write_config(configs.otaServerUrl, m_buildserver_url); // mir egal ob succeeded
        if (m_buildserver_url.substr(m_buildserver_url.length() - 4) == ".bin")
            configs.write_config(configs.otaUrl, m_buildserver_url); // mir egal ob es succeeded

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
        std::string url = otaServer.url.value;
        std::string name = (otaServer.name.value.empty()) ? url : otaServer.name.value;

        if (!name.empty())
        {
            auto &menuitem = constructMenuItem<BuildserverMenuItem>();
            menuitem.setBuildserverName(name);
            menuitem.setBuildserverUrl(url);
        }
    }

    if (menuItemCount() < 1)
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NOBUILDSERVERCONFIGURED>, DefaultFont, StaticColor<TFT_RED>, DummyAction>>();
    }

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<OtaMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string SelectBuildServerMenu::text() const
{
    return TEXT_SELECTBUILDSERVERMENU;
}

void SelectBuildServerMenu::back()
{
    espgui::switchScreen<OtaMenu>();
}
