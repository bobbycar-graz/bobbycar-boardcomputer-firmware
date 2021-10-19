#include "accesspointwifisettingsmenu.h"

using namespace espgui;

AccessPointWifiSettingsMenu::AccessPointWifiSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, WifiSoftApGetStationNumText,                  StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiSoftApIpText,                             StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiSoftApBroadcastIpText,                    StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiSoftApNetworkIdText,                      StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiSoftApSubnetCidrText,                     StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiSoftApIpV6Text,                           StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiSoftApHostnameText,                       StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiSoftApMacAddressText,                     StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                        SwitchScreenAction<WifiSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}
