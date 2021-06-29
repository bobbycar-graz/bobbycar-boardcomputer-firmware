#pragma once

#include "changevaluedisplay.h"
#include "menudisplay.h"
#include "utils.h"
#include "actions/setvalueaction.h"
#include "actions/backproxyaction.h"
#include "icons/back.h"
#include "texts.h"
#include "unifiedmodelmode.h"

namespace {
template<>
class ChangeValueDisplay<UnifiedModelMode> :
    public MenuDisplay,
    public virtual AccessorInterface<UnifiedModelMode>,
    public virtual ActionInterface
{
    using Base = MenuDisplay;

public:
    ChangeValueDisplay();

    void start() override;
};

ChangeValueDisplay<UnifiedModelMode>::ChangeValueDisplay()
{
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_COMMUTATION>>>(UnifiedModelMode::Commutation, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_SINUSOIDAL>>>(UnifiedModelMode::Sinusoidal, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_FOCVOLTAGE>>>(UnifiedModelMode::FocVoltage, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_FOCSPEED>>>(UnifiedModelMode::FocSpeed, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<UnifiedModelMode>, StaticText<TEXT_FOCTORQUE>>>(UnifiedModelMode::FocTorque, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&icons::back>>>(*this);
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
        //Serial.printf("Unknown UnifiedModelMode: %i\r\n", int(value));
        setSelectedIndex(5);
    }
}
}
