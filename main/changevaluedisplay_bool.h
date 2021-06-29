#pragma once

#include "changevaluedisplay.h"
#include "menudisplay.h"
#include "actioninterface.h"
#include "utils.h"
#include "actions/setvalueaction.h"
#include "actions/backproxyaction.h"
#include "icons/back.h"
#include "texts.h"

namespace {
template<>
class ChangeValueDisplay<bool> :
    public MenuDisplay,
    public virtual AccessorInterface<bool>,
    public virtual ActionInterface
{
    using Base = MenuDisplay;

public:
    ChangeValueDisplay();

    void start() override;
};

ChangeValueDisplay<bool>::ChangeValueDisplay()
{
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<bool>, StaticText<TEXT_TRUE>>>(true, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<bool>, StaticText<TEXT_FALSE>>>(false, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&icons::back>>>(*this);
}

void ChangeValueDisplay<bool>::start()
{
    Base::start();

    switch (getValue())
    {
    case true:  setSelectedIndex(0); break;
    case false: setSelectedIndex(1); break;
    }
}
}
