#include "networksettingsmenu.h"

// 3rdparty lib includes
#include <menuitem.h>
#include <actions/dummyaction.h>
#include <actions/pushscreenaction.h>
#include <actions/popscreenaction.h>
#include <icons/back.h>
#include <screenmanager.h>
#include <fmt/format.h>

// local includes
#include "wifistasettingsmenu.h"
#include "wifiapsettingsmenu.h"
#include "texthelpers/networktexthelpers.h"

using namespace espgui;

namespace {
constexpr char TEXT_STASETTINGS[] = "STA Settings";
constexpr char TEXT_APSETTINGS[] = "AP Settings";
constexpr char TEXT_BACK[] = "Back";
} // namespace

NetworkSettingsMenu::NetworkSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem,     StaticText<TEXT_STASETTINGS>, PushScreenAction<WifiStaSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem,     StaticText<TEXT_APSETTINGS>,  PushScreenAction<WifiApSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem,     WifiDefaultMacText,           DummyAction>>();
    constructMenuItem<makeComponent<MenuItem,     WifiBaseMacText,              DummyAction>>();
    constructMenuItem<makeComponentArgs<MenuItem, DnsText,                      DummyAction>>(uint8_t{0});
    constructMenuItem<makeComponentArgs<MenuItem, DnsText,                      DummyAction>>(uint8_t{1});
    constructMenuItem<makeComponentArgs<MenuItem, DnsText,                      DummyAction>>(uint8_t{2});
    constructMenuItem<makeComponent<MenuItem,     WifiTxPowerText,              DummyAction>>();
    constructMenuItem<makeComponent<MenuItem,     StaticText<TEXT_BACK>,        PopScreenAction, StaticMenuItemIcon<&icons::back>>>();
}

std::string NetworkSettingsMenu::text() const
{
    return "Network settings";
}

void NetworkSettingsMenu::back()
{
    popScreen();
}
