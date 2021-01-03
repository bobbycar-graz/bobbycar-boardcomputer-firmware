#pragma once

// local includes
#include "menudisplay.h"
#include "menuitem.h"
#include "actions/dummyaction.h"
#include "actions/wifisoftapaction.h"
#include "actions/wifisoftapdisconnectaction.h"
#include "actions/wifisoftapdisconnectwifioffaction.h"
#include "actions/wifisoftapenableipv6action.h"
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
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_WIFISOFTAP>>(),                  std::make_unique<WifiSoftApAction>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_WIFISOFTAPDISCONNECT>>(),        std::make_unique<WifiSoftApDisconnectAction>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_WIFISOFTAPDISCONNECTWIFIOFF>>(), std::make_unique<WifiSoftApDisconnectWifioffAction>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiSoftApGetStationNumText>(),                  std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiSoftApIpText>(),                             std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiSoftApBroadcastIpText>(),                    std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiSoftApNetworkIdText>(),                      std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiSoftApSubnetCidrText>(),                     std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_WIFISOFTAPENABLEIPV6>>(),        std::make_unique<WifiSoftApEnableIpV6Action>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiSoftApIpV6Text>(),                           std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiSoftApHostnameText>(),                       std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiSoftApMacAddressText>(),                     std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_BACK>>(),                        std::make_unique<SwitchScreenAction<WifiSettingsMenu>>(), std::make_unique<StaticMenuItemIcon<&icons::back>>());
    }
};
} // namespace
