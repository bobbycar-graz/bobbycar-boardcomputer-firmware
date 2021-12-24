#include "stationwifisettingsmenu.h"

// 3rdparty lib includes
#include "menuitem.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "actions/toggleboolaction.h"
#include "checkboxicon.h"
#include "icons/back.h"

// local includes
#include "utilities/utils.h"
#include "actions/wifiscanaction.h"
#include "icons/scan.h"
#include "wifitexthelpers.h"
#include "accessors/wifiaccessors.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/wifiscanmenu.h"
#include "displays/menus/wifisettingsmenu.h"

using namespace espgui;

StationWifiSettingsMenu::StationWifiSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFISTAENABLED>,  ToggleBoolAction, CheckboxIcon, WifiStaEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFITRIGGERSCAN>, WifiScanAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFISCANRESULTS>, SwitchScreenAction<WifiScanMenu>, StaticMenuItemIcon<&bobbyicons::scan>>>();
    constructMenuItem<makeComponent<MenuItem, WifiStatusText,                   StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiScanStatusText,               StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiHostnameText,                 StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiMacText,                      StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiSsidText,                     StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiBssidText,                    StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiRssiText,                     StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiEncryptionTypeText,           StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiPairwiseCipherText,           StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiGroupCipherText,              StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiIpText,                       StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiNetmaskText,                  StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiGatewayText,                  StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiIpv6LinklocalText,            StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiIpv6GlobalText,               StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiDns0Text,                     StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiDns1Text,                     StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiDns2Text,                     StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,            SwitchScreenAction<WifiSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void StationWifiSettingsMenu::back()
{
    switchScreen<WifiSettingsMenu>();
}
