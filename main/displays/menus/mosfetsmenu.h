#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "menuitem.h"
#include "actions/toggleboolaction.h"
#include "actions/switchscreenaction.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "texts.h"
#include "types.h"

// forward declares
namespace {
class MainMenu;
} // namespace

namespace {
#ifdef FEATURE_MOSFETS
template<pin_t PIN>
class GPIOAccessor : public virtual AccessorInterface<bool>
{
public:
    bool getValue() const override { return digitalRead(PIN); }
    void setValue(bool value) override { digitalWrite(PIN, value ? HIGH : LOW); }
};

using Mosfet0Accessor = GPIOAccessor<PINS_MOSFET0>;
using Mosfet1Accessor = GPIOAccessor<PINS_MOSFET1>;
using Mosfet2Accessor = GPIOAccessor<PINS_MOSFET2>;

class MosfetsMenu :
    public MenuDisplay,
    public StaticText<TEXT_MOSFETS>,
    public BackActionInterface<SwitchScreenAction<MainMenu>>
{
public:
    MosfetsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOSFET0>, ToggleBoolAction, CheckboxIcon, Mosfet0Accessor>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOSFET1>, ToggleBoolAction, CheckboxIcon, Mosfet1Accessor>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOSFET2>, ToggleBoolAction, CheckboxIcon, Mosfet2Accessor>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,    SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
#endif
} // namespace
