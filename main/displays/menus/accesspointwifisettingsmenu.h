#pragma once

// local includes
#include "menudisplay.h"
#include "menuitem.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "wifitexthelpers.h"
#include "texts.h"

// forward declares
namespace {
class WifiSettingsMenu;
} // namespace

namespace {
class AccessPointWifiSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_ACCESSPOINTWIFISETTINGS>,
    public BackActionInterface<SwitchScreenAction<WifiSettingsMenu>>
{
public:
    AccessPointWifiSettingsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, WifiSoftApGetStationNumText,                  StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiSoftApIpText,                             StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiSoftApBroadcastIpText,                    StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiSoftApNetworkIdText,                      StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiSoftApSubnetCidrText,                     StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiSoftApIpV6Text,                           StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiSoftApHostnameText,                       StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiSoftApMacAddressText,                     StaticFont<2>, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                        SwitchScreenAction<WifiSettingsMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
} // namespace
