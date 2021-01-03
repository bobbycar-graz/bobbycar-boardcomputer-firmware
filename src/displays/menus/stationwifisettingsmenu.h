#pragma once

// Arduino includes
#include <WiFi.h>

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
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_WIFIRECONNECT>>(),           std::make_unique<WifiReconnectAction>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_WIFIDISCONNECT>>(),          std::make_unique<WifiDisconnectAction>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiIsConnectedText>(),                      std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        //constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_WIFICHANGEAUTOCONNECT>>(),   std::make_unique<ToggleBoolAction>(std::make_unique<WifiAutoConnectAccessor>()), std::make_unique<CheckboxIcon>(std::make_unique<WifiAutoConnectAccessor>())); // TODO feedc0de
        //constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_WIFICHANGEAUTORECONNECT>>(), std::make_unique<ToggleBoolAction>(std::make_unique<WifiAutoReconnectAccessor>()), std::make_unique<CheckboxIcon>(std::make_unique<WifiAutoReconnectAccessor>())); // TODO feedc0de
        constructMenuItem<NewMenuItem>(std::make_unique<WifiLocalIpText>(),                          std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiMacAddressText>(),                       std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiSubnetMaskText>(),                       std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiGatewayIpText>(),                        std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiDnsIpText>(),                            std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiBroadcastIpText>(),                      std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiNetworkIdText>(),                        std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiSubnetCIDRText>(),                       std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_WIFIENABLEIPV6>>(),          std::make_unique<WifiEnableIpV6Action>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiLocalIpV6Text>(),                        std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiHostnameText>(),                         std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiStatusText>(),                           std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiSsidText>(),                             std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiPskText>(),                              std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiBssidText>(),                            std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<WifiRssiText>(),                             std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_BACK>>(),                    std::make_unique<SwitchScreenAction<WifiSettingsMenu>>(), std::make_unique<StaticMenuItemIcon<&icons::back>>());
    }
};
} // namespace
