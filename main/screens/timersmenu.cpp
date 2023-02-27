#include "timersmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "icons/back.h"
#include "screens/boardcomputerhardwaresettingsmenu.h"

namespace bobby {

namespace {
constexpr char TEXT_TIMERS[] = "Timers";
constexpr char TEXT_STATSUPDATERATE[] = "Stats update rate";
constexpr char TEXT_BACK[] = "Back";

using StatsUpdateRateChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_STATSUPDATERATE>,
    StatsUpdateRateAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

TimersMenu::TimersMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STATSUPDATERATE>,   espgui::PushScreenAction<StatsUpdateRateChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,              espgui::PushScreenAction<BoardcomputerHardwareSettingsMenu>, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string TimersMenu::text() const
{
    return TEXT_TIMERS;
}

void TimersMenu::back()
{
    espgui::popScreen();
}

} // namespace bobby
