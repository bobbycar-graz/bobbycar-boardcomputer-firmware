#include "changevaluedisplay_controlmode.h"

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

constexpr char TEXT_OPENMODE[] = "Open mode";
constexpr char TEXT_VOLTAGE[] = "Voltage";
constexpr char TEXT_SPEED[] = "Speed";
constexpr char TEXT_TORQUE[] = "Torque";
constexpr char TEXT_BACK[] = "Back";
} // namespace

ChangeValueDisplay<bobbycar::protocol::ControlMode>::ChangeValueDisplay()
{
    using bobbycar::protocol::ControlMode;
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<ControlMode>, StaticText<TEXT_OPENMODE>>>(ControlMode::OpenMode, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<ControlMode>, StaticText<TEXT_VOLTAGE>>>(ControlMode::Voltage, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<ControlMode>, StaticText<TEXT_SPEED>>>(ControlMode::Speed, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<ControlMode>, StaticText<TEXT_TORQUE>>>(ControlMode::Torque, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&bobbyicons::back>>>(*this);
}

void ChangeValueDisplay<bobbycar::protocol::ControlMode>::start()
{
    Base::start();

    switch (const auto value = getValue())
    {
    using bobbycar::protocol::ControlMode;
    case ControlMode::OpenMode: setSelectedIndex(0); break;
    case ControlMode::Voltage:  setSelectedIndex(1); break;
    case ControlMode::Speed:    setSelectedIndex(2); break;
    case ControlMode::Torque:   setSelectedIndex(3); break;
    default:
        ESP_LOGW(TAG, "Unknown ControlMode: %i", std::to_underlying(value));
        setSelectedIndex(4);
    }
}

} // namespace espgui
