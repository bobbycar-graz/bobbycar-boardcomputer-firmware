#include "selectbuildserverbranch.h"

// 3rd party includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <espwifistack.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "buildserver.h"
#include "guihelpers/bobbyerrorhandler.h"
#include "icons/back.h"
#include "icons/reboot.h"
#include "newsettings.h"

namespace bobby {

namespace {
constexpr char TEXT_OTA_NOBUILDSERVERAVAILABLE[] = "E:No server saved.";
constexpr char TEXT_OTA_NOBUILDSERVERSELECTED[] = "E:No server selected.";
constexpr char TEXT_OTA_NOCONNECTION[] = "E:No internet.";
constexpr char TEXT_SELECT_BRANCH[] = "Select Branch";
constexpr char TEXT_SELECT_BRANCH_CLEAR[] = "Clear Branch";
constexpr char TEXT_BACK[] = "Back";

class CurrentBranch : public virtual espgui::TextInterface
{
public:
    std::string text() const override;
};

class BranchMenuItem : public espgui::MenuItem
{
public:
    std::string text() const override;
    void setName(std::string &&name);
    void setName(const std::string &name);

    void triggered() override;
private:
    std::string m_name;
};

class ClearBranchAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};
}

namespace {
std::string CurrentBranch::text() const
{
    return configs.otaServerBranch.value().empty() ? "All builds" : configs.otaServerBranch.value();
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
    configs.write_config(configs.otaServerBranch, m_name); // mir egal ob succeeded
}

void ClearBranchAction::triggered()
{
    configs.write_config(configs.otaServerBranch, {}); // mir egal ob succeeded
}
} // namespace

SelectBuildserverBranchMenu::SelectBuildserverBranchMenu()
{
    using namespace espgui;
    using namespace buildserver;

#define ERR_MESSAGE(text)                                                                                                                       \
    constructMenuItem<makeComponent<MenuItem, StaticText<text>, DefaultFont, StaticColor<espgui::TFT_RED>, DummyAction>>();                             \
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>(); \
    return;

    if (count_available_buildserver() < 1)
    {
        ERR_MESSAGE(TEXT_OTA_NOBUILDSERVERAVAILABLE); // E:No server saved.
    }

    if (configs.otaServerUrl.value().empty())
    {
        ERR_MESSAGE(TEXT_OTA_NOBUILDSERVERSELECTED); // E:No server selected.
    }

    if (const auto staStatus = wifi_stack::get_sta_status(); staStatus != wifi_stack::WiFiStaStatus::CONNECTED)
    {
        ERR_MESSAGE(TEXT_OTA_NOCONNECTION); // E:No internet.
    }
#undef ERR_MESSAGE

    SelectBranch::setup_request();
    SelectBranch::start_descriptor_request(configs.otaServerUrl.value());
}

std::string SelectBuildserverBranchMenu::text() const
{
    return TEXT_SELECT_BRANCH;
}

void SelectBuildserverBranchMenu::update()
{
    using namespace espgui;
    using namespace buildserver::SelectBranch;

    if(get_request_running())
    {
        check_descriptor_request();
        if (!request_failed.empty())
        {
            BobbyErrorHandler{}.errorOccurred(fmt::format("Error: {}", request_failed));
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
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
    }
    Base::update();
}

void SelectBuildserverBranchMenu::back()
{
    espgui::popScreen();
}
} // namespace bobby
