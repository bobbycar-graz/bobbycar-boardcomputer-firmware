#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "menuitem.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "actions/wifireconnectaction.h"
#include "actions/wifidisconnectaction.h"
#include "actions/toggleboolaction.h"
#include "actions/wifienableipv6action.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "wifitexthelpers.h"
#include "wifiaccessors.h"
#include "texts.h"

// forward declares
namespace {
class WifiSettingsMenu;
} // namespace

namespace {
class StationWifiSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_STATIONWIFISETTINGS>,
    public BackActionInterface<SwitchScreenAction<WifiSettingsMenu>>
{
public:
    StationWifiSettingsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFIRECONNECT>,           WifiReconnectAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFIDISCONNECT>,          WifiDisconnectAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiIsConnectedText,                      StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFICHANGEAUTOCONNECT>,   ToggleBoolAction, CheckboxIcon, WifiAutoConnectAccessor>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFICHANGEAUTORECONNECT>, ToggleBoolAction, CheckboxIcon, WifiAutoReconnectAccessor>>();
        constructMenuItem<makeComponent<MenuItem, WifiLocalIpText,                          StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiMacAddressText,                       StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiSubnetMaskText,                       StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiGatewayIpText,                        StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiDnsIpText,                            StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiBroadcastIpText,                      StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiNetworkIdText,                        StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiSubnetCIDRText,                       StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFIENABLEIPV6>,          WifiEnableIpV6Action>>();
        constructMenuItem<makeComponent<MenuItem, WifiLocalIpV6Text,                        StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiHostnameText,                         StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiStatusText,                           StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiSsidText,                             StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiPskText,                              StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiBssidText,                            StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiRssiText,                             StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                    SwitchScreenAction<WifiSettingsMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
} // namespace
