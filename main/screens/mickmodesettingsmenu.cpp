#include "mickmodesettingsmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay.h>
#include <menuitem.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "changevaluedisplay_unifiedmodelmode.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "icons/back.h"

namespace bobby {

namespace {
constexpr char TEXT_MICKMODESETTINGS[] = "Mick mode settings";
constexpr char TEXT_MODELMODE[] = "Model mode";
constexpr char TEXT_MICKMODE_SMOOTHING[] = "Tau in ms";
constexpr char TEXT_BACK[] = "Back";

using MickModeModelModeChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<UnifiedModelMode>,
    espgui::StaticText<TEXT_MODELMODE>,
    MickModeModelModeAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using MickModeSmoothingChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<uint16_t>,
    espgui::StaticText<TEXT_MICKMODE_SMOOTHING>,
    MickModeSmoothingAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

MickModeSettingsMenu::MickModeSettingsMenu()
{
    using namespace espgui;

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODELMODE>,          PushScreenAction<MickModeModelModeChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MICKMODE_SMOOTHING>, PushScreenAction<MickModeSmoothingChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,               PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string MickModeSettingsMenu::text() const
{
    return TEXT_MICKMODESETTINGS;
}

void MickModeSettingsMenu::back()
{
    espgui::popScreen();
}
} // namespace bobby
