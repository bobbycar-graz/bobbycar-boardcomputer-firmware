#include "gitmenu.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <fmt/core.h>

// local includes
#include "guihelpers/bobbyerrorhandler.h"
#include "icons/back.h"
#include "icons/info.h"
#include "icons/modes.h"
#include "screens/qrdisplay.h"

namespace bobby {

namespace {
    constexpr char TEXT_GIT[] = "Git";
    constexpr char TEXT_GIT_BRANCH_TITLE[] = "Branch";
    constexpr char TEXT_GIT_BRANCH[] = GIT_BRANCH;
    constexpr char TEXT_GIT_COMMIT_TITLE[] = "Commit";
    constexpr char TEXT_GIT_COMMIT[] = GIT_REV;
    constexpr char TEXT_GIT_COMMIT_SHORT[] = GIT_SHORT_REV;
    constexpr char TEXT_GIT_MESSAGE_TITLE[] = "Commit Message";
    constexpr char TEXT_GIT_MESSAGE[] = GIT_MESSAGE;
    constexpr char TEXT_GITHUB_URL[] = "Github URL";
    constexpr char TEXT_IDF_VERSION[] = "ESP-IDF Version";
    constexpr char TEXT_IDF_VERSION_VALUE[] = IDF_VER;
    constexpr char TEXT_BACK[] = "Back";

    class OpenPopupAction : public virtual espgui::ActionInterface
    {
    public:
        void triggered() override
        {
            BobbyErrorHandler{}.errorOccurred(TEXT_GIT_MESSAGE);
        }
    };

    class GitQrAction : public virtual espgui::ActionInterface
    {
    public:
        void triggered() override
        {
            // ToDo: Fix qr library and remove spaces, also, why is "`" at the end of string in qr code but not when logging?
            const std::string qr_text = fmt::format("https://github.com/bobbycar-graz/bobbycar-boardcomputer-firmware/commit/{}                  ", GIT_REV);
            espgui::pushScreen<QrDisplay>(qr_text);
        }
    };
} // namespace

GitMenu::GitMenu()
{
    using namespace espgui;

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_IDF_VERSION>,        StaticColor<espgui::TFT_GREY>, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_IDF_VERSION_VALUE>,  DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GIT_BRANCH_TITLE>,   StaticColor<espgui::TFT_GREY>, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GIT_BRANCH>,         DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GIT_COMMIT_TITLE>,   StaticColor<espgui::TFT_GREY>, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GIT_COMMIT>,         DummyAction, StaticFont<2>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GIT_COMMIT_SHORT>,   DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GIT_MESSAGE_TITLE>,  OpenPopupAction, StaticMenuItemIcon<&bobbyicons::info>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GITHUB_URL>,         GitQrAction, StaticMenuItemIcon<&bobbyicons::modes>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,               PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string GitMenu::text() const
{
    return TEXT_GIT;
}

void GitMenu::back()
{
    espgui::popScreen();
}

} // namespace bobby
