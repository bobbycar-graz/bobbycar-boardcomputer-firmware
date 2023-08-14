#include "handbremssettingsmenu.h"

// 3rd party libs
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay.h>
#include <fmt/core.h>
#include <icons/back.h>
#include <textwithvaluehelper.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "bobbycheckbox.h"
#include "changevaluedisplay_handbremsmode.h"
#include "displays/bobbychangevaluedisplay.h"
#include "displays/menus/typesafeenumchangemenu.h"

namespace {
constexpr char TEXT_HANDBREMSE_ENABLE[] = "Enable Handbremse";
constexpr char TEXT_HANDBREMSE_AUTOMATIC[] = "Automatic Handbremse";
constexpr char TEXT_HANDBREMSE_VISUALIZE[] = "Visualize Handbremse";
constexpr char TEXT_HANDBREMSE_MODE[] = "Handbrems Mode";
constexpr char TEXT_HANDBREMSE_TRIGGERTIMEOUT[] = "Handbrems Timeout";
constexpr char TEXT_HANDBREMSE[] = "Handbremse";
constexpr char TEXT_BACK[] = "Back";

using HandBremsTriggerTimeoutChangeValueDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<uint16_t>,
    espgui::StaticTitle<TEXT_HANDBREMSE_TRIGGERTIMEOUT>,
    HandbremsTimeoutAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
}

HandbremsSettingsMenu::HandbremsSettingsMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_HANDBREMSE_ENABLE>, BobbyCheckbox, HandbremsEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_HANDBREMSE_AUTOMATIC>, BobbyCheckbox, HandbremsAutomaticAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_HANDBREMSE_VISUALIZE>, BobbyCheckbox, HandbremsVisualizeAccessor>>();
    constructMenuItem<PushScreenTypeSafeChangeMenuItem<HandbremseMode, TEXT_HANDBREMSE_MODE>>(&configs.handbremse.mode);
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_HANDBREMSE_TRIGGERTIMEOUT, HandbremsTimeoutAccessor>, PushScreenAction<HandBremsTriggerTimeoutChangeValueDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string HandbremsSettingsMenu::title() const
{
    return TEXT_HANDBREMSE;
}

void HandbremsSettingsMenu::back()
{
    espgui::popScreen();
}
