#include "larsmmodesettingsmenu.h"

// 3rdparty lib includes
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"

// local includes
#include "utilities/utils.h"
#include "changevaluedisplay_larsmmode_mode.h"
#include "changevaluedisplay_unifiedmodelmode.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/modessettingsmenu.h"

namespace {
using LarsmModeModelModeChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<UnifiedModelMode>,
    espgui::StaticText<TEXT_MODELMODE>,
    LarsmModeModelModeAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LarsmModeSettingsMenu>>,
    espgui::SwitchScreenAction<LarsmModeSettingsMenu>
>;
using LarsmModeModeChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<LarsmModeMode>,
    espgui::StaticText<TEXT_SETMODE>,
    LarsmModeModeAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LarsmModeSettingsMenu>>,
    espgui::SwitchScreenAction<LarsmModeSettingsMenu>
>;
using LarsmModeIterationsChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_SETITERATIONS>,
    LarsmModeIterationsAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LarsmModeSettingsMenu>>,
    espgui::SwitchScreenAction<LarsmModeSettingsMenu>
>;
} // namespace

using namespace espgui;

LarsmModeSettingsMenu::LarsmModeSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODELMODE>,     SwitchScreenAction<LarsmModeModelModeChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETMODE>,       SwitchScreenAction<LarsmModeModeChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETITERATIONS>, SwitchScreenAction<LarsmModeIterationsChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,          SwitchScreenAction<ModesSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void LarsmModeSettingsMenu::back()
{
    switchScreen<ModesSettingsMenu>();
}
