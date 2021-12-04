#include "changevaluedisplay_handbremsmode.h"

// esp-idf includes
#include <esp_log.h>

// local includes
#include "utils.h"
#include "texts.h"

namespace espgui {
ChangeValueDisplay<HandbremseMode>::ChangeValueDisplay()
{
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<HandbremseMode>, StaticText<TEXT_HANDBREMS_MOSFETS_OFF>>>(HandbremseMode::MOSFETS_OFF, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<HandbremseMode>, StaticText<TEXT_HANDBREMS_OPENMODE>>>(HandbremseMode::OPENMODE, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<HandbremseMode>, StaticText<TEXT_HANDBREMS_SPEED0>>>(HandbremseMode::SPEED_0, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&espgui::icons::back>>>(*this);
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
        ESP_LOGW("BOBBY", "Unknown HandbremseMode: %i", int(value));
        setSelectedIndex(3);
    }
}
} // namespace espgui
