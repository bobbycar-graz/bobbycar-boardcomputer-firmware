#pragma once

#include "changevaluedisplay.h"
#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "actions/dummyaction.h"
#include "icons/back.h"
#include "texts.h"

#ifdef GLUMP_CONTROLLER
namespace {
template<>
class ChangeValueDisplay<ControlType> :
    public MenuDisplay,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_COMMUTATION>,          DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_SINUSOIDAL>,           DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_FIELDORIENTEDCONTROL>, DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,                 DummyAction, StaticMenuItemIcon<&icons::back>>
    >,
    public virtual AccessorInterface<ControlType>,
    public virtual ActionInterface
{
    using Base = MenuDisplay;

public:
    void start() override;

    void itemPressed(int index) override;
};

void ChangeValueDisplay<ControlType>::start()
{
    Base::start();

    if (getValue() == ControlType::Commutation)
        setSelectedIndex(0);
    else if (getValue() == ControlType::Sinusoidal)
        setSelectedIndex(1);
    else if (getValue() == ControlType::FieldOrientedControl)
        setSelectedIndex(2);
    else
    {
        Serial.printf("Unknown ControlType: %i", int(getValue()));
        setSelectedIndex(3);
    }
}

void ChangeValueDisplay<ControlType>::itemPressed(int index)
{
    switch (index)
    {
    case 0: setValue(ControlType::Commutation); break;
    case 1: setValue(ControlType::Sinusoidal); break;
    case 2: setValue(ControlType::FieldOrientedControl); break;
    }

    triggered();
}
}
#endif