#include "mosfetsmenu.h"

#ifdef FEATURE_MOSFETS
// 3rdparty lib includes
#include "accessorinterface.h"
#include "actions/popscreenaction.h"
#include "icons/back.h"

// local includes
#include "bobbycheckbox.h"
#include "types.h"

namespace {
constexpr char TEXT_MOSFETS[] = "Mosfets";
constexpr char TEXT_MOSFET0[] = "Mosfet0";
constexpr char TEXT_MOSFET1[] = "Mosfet1";
constexpr char TEXT_MOSFET2[] = "Mosfet2";
constexpr char TEXT_BACK[] = "Back";

template<pin_t PIN>
class GPIOAccessor : public virtual espgui::AccessorInterface<bool>
{
public:
    bool getValue() const override { return digitalRead(PIN); }
    espgui::AccessorInterface<bool>::setter_result_t setValue(bool value) override { digitalWrite(PIN, value ? HIGH : LOW); return {}; }
};

using Mosfet0Accessor = GPIOAccessor<PINS_MOSFET0>;
using Mosfet1Accessor = GPIOAccessor<PINS_MOSFET1>;
using Mosfet2Accessor = GPIOAccessor<PINS_MOSFET2>;
} // namespace

MosfetsMenu::MosfetsMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOSFET0>, BobbyCheckbox, Mosfet0Accessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOSFET1>, BobbyCheckbox, Mosfet1Accessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOSFET2>, BobbyCheckbox, Mosfet2Accessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,    PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string MosfetsMenu::text() const
{
    return TEXT_MOSFETS;
}

void MosfetsMenu::back()
{
    espgui::popScreen();
}

#endif
