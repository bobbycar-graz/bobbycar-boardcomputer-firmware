#include "motortestmodesettingsmenu.h"

// 3rdparty lib includes
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/pushscreenaction.h"
#include "actions/popscreenaction.h"
#include "icons/back.h"

// local includes
#include "displays/bobbychangevaluedisplay.h"
#include "utils.h"
#include "accessors/settingsaccessors.h"

namespace {
constexpr char TEXT_MOTORTESTMODESETTINGS[] = "Motortest mode seetings";
constexpr char TEXT_MOTORTESTMAXPWM[] = "Max Pwm";
constexpr char TEXT_MOTORTESTMULTIPLIKATOR[] = "Acceleration";
constexpr char TEXT_BACK[] = "Back";

using MotortestMultiplikatorChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_MOTORTESTMULTIPLIKATOR>,
    MotortestModeMultiplikatorAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using MotortestMaxPwmChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<uint16_t>,
    espgui::StaticText<TEXT_MOTORTESTMAXPWM>,
    MotortestMaxPwmAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

using namespace espgui;

MotortestModeSettingsMenu::MotortestModeSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOTORTESTMULTIPLIKATOR>,  PushScreenAction<MotortestMultiplikatorChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOTORTESTMAXPWM>,         PushScreenAction<MotortestMaxPwmChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                    PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string MotortestModeSettingsMenu::text() const
{
    return TEXT_MOTORTESTMODESETTINGS;
}

void MotortestModeSettingsMenu::back()
{
    popScreen();
}
