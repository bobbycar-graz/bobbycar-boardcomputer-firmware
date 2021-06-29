#pragma once

#include "changevaluedisplay.h"
#include "menudisplay.h"
#include "utils.h"
#include "actions/setvalueaction.h"
#include "actions/backproxyaction.h"
#include "icons/back.h"
#include "texts.h"
#ifdef FEATURE_BLUETOOTH
#include "bluetoothmode.h"
#endif

namespace {
#ifdef FEATURE_BLUETOOTH
template<>
class ChangeValueDisplay<BluetoothMode> :
    public MenuDisplay,
    public virtual AccessorInterface<BluetoothMode>,
    public virtual ActionInterface
{
    using Base = MenuDisplay;

public:
    ChangeValueDisplay();

    void start() override;
};

ChangeValueDisplay<BluetoothMode>::ChangeValueDisplay()
{
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<BluetoothMode>, StaticText<TEXT_OFF>>>(BluetoothMode::Off, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<BluetoothMode>, StaticText<TEXT_MASTER>>>(BluetoothMode::Master, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<BluetoothMode>, StaticText<TEXT_SLAVE>>>(BluetoothMode::Slave, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&icons::back>>>(*this);
}

void ChangeValueDisplay<BluetoothMode>::start()
{
    Base::start();

    switch (const auto value = getValue())
    {
    case BluetoothMode::Off:    setSelectedIndex(0); break;
    case BluetoothMode::Master: setSelectedIndex(1); break;
    case BluetoothMode::Slave:  setSelectedIndex(2); break;
    default:
        //Serial.printf("Unknown BluetoothMode: %i\r\n", int(value));
        setSelectedIndex(4);
    }
}
#endif
}
