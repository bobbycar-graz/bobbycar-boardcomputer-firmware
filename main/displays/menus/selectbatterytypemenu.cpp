#include "selectbatterytypemenu.h"

// 3rdparty lib includes
#include <menuitem.h>
#include <icons/back.h>
#include <actions/popscreenaction.h>
#include <actioninterface.h>
#include <actions/dummyaction.h>

// local includes
#include "battery.h"
#include "newsettings.h"
#include "utils.h"
#include "displays/menus/batterymenu.h"
#include "batterymenu.h"
#include "displays/menus/mainmenu.h"

namespace {
constexpr char TEXT_CELL_SERIES[] = "Cells (Series)";
constexpr char TEXT_CELL_PARALLEL[] = "Cells (Parallel)";
constexpr char TEXT_SELECT_CELL_TYPE[] = "Select Cell Type";
constexpr char TEXT_CELL_TYPE[] = "Cell Type";
constexpr char TEXT_BATTERY_CALIBRATE[] = "Calibrate Voltages";
constexpr char TEXT_BATTERY_TYPE_22P[] = "22P cells";
constexpr char TEXT_BATTERY_TYPE_HG2[] = "HG2 cells";
constexpr char TEXT_BATTERY_TYPE_MH1[] = "MH1 cells";
constexpr char TEXT_BATTERY_TYPE_VTC5[] = "VTC5 cells";
constexpr char TEXT_BATTERY_TYPE_BAK_25R[] = "BAK / 25R cells";
constexpr char TEXT_BATTERY_WHKM[] = "Wh per km";
constexpr char TEXT_BATTERY_APPLYCALIB[] = "Apply calibration";
constexpr char TEXT_VOLTAGECALIBRATION_RESET[] = "Reset calibration";
constexpr char TEXT_BACK[] = "Back";

class CurrentBatteryTypeText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return getBatteryCellTypeString();
    }
};

template<BatteryCellType T>
class BatterySelectTypeAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override { configs.write_config(configs.battery.cellType, uint8_t(T)); }
};
} // namespace

BatteryTypeMenu::BatteryTypeMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, CurrentBatteryTypeText,             DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_TYPE_22P>,  BatterySelectTypeAction<BatteryCellType::_22P>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_TYPE_HG2>,  BatterySelectTypeAction<BatteryCellType::HG2>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_TYPE_MH1>,  BatterySelectTypeAction<BatteryCellType::MH1>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_TYPE_VTC5>, BatterySelectTypeAction<BatteryCellType::VTC5>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_TYPE_BAK_25R>, BatterySelectTypeAction<BatteryCellType::BAK_25R>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,              PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string BatteryTypeMenu::text() const
{
    return TEXT_SELECT_CELL_TYPE;
}

void BatteryTypeMenu::back()
{
    espgui::popScreen();
}
