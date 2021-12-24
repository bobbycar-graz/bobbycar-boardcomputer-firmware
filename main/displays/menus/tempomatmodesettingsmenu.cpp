#include "tempomatmodesettingsmenu.h"

// 3rdparty lib includes
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"

// local includes
#include "utilities/utils.h"
#include "changevaluedisplay_unifiedmodelmode.h"
#include "accessors/settingsaccessors.h"
#include "accessors/globalaccessors.h"
#include "actions/tempomatmodeapplycurrentpeedaction.h"
#include "displays/menus/modessettingsmenu.h"

namespace {
using TempomatModeCruiseMotTgtChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_NCRUISEMOTTGT>,
    TempomatModeCruiseMotTgtAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<TempomatModeSettingsMenu>>,
    espgui::SwitchScreenAction<TempomatModeSettingsMenu>
>;

using TempomatModeModelModeChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<UnifiedModelMode>,
    espgui::StaticText<TEXT_MODELMODE>,
    TempomatModeModelModeAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<TempomatModeSettingsMenu>>,
    espgui::SwitchScreenAction<TempomatModeSettingsMenu>
>;
} // namespace

using namespace espgui;

TempomatModeSettingsMenu::TempomatModeSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_APPLY, AvgSpeedAccessor>, TempomatModeApplyCurrentSpeedAction>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_NCRUISEMOTTGT, TempomatModeCruiseMotTgtAccessor>, SwitchScreenAction<TempomatModeModelModeChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODELMODE>, SwitchScreenAction<TempomatModeModelModeChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,      SwitchScreenAction<ModesSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void TempomatModeSettingsMenu::back()
{
    switchScreen<ModesSettingsMenu>();
}
