#include "wifistasettingsmenu.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay.h>
#include <menuitem.h>
#include <screenmanager.h>
#include <textwithvaluehelper.h>

// local includes
#include "accessors/wifistaconfigaccessors.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"
#include "texthelpers/wifistatexthelpers.h"
#include "wifistaconfigsmenu.h"
#include "wifistascanmenu.h"

namespace bobby {

using namespace espgui;

namespace {
constexpr char TEXT_ENABLED[] = "Enabled";
constexpr char TEXT_CONFIGURATIONS[] = "Configurations";
constexpr char TEXT_MIN_RSSI[] = "AP Min RSSI";
constexpr char TEXT_MIN_RSSI_FORMATTED[] = "&sMin RSSI: &f&2";
constexpr char TEXT_BACK[] = "Back";

using StaMinRssiChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int8_t>,
    espgui::StaticText<TEXT_MIN_RSSI>,
    WifiStaMinRssiAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

WifiStaSettingsMenu::WifiStaSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLED>,        BobbyCheckbox, WifiStaEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, WifiStaStatusText,               DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiStaMacText,                  DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiStaScanStatusText,           PushScreenAction<WifiStaScanMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CONFIGURATIONS>, PushScreenAction<WifiStaConfigsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, WifiSsidText,                    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiBssidText,                   DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiRssiText,                    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiAuthmodeText,                DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiPairwiseCipherText,          DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiGroupCipherText,             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiStaIpText,                   DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiStaNetmaskText,              DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiStaGatewayText,              DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiStaHostnameText,             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiStaIpv6LinklocalText,        DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiStaIpv6GlobalText,           DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_MIN_RSSI_FORMATTED, WifiStaMinRssiAccessor>, PushScreenAction<StaMinRssiChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,           PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string WifiStaSettingsMenu::text() const
{
    return "Sta Settings";
}

void WifiStaSettingsMenu::back()
{
    popScreen();
}
} // namespace bobby
