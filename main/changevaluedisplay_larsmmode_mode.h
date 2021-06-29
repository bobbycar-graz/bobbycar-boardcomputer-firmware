#pragma once

#include "changevaluedisplay.h"
#include "menudisplay.h"
#include "utils.h"
#include "actions/setvalueaction.h"
#include "actions/backproxyaction.h"
#include "icons/back.h"
#include "texts.h"
#include "modes/larsmmode.h"

namespace {
template<>
class ChangeValueDisplay<LarsmModeMode> :
    public MenuDisplay,
    public virtual AccessorInterface<LarsmModeMode>,
    public virtual ActionInterface
{
    using Base = MenuDisplay;

public:
    ChangeValueDisplay();

    void start() override;
};

ChangeValueDisplay<LarsmModeMode>::ChangeValueDisplay()
{
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<LarsmModeMode>, StaticText<TEXT_LARSMMODE1>>>(LarsmModeMode::Mode1, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<LarsmModeMode>, StaticText<TEXT_LARSMMODE2>>>(LarsmModeMode::Mode2, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<LarsmModeMode>, StaticText<TEXT_LARSMMODE3>>>(LarsmModeMode::Mode3, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<LarsmModeMode>, StaticText<TEXT_LARSMMODE4>>>(LarsmModeMode::Mode4, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&icons::back>>>(*this);
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
        //Serial.printf("Unknown LarsmModeMode: %i\r\n", int(value));
        setSelectedIndex(4);
    }
}
}
