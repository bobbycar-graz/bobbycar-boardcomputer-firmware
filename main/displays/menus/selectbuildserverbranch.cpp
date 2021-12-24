#ifdef FEATURE_OTA
#include "selectbuildserverbranch.h"

// 3rd party includes
#include <espwifistack.h>

// local includes
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "buildserver.h"
#include "displays/menus/otamenu.h"
#include "globals.h"
#include "icons/back.h"
#include "icons/reboot.h"
#include "utilities/utils.h"

#define ERR_MESSAGE(text)                                                                                                                       \
    constructMenuItem<makeComponent<MenuItem, StaticText<text>, DefaultFont, StaticColor<TFT_RED>, DummyAction>>();                             \
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<OtaMenu>, StaticMenuItemIcon<&espgui::icons::back>>>(); \
    return;

using namespace espgui;
using namespace buildserver;
using namespace SelectBuildServerBranch;

namespace SelectBuildServerBranch {
std::string CurrentBranch::text() const
{
    return stringSettings.otaServerBranch.empty() ? "All builds" : stringSettings.otaServerBranch;
}

std::string BranchMenuItem::text() const
{
    return m_name;
}

void BranchMenuItem::setName(std::string &&name)
{
    m_name = std::move(name);
}

void BranchMenuItem::setName(const std::string &name)
{
    m_name = name;
}

void BranchMenuItem::triggered()
{
    stringSettings.otaServerBranch = m_name;
    saveSettings();
}

void ClearBranchAction::triggered()
{
    stringSettings.otaServerBranch = {};
    saveSettings();
}
}

SelectBuildserverBranchMenu::SelectBuildserverBranchMenu()
{
    using namespace SelectBuildServerBranch;

    if (count_available_buildserver() < 1)
    {
        ERR_MESSAGE(TEXT_OTA_NOBUILDSERVERAVAILABLE); // E:No server saved.
    }

    if (stringSettings.otaServerUrl.empty())
    {
        ERR_MESSAGE(TEXT_OTA_NOBUILDSERVERSELECTED); // E:No server selected.
    }

    if (const auto staStatus = wifi_stack::get_sta_status(); staStatus != wifi_stack::WiFiStaStatus::CONNECTED)
    {
        ERR_MESSAGE(TEXT_OTA_NOCONNECTION); // E:No internet.
    }

    SelectBranch::setup_request();
    SelectBranch::start_descriptor_request(stringSettings.otaServerUrl);
}

void SelectBuildserverBranchMenu::update()
{
    using namespace SelectBranch;
    if(get_request_running())
    {
        check_descriptor_request();
        if (!request_failed.empty())
        {
            this->buildMenuRequestError(request_failed);
            request_failed = {};
        }
    }

    if (!constructedMenu && branches.size() > 0)
    {
        constructedMenu = true;
        constructMenuItem<makeComponent<MenuItem, CurrentBranch, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, EmptyText, DummyAction>>();

        for (const std::string &branch : branches)
        {
            if (branch.empty())
                continue;
            auto &menuitem = constructMenuItem<BranchMenuItem>();
            menuitem.setName(branch);
        }

        constructMenuItem<makeComponent<MenuItem, EmptyText, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECT_BRANCH_CLEAR>, ClearBranchAction, StaticMenuItemIcon<&bobbyicons::reboot>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<OtaMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
    }
    Base::update();
}

void SelectBuildserverBranchMenu::back()
{
    switchScreen<OtaMenu>();
}

void SelectBuildserverBranchMenu::buildMenuRequestError(std::string error)
{
    auto &item = constructMenuItem<makeComponent<MenuItem, ChangeableText, DefaultFont, StaticColor<TFT_RED>, DummyAction>>();
    item.setTitle(error);
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<OtaMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}
#endif
