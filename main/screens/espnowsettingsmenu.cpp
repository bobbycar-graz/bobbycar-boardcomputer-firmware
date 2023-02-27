#include "espnowsettingsmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <textinterface.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"

namespace bobby {

namespace {
constexpr char TEXT_ESPNOW_SETTINGS[] = "ESP-Now settings";
constexpr char TEXT_ESPNOW_SYNCTIME[] = "Sync time (no NTP)";
constexpr char TEXT_ESPNOW_SYNCWITHOTHERS[] = "Sync time with others";
constexpr char TEXT_ESPNOW_SYNCBLINK[] = "Sync blink";
constexpr char TEXT_BACK[] = "Back";
} // namespace

EspNowSettingsMenu::EspNowSettingsMenu()
{
    using namespace espgui;

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ESPNOW_SYNCTIME>, BobbyCheckbox, ESPNowSyncTimeEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ESPNOW_SYNCWITHOTHERS>, BobbyCheckbox, ESPNowSyncTimeWithOthersEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ESPNOW_SYNCBLINK>, BobbyCheckbox, ESPNowSyncBlinkEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string EspNowSettingsMenu::text() const
{
    return TEXT_ESPNOW_SETTINGS;
}

void EspNowSettingsMenu::back()
{
    espgui::popScreen();
}

} // namespace bobby
