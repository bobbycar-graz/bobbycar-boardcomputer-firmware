#pragma once

#include "changevaluedisplay.h"
#include "menudisplay.h"
#include "utils.h"
#include "actions/setvalueaction.h"
#include "actions/backproxyaction.h"
#include "icons/back.h"
#include "texts.h"

namespace {
template<>
class ChangeValueDisplay<bobbycar::protocol::ControlMode> :
    public MenuDisplay,
    public virtual AccessorInterface<bobbycar::protocol::ControlMode>,
    public virtual ActionInterface
{
    using Base = MenuDisplay;

public:
    ChangeValueDisplay();

    void start() override;
};

ChangeValueDisplay<bobbycar::protocol::ControlMode>::ChangeValueDisplay()
{
    using bobbycar::protocol::ControlMode;
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<ControlMode>, StaticText<TEXT_OPENMODE>>>(ControlMode::OpenMode, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<ControlMode>, StaticText<TEXT_VOLTAGE>>>(ControlMode::Voltage, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<ControlMode>, StaticText<TEXT_SPEED>>>(ControlMode::Speed, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<ControlMode>, StaticText<TEXT_TORQUE>>>(ControlMode::Torque, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&icons::back>>>(*this);
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
        //Serial.printf("Unknown ControlMode: %i\r\n", int(value));
        setSelectedIndex(4);
    }
}
}
