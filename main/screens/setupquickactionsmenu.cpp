#include "setupquickactionsmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <textwithvaluehelper.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "bobbyquickactions.h"
#include "changevaluedisplay_bobbyquickactions.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "icons/back.h"

namespace bobby {

namespace {
constexpr char TEXT_SETUPQUICKACTIONS[] = "Setup QuickActions";
constexpr char TEXT_SETUPLEFT2[] = "&sSetup Left2";
constexpr char TEXT_SETUPRIGHT2[] = "&sSetup Right2";
constexpr char TEXT_SETUPUP2[] = "&sSetup Up2";
constexpr char TEXT_SETUPDOWN2[] = "&sSetup Down2";
constexpr char TEXT_SETUPEXTRA1[] = "&sSetup Extra1";
constexpr char TEXT_SETUPEXTRA2[] = "&sSetup Extra2";
constexpr char TEXT_SETUPEXTRA3[] = "&sSetup Extra3";
constexpr char TEXT_SETUPEXTRA4[] = "&sSetup Extra4";
constexpr char TEXT_BACK[] = "Back";

template<typename Tvalue, const char* TEXT, typename Accessor>
using QuickActionChangeValueDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<Tvalue>,
    espgui::StaticText<TEXT>,
    Accessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

SetupQuickActionsMenu::SetupQuickActionsMenu()
{
    using namespace espgui;

    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SETUPLEFT2, QuickActionLeft2Accessor>,    PushScreenAction<QuickActionChangeValueDisplay<BobbyQuickActions, TEXT_SETUPLEFT2, QuickActionLeft2Accessor>>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SETUPRIGHT2, QuickActionRight2Accessor>,  PushScreenAction<QuickActionChangeValueDisplay<BobbyQuickActions, TEXT_SETUPRIGHT2, QuickActionRight2Accessor>>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SETUPUP2, QuickActionUp2Accessor>,        PushScreenAction<QuickActionChangeValueDisplay<BobbyQuickActions, TEXT_SETUPUP2, QuickActionUp2Accessor>>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SETUPDOWN2, QuickActionDown2Accessor>,    PushScreenAction<QuickActionChangeValueDisplay<BobbyQuickActions, TEXT_SETUPDOWN2, QuickActionDown2Accessor>>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SETUPEXTRA1, QuickActionExtra1Accessor>,  PushScreenAction<QuickActionChangeValueDisplay<BobbyQuickActions, TEXT_SETUPEXTRA1, QuickActionExtra1Accessor>>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SETUPEXTRA2, QuickActionExtra2Accessor>,  PushScreenAction<QuickActionChangeValueDisplay<BobbyQuickActions, TEXT_SETUPEXTRA2, QuickActionExtra2Accessor>>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SETUPEXTRA3, QuickActionExtra3Accessor>,  PushScreenAction<QuickActionChangeValueDisplay<BobbyQuickActions, TEXT_SETUPEXTRA3, QuickActionExtra3Accessor>>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SETUPEXTRA4, QuickActionExtra4Accessor>,  PushScreenAction<QuickActionChangeValueDisplay<BobbyQuickActions, TEXT_SETUPEXTRA4, QuickActionExtra4Accessor>>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,  PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string SetupQuickActionsMenu::text() const
{
    return TEXT_SETUPQUICKACTIONS;
}

void SetupQuickActionsMenu::back()
{
    espgui::popScreen();
}
} // namespace bobby
