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
class ChangeValueDisplay<ControlMode> :
    public MenuDisplay,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_OPENMODE>, DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_VOLTAGE>,  DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_SPEED>,    DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_TORQUE>,   DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,     DummyAction, StaticMenuItemIcon<&icons::back>>
    >,
    public virtual AccessorInterface<ControlMode>,
    public virtual ActionInterface
{
    using Base = MenuDisplay;

public:
    void start() override;

    void itemPressed(int index) override;
};

void ChangeValueDisplay<ControlMode>::start()
{
    Base::start();

    if (getValue() == ControlMode::OpenMode)
        setSelectedIndex(0);
    else if (getValue() == ControlMode::Voltage)
        setSelectedIndex(1);
    else if (getValue() == ControlMode::Speed)
        setSelectedIndex(2);
    else if (getValue() == ControlMode::Torque)
        setSelectedIndex(3);
    else
    {
        Serial.printf("Unknown ControlMode: %i", int(getValue()));
        setSelectedIndex(4);
    }
}

void ChangeValueDisplay<ControlMode>::itemPressed(int index)
{
    switch (index)
    {
    case 0: setValue(ControlMode::OpenMode); break;
    case 1: setValue(ControlMode::Voltage); break;
    case 2: setValue(ControlMode::Speed); break;
    case 3: setValue(ControlMode::Torque); break;
    }

    triggered();
}
}
#endif