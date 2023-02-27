#include "changevaluedisplay_handbremsmode.h"

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

constexpr char TEXT_HANDBREMS_MOSFETS_OFF[] = "Mosfets off";
constexpr char TEXT_HANDBREMS_OPENMODE[] = "Open Mode";
constexpr char TEXT_HANDBREMS_SPEED0[] = "Speed 0";
constexpr char TEXT_HANDBREMSE[] = "Handbremse";
constexpr char TEXT_BACK[] = "Back";
} // namespace

ChangeValueDisplay<HandbremseMode>::ChangeValueDisplay()
{
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<HandbremseMode>, StaticText<TEXT_HANDBREMS_MOSFETS_OFF>>>(HandbremseMode::MOSFETS_OFF, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<HandbremseMode>, StaticText<TEXT_HANDBREMS_OPENMODE>>>(HandbremseMode::OPENMODE, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<HandbremseMode>, StaticText<TEXT_HANDBREMS_SPEED0>>>(HandbremseMode::SPEED_0, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&bobbyicons::back>>>(*this);
}

void ChangeValueDisplay<HandbremseMode>::start()
{
    Base::start();

    switch (const auto value = getValue())
    {
    case HandbremseMode::MOSFETS_OFF: setSelectedIndex(0); break;
    case HandbremseMode::OPENMODE:  setSelectedIndex(1); break;
    case HandbremseMode::SPEED_0:  setSelectedIndex(2); break;
    default:
        ESP_LOGW(TAG, "Unknown HandbremseMode: %i", std::to_underlying(value));
        setSelectedIndex(3);
    }
}

} // namespace espgui
