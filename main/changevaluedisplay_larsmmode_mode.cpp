#include "changevaluedisplay_larsmmode_mode.h"

// system includes
#include <utility>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib inclues
#include <actions/setvalueaction.h>
#include <actions/backproxyaction.h>

// local includes
#include "icons/back.h"
#include "utils.h"

namespace espgui {
namespace {
constexpr const char * const TAG = "ESPGUI";

constexpr char TEXT_LARSMMODE1[] = "Mode1";
constexpr char TEXT_LARSMMODE2[] = "Mode2";
constexpr char TEXT_LARSMMODE3[] = "Mode3";
constexpr char TEXT_LARSMMODE4[] = "Mode4";
constexpr char TEXT_BACK[] = "Back";
} // namespace

ChangeValueDisplay<LarsmModeMode>::ChangeValueDisplay()
{
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<LarsmModeMode>, StaticText<TEXT_LARSMMODE1>>>(LarsmModeMode::Mode1, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<LarsmModeMode>, StaticText<TEXT_LARSMMODE2>>>(LarsmModeMode::Mode2, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<LarsmModeMode>, StaticText<TEXT_LARSMMODE3>>>(LarsmModeMode::Mode3, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<LarsmModeMode>, StaticText<TEXT_LARSMMODE4>>>(LarsmModeMode::Mode4, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&bobbyicons::back>>>(*this);
}

void ChangeValueDisplay<LarsmModeMode>::start()
{
    Base::start();

    switch (const auto value = getValue())
    {
    case LarsmModeMode::Mode1: setSelectedIndex(0); break;
    case LarsmModeMode::Mode2: setSelectedIndex(1); break;
    case LarsmModeMode::Mode3: setSelectedIndex(2); break;
    case LarsmModeMode::Mode4: setSelectedIndex(3); break;
    default:
        ESP_LOGW(TAG, "Unknown LarsmModeMode: %i", std::to_underlying(value));
        setSelectedIndex(4);
    }
}

} // namespace espgui
