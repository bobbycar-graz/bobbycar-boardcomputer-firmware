#pragma once

#include "changevaluedisplay.h"
#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "actions/dummyaction.h"
#include "icons/back.h"
#include "texts.h"
#include "modes/larsmmode.h"

#ifdef GLUMP_CONTROLLER
namespace {
template<>
class ChangeValueDisplay<LarsmModeMode> :
    public MenuDisplay,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_LARSMMODE1>, DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_LARSMMODE2>, DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_LARSMMODE3>, DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_LARSMMODE4>, DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,       DummyAction, StaticMenuItemIcon<&icons::back>>
    >,
    public virtual AccessorInterface<LarsmModeMode>,
    public virtual ActionInterface
{
    using Base = MenuDisplay;

public:
    void start() override;

    void itemPressed(int index) override;
};

void ChangeValueDisplay<LarsmModeMode>::start()
{
    Base::start();

    if (getValue() == LarsmModeMode::Mode1)
        setSelectedIndex(0);
    else if (getValue() == LarsmModeMode::Mode2)
        setSelectedIndex(1);
    else if (getValue() == LarsmModeMode::Mode3)
        setSelectedIndex(2);
    else if (getValue() == LarsmModeMode::Mode4)
        setSelectedIndex(3);
    else
    {
        Serial.printf("Unknown LarsmModeMode: %i", int(getValue()));
        setSelectedIndex(4);
    }
}

void ChangeValueDisplay<LarsmModeMode>::itemPressed(int index)
{
    switch (index)
    {
    case 0: setValue(LarsmModeMode::Mode1); break;
    case 1: setValue(LarsmModeMode::Mode2); break;
    case 2: setValue(LarsmModeMode::Mode3); break;
    case 3: setValue(LarsmModeMode::Mode4); break;
    }

    triggered();
}
}
#endif