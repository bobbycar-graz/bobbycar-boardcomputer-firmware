#include "handbremssettingsmenu.h"

// 3rd party libs
#include <fmt/core.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "actions/switchscreenaction.h"
#include "actions/toggleboolaction.h"
#include "changevaluedisplay.h"
#include "changevaluedisplay_handbremsmode.h"
#include "checkboxicon.h"
#include "displays/menus/defaultmodesettingsmenu.h"
#include "icons/back.h"
#include "texts.h"

using namespace espgui;

namespace {
using HandBremsTriggerTimeoutChangeValueDisplay = makeComponent<
    ChangeValueDisplay<uint16_t>,
    StaticText<TEXT_HANDBREMSE_TRIGGERTIMEOUT>,
    HandbremsTimeoutAccessor,
    BackActionInterface<SwitchScreenAction<HandbremsSettingsMenu>>,
    SwitchScreenAction<HandbremsSettingsMenu>
>;
using HandBremsModeChangeValueDisplay = makeComponent<
    ChangeValueDisplay<HandbremseMode>,
    StaticText<TEXT_HANDBREMSE_MODE>,
    HandbremsModeAccessor,
    BackActionInterface<SwitchScreenAction<HandbremsSettingsMenu>>,
    SwitchScreenAction<HandbremsSettingsMenu>
>;
class HandBremsModeText : public virtual TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("Mode: &2{}", toString(settings.handbremse.mode));
    }
};
}

HandbremsSettingsMenu::HandbremsSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_HANDBREMSE_ENABLE>, ToggleBoolAction, CheckboxIcon, HandbremsEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_HANDBREMSE_AUTOMATIC>, ToggleBoolAction, CheckboxIcon, HandbremsAutomaticAccessor>>();
    constructMenuItem<makeComponent<MenuItem, HandBremsModeText,    SwitchScreenAction<HandBremsModeChangeValueDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_HANDBREMSE_TRIGGERTIMEOUT, HandbremsTimeoutAccessor>, SwitchScreenAction<HandBremsTriggerTimeoutChangeValueDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<DefaultModeSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void HandbremsSettingsMenu::back()
{
    switchScreen<DefaultModeSettingsMenu>();
}
