#include "motortestmodesettingsmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay.h>
#include <menuitem.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "guihelpers/bobbychangevaluedisplay.h"

namespace bobby {

namespace {
constexpr char TEXT_MOTORTESTMODESETTINGS[] = "Motortest mode seetings";
constexpr char TEXT_MOTORTESTMAXPWM[] = "Max Pwm";
constexpr char TEXT_MOTORTESTMULTIPLIKATOR[] = "Acceleration";
constexpr char TEXT_BACK[] = "Back";

using MotortestMultiplikatorChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticTitle<TEXT_MOTORTESTMULTIPLIKATOR>,
    MotortestModeMultiplikatorAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using MotortestMaxPwmChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<uint16_t>,
    espgui::StaticTitle<TEXT_MOTORTESTMAXPWM>,
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
}

std::string MotortestModeSettingsMenu::title() const
{
    return TEXT_MOTORTESTMODESETTINGS;
}

void MotortestModeSettingsMenu::back()
{
    popScreen();
}
} // namespace bobby
