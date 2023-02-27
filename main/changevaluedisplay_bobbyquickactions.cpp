#include "changevaluedisplay_bobbyquickactions.h"

// system includes
#include <utility>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <actions/setvalueaction.h>
#include <actions/backproxyaction.h>

// local includes
#include "icons/back.h"
#include "utils.h"

namespace espgui {
namespace {
constexpr const char * const TAG = "ESPGUI";

constexpr char TEXT_QUICKACTION_NONE[] = "None";
constexpr char TEXT_QUICKACTION_BLINK_LEFT[] = "Blink Left";
constexpr char TEXT_QUICKACTION_BLINK_RIGHT[] = "Blink Right";
constexpr char TEXT_QUICKACTION_HANDBREMSE[] = "Handbremse";
constexpr char TEXT_QUICKACTION_OPEN_GARAGE[] = "Open Garage";
constexpr char TEXT_QUICKACTION_WIFI_SCAN[] = "Wifi Scan";
constexpr char TEXT_QUICKACTION_TEMPOMAT[] = "Toggle PWM-Omat";
constexpr char TEXT_QUICKACTION_COMPRESSOR[] = "Remote-Compressor";
constexpr char TEXT_QUICKACTION_HUPE[] = "Remote-Hupe";
constexpr char TEXT_BACK[] = "Back";
} // namespace

ChangeValueDisplay<BobbyQuickActions>::ChangeValueDisplay()
{
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<BobbyQuickActions>, StaticText<TEXT_QUICKACTION_NONE>>>(BobbyQuickActions::NONE, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<BobbyQuickActions>, StaticText<TEXT_QUICKACTION_BLINK_LEFT>>>(BobbyQuickActions::BLINK_LEFT, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<BobbyQuickActions>, StaticText<TEXT_QUICKACTION_BLINK_RIGHT>>>(BobbyQuickActions::BLINK_RIGHT, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<BobbyQuickActions>, StaticText<TEXT_QUICKACTION_HANDBREMSE>>>(BobbyQuickActions::HANDBREMSE, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<BobbyQuickActions>, StaticText<TEXT_QUICKACTION_OPEN_GARAGE>>>(BobbyQuickActions::OPEN_GARAGE, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<BobbyQuickActions>, StaticText<TEXT_QUICKACTION_WIFI_SCAN>>>(BobbyQuickActions::WIFI_SCAN, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<BobbyQuickActions>, StaticText<TEXT_QUICKACTION_TEMPOMAT>>>(BobbyQuickActions::PWMOMAT, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<BobbyQuickActions>, StaticText<TEXT_QUICKACTION_COMPRESSOR>>>(BobbyQuickActions::COMPRESSOR_TOGGLE, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<BobbyQuickActions>, StaticText<TEXT_QUICKACTION_HUPE>>>(BobbyQuickActions::HUPE, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&bobbyicons::back>>>(*this);
}

void ChangeValueDisplay<BobbyQuickActions>::start()
{
    Base::start();

    switch (const auto value = getValue())
    {
    case BobbyQuickActions::NONE: setSelectedIndex(0); break;
    case BobbyQuickActions::BLINK_LEFT: setSelectedIndex(1); break;
    case BobbyQuickActions::BLINK_RIGHT: setSelectedIndex(2); break;
    case BobbyQuickActions::HANDBREMSE: setSelectedIndex(3); break;
    case BobbyQuickActions::OPEN_GARAGE: setSelectedIndex(4); break;
    case BobbyQuickActions::WIFI_SCAN: setSelectedIndex(5); break;
    case BobbyQuickActions::PWMOMAT: setSelectedIndex(6); break;
    case BobbyQuickActions::COMPRESSOR_TOGGLE: setSelectedIndex(7); break;
    case BobbyQuickActions::HUPE: setSelectedIndex(8); break;
        default:
        ESP_LOGW(TAG, "Unknown BobbyQuickActions: %i", std::to_underlying(value));
        setSelectedIndex(9);
    }
}

} // namespace espgui
