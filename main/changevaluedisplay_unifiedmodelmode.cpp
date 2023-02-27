#include "changevaluedisplay_unifiedmodelmode.h"

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

constexpr char TEXT_COMMUTATION[] = "Commutation";
constexpr char TEXT_SINUSOIDAL[] = "Sinusoidal";
constexpr char TEXT_FOCVOLTAGE[] = "FOC/Voltage";
constexpr char TEXT_FOCSPEED[] = "FOC/Speed";
constexpr char TEXT_FOCTORQUE[] = "FOC/Torque";
constexpr char TEXT_BACK[] = "Back";
} // namespace

ChangeValueDisplay<UnifiedModelMode>::ChangeValueDisplay()
{
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_COMMUTATION>>>(UnifiedModelMode::Commutation, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_SINUSOIDAL>>>(UnifiedModelMode::Sinusoidal, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_FOCVOLTAGE>>>(UnifiedModelMode::FocVoltage, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_FOCSPEED>>>(UnifiedModelMode::FocSpeed, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_FOCTORQUE>>>(UnifiedModelMode::FocTorque, *this, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&bobbyicons::back>>>(*this);
}

void ChangeValueDisplay<UnifiedModelMode>::start()
{
    Base::start();

    switch (const auto value = getValue())
    {
    case UnifiedModelMode::Commutation: setSelectedIndex(0); break;
    case UnifiedModelMode::Sinusoidal:  setSelectedIndex(1); break;
    case UnifiedModelMode::FocVoltage:  setSelectedIndex(2); break;
    case UnifiedModelMode::FocSpeed:    setSelectedIndex(3); break;
    case UnifiedModelMode::FocTorque:   setSelectedIndex(4); break;
    default:
        ESP_LOGW(TAG, "Unknown UnifiedModelMode: %i", std::to_underlying(value));
        setSelectedIndex(5);
    }
}

} // namespace espgui
