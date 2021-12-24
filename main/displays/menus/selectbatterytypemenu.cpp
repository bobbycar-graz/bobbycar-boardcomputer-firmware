#include "selectbatterytypemenu.h"

// local includes
#include "battery.h"
#include "globals.h"
#include "utilities/utils.h"
#include "displays/menus/batterymenu.h"

namespace {
class CurrentBatteryTypeText : public virtual espgui::TextInterface { public: std::string text() const override { return getBatteryCellTypeString(); } };

template<BatteryCellType T>
class BatterySelectTypeAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override { settings.battery.cellType = uint8_t(T); saveSettings(); }
};
} // namespace

using namespace espgui;

BatteryTypeMenu::BatteryTypeMenu()
{
    constructMenuItem<makeComponent<MenuItem, CurrentBatteryTypeText,             DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_TYPE_22P>,  BatterySelectTypeAction<BatteryCellType::_22P>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_TYPE_HG2>,  BatterySelectTypeAction<BatteryCellType::HG2>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_TYPE_MH1>,  BatterySelectTypeAction<BatteryCellType::MH1>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_TYPE_VTC5>, BatterySelectTypeAction<BatteryCellType::VTC5>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_TYPE_BAK_25R>, BatterySelectTypeAction<BatteryCellType::BAK_25R>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,              SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void BatteryTypeMenu::back()
{
    espgui::switchScreen<BatteryMenu>();
}
