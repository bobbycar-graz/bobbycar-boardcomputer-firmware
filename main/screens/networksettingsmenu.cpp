#include "networksettingsmenu.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <fmt/format.h>
#include <menuitem.h>
#include <screenmanager.h>

// local includes
#include "globals.h"
#include "icons/back.h"
#include "screens/qrdisplay.h"
#include "texthelpers/networktexthelpers.h"
#include "utils.h"
#include "wifiapsettingsmenu.h"
#include "wifistasettingsmenu.h"

namespace bobby {

using namespace espgui;

namespace {
constexpr char TEXT_STASETTINGS[] = "STA Settings";
constexpr char TEXT_APSETTINGS[] = "AP Settings";
constexpr char TEXT_APQRCODE[] = "AP QR Code";
constexpr char TEXT_BACK[] = "Back";
} // namespace

NetworkSettingsMenu::NetworkSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem,     StaticText<TEXT_STASETTINGS>, PushScreenAction<WifiStaSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem,     StaticText<TEXT_APSETTINGS>,  PushScreenAction<WifiApSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem,     StaticText<TEXT_APQRCODE>,    NetworkAccessPointQRAction>>();
    constructMenuItem<makeComponent<MenuItem,     WifiDefaultMacText,           DummyAction>>();
    constructMenuItem<makeComponent<MenuItem,     WifiBaseMacText,              DummyAction>>();
    constructMenuItem<makeComponentArgs<MenuItem, DnsText,                      DummyAction>>(uint8_t{0});
    constructMenuItem<makeComponentArgs<MenuItem, DnsText,                      DummyAction>>(uint8_t{1});
    constructMenuItem<makeComponentArgs<MenuItem, DnsText,                      DummyAction>>(uint8_t{2});
    constructMenuItem<makeComponent<MenuItem,     WifiTxPowerText,              DummyAction>>();
    constructMenuItem<makeComponent<MenuItem,     StaticText<TEXT_BACK>,        PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string NetworkSettingsMenu::text() const
{
    return "Network settings";
}

void NetworkSettingsMenu::back()
{
    popScreen();
}

void NetworkAccessPointQRAction::triggered()
{
    std::string qr = fmt::format("WIFI:T:{};S:{};P:{};H:;", get_wifi_security_string(configs.wifiApAuthmode.value()), configs.wifiApName.value(), configs.wifiApKey.value());
    espgui::pushScreen<QrDisplay>(qr);
}
} // namespace bobby
