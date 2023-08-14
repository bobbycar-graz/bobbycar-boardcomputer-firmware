#include "timersmenu.h"

// 3rdparty lib includes
#include "actions/pushscreenaction.h"
#include "actions/popscreenaction.h"
#include "changevaluedisplay.h"
#include "icons/back.h"

// local includes
#include "displays/bobbychangevaluedisplay.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/boardcomputerhardwaresettingsmenu.h"

namespace {
constexpr char TEXT_TIMERS[] = "Timers";
constexpr char TEXT_STATSUPDATERATE[] = "Stats update rate";
constexpr char TEXT_BACK[] = "Back";

using StatsUpdateRateChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticTitle<TEXT_STATSUPDATERATE>,
    StatsUpdateRateAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

TimersMenu::TimersMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STATSUPDATERATE>,   espgui::PushScreenAction<StatsUpdateRateChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,              espgui::PushScreenAction<BoardcomputerHardwareSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string TimersMenu::title() const
{
    return TEXT_TIMERS;
}

void TimersMenu::back()
{
    espgui::popScreen();
}
