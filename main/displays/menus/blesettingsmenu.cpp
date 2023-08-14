#include "blesettingsmenu.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/pushscreenaction.h>
#include <actions/popscreenaction.h>
#include <icons/back.h>
#include <changevaluedisplay.h>
#include <changevaluedisplay_string.h>
#include <textwithvaluehelper.h>

// local includes
#include "displays/bobbychangevaluedisplay.h"
#include "accessors/settingsaccessors.h"
#include "bletexthelpers.h"
#include "displays/menus/settingsmenu.h"
#include "bobbycheckbox.h"

namespace {
constexpr char TEXT_BLESETTINGS[] = "BLE settings";
constexpr char TEXT_ENABLED[] = "Enabled";
constexpr char TEXT_FENCE_ENABLED[] = "Fence enabled";
constexpr char TEXT_NAME[] = "Name";
constexpr char TEXT_NAME_FORMATTED[] = "Name: &s";
constexpr char TEXT_BACK[] = "Back";

using ApSsidChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<std::string>,
    espgui::StaticTitle<TEXT_NAME>,
    BluetoothNameAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

BleSettingsMenu::BleSettingsMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLED>, BobbyCheckbox, BleEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FENCE_ENABLED>, BobbyCheckbox, BleFenceEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, BleServerPeerDevicesText,    DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BleCharacSubscribedText,     DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_NAME_FORMATTED, BluetoothNameAccessor>, PushScreenAction<ApSsidChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,       PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string BleSettingsMenu::title() const
{
    return TEXT_BLESETTINGS;
}

void BleSettingsMenu::back()
{
    espgui::popScreen();
}
