#pragma once

// 3rdparty lib includes
#include <TFT_eSPI.h>

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "actions/dummyaction.h"
#include "icons/back.h"
#include "texts.h"
#include "globals.h"
#include "buildserver.h"

// Debugging
#include "esp_log.h"
#include "fmt/core.h"


// forward declares
namespace {
class SettingsMenu;
} // namespace

using namespace espgui;

namespace {

class BuildserverMenuItem : public MenuItem
{

public:

    std::string text() const override { return m_buildserver_name; }
    void setBuildserverName(std::string &&buildserver_name) { m_buildserver_name = std::move(buildserver_name); }
    void setBuildserverName(const std::string &buildserver_name) { m_buildserver_name = buildserver_name; }
    void setBuildserverUrl(std::string &&buildserver_url) { m_buildserver_url = std::move(buildserver_url); }
    void setBuildserverUrl(const std::string &buildserver_url) { m_buildserver_url = buildserver_url; }

    void triggered() override
    {
        stringSettings.otaServerUrl = m_buildserver_url;
        if (m_buildserver_url.substr(m_buildserver_url.length() - 4) == ".bin")
        {
            stringSettings.otaUrl = m_buildserver_url;
        }
        saveSettings();
        redownload = true;
        url_for_latest.clear();
        url_for_hashes.clear();
        availableVersions = {};
    }

private:
    std::string m_buildserver_url;
    std::string m_buildserver_name;
};

class SelectBuildServerMenu :
    public MenuDisplay,
    public StaticText<TEXT_SELECTBUILDSERVERMENU>,
    public BackActionInterface<SwitchScreenAction<SettingsMenu>>
{
public:
    SelectBuildServerMenu() {

        for (const auto &otaServer : stringSettings.otaServers)
        {
            std::string url = otaServer.url;
            std::string name = (otaServer.name.empty()) ? url : otaServer.name;

            if (!name.empty()) {
                auto &menuitem = constructMenuItem<BuildserverMenuItem>();
                menuitem.setBuildserverName(name);
                menuitem.setBuildserverUrl(url);
            }
        }

        if (menuItemCount() < 1)
        {
            constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NOBUILDSERVERCONFIGURED>, DefaultFont, StaticColor<TFT_RED>, DummyAction>>();
        }

        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
    }
};
} // namespace
