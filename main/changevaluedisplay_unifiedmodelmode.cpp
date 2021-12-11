#include "changevaluedisplay_unifiedmodelmode.h"

// esp-idf includes
#include <esp_log.h>

// local includes
#include "texts.h"
#include "utils.h"

namespace espgui
{
ChangeValueDisplay<UnifiedModelMode>::ChangeValueDisplay()
{
  constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_COMMUTATION>>>(UnifiedModelMode::Commutation, *this, *this);
  constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_SINUSOIDAL>>>(UnifiedModelMode::Sinusoidal, *this, *this);
  constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_FOCVOLTAGE>>>(UnifiedModelMode::FocVoltage, *this, *this);
  constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_FOCSPEED>>>(UnifiedModelMode::FocSpeed, *this, *this);
  constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_FOCTORQUE>>>(UnifiedModelMode::FocTorque, *this, *this);
  constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&espgui::icons::back>>>(*this);
}

void ChangeValueDisplay<UnifiedModelMode>::start()
{
  Base::start();

  switch (const auto value = getValue())
  {
  case UnifiedModelMode::Commutation: setSelectedIndex(0); break;
  case UnifiedModelMode::Sinusoidal: setSelectedIndex(1); break;
  case UnifiedModelMode::FocVoltage: setSelectedIndex(2); break;
  case UnifiedModelMode::FocSpeed: setSelectedIndex(3); break;
  case UnifiedModelMode::FocTorque: setSelectedIndex(4); break;
  default:
    ESP_LOGW("BOBBY", "Unknown UnifiedModelMode: %i", int(value));
    setSelectedIndex(5);
  }
}
} // namespace espgui
