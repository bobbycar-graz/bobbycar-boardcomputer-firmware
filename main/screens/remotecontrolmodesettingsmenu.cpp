#include "remotecontrolmodesettingsmenu.h"

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
    constexpr char TEXT_REMOTEMODESETTINGS[] = "Remote mode settings";
    constexpr char TEXT_MODELMODE[] = "Model mode";
    constexpr char TEXT_BACK[] = "Back";

    using RemoteControlModeModelModeChangeDisplay = espgui::makeComponent<
            BobbyChangeValueDisplay<UnifiedModelMode>,
            espgui::StaticText<TEXT_MODELMODE>,
            RemoteControlModeModelModeAccessor,
            espgui::ConfirmActionInterface<espgui::PopScreenAction>,
            espgui::BackActionInterface<espgui::PopScreenAction>
    >;
} // namespace

RemoteControlModeSettingsMenu::RemoteControlModeSettingsMenu()
{
    using namespace espgui;

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODELMODE>,          PushScreenAction<RemoteControlModeModelModeChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,               PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string RemoteControlModeSettingsMenu::text() const
{
    return TEXT_REMOTEMODESETTINGS;
}

void RemoteControlModeSettingsMenu::back()
{
    espgui::popScreen();
}
} // namespace bobby
