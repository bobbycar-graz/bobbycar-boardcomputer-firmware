#include "motortestmodesettingsmenu.h"

// 3rdparty lib includes
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"

// local includes
#include "utils.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/modessettingsmenu.h"

namespace {
using MotortestMultiplikatorChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_MOTORTESTMULTIPLIKATOR>,
    MotortestModeMultiplikatorAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<MotortestModeSettingsMenu>>,
    espgui::SwitchScreenAction<MotortestModeSettingsMenu>
>;

using MotortestMaxPwmChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<uint16_t>,
    espgui::StaticText<TEXT_MOTORTESTMAXPWM>,
    MotortestMaxPwmAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<MotortestModeSettingsMenu>>,
    espgui::SwitchScreenAction<MotortestModeSettingsMenu>
>;
} // namespace

using namespace espgui;

MotortestModeSettingsMenu::MotortestModeSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOTORTESTMULTIPLIKATOR>,  SwitchScreenAction<MotortestMultiplikatorChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOTORTESTMAXPWM>,         SwitchScreenAction<MotortestMaxPwmChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                    SwitchScreenAction<ModesSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void MotortestModeSettingsMenu::back()
{
    switchScreen<ModesSettingsMenu>();
}
