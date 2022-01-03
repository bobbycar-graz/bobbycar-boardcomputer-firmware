#include "batterymenu.h"

// 3rdparty lib includes
#include <menuitem.h>
#include <icons/back.h>
#include <actions/dummyaction.h>
#include <actions/switchscreenaction.h>
#include <changevaluedisplay.h>
#include <textwithvaluehelper.h>
#include <fmt/core.h>

// Local includes
#include "utils.h"
#include "icons/settings.h"
#include "battery.h"
#include "selectbatterytypemenu.h"
#include "displays/bobbychangevaluedisplay.h"
#include "mainmenu.h"
#include "displays/calibratevoltagedisplay.h"
#include "accessors/settingsaccessors.h"
#include "battery.h"

namespace {
constexpr char TEXT_BATTERY[] = "Battery";
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

class CurrentBatteryStatusText : public virtual espgui::TextInterface { public: std::string text() const override { return getBatteryPercentageString(); } };

using BatteryCellSeriesChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_CELL_SERIES>,
    BatterySeriesCellsAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BatteryMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BatteryMenu>>
>;

using BatteryCellParallelChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_CELL_PARALLEL>,
    BatteryParallelCellsAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BatteryMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BatteryMenu>>
>;

using BatteryWHperKMChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint16_t>,
    espgui::StaticText<TEXT_BATTERY_WHKM>,
    BatteryWHperKMAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BatteryMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BatteryMenu>>
>;
} // namespace

BatteryMenu::BatteryMenu()
{
    using namespace espgui;

    constructMenuItem<makeComponent<MenuItem, CurrentBatteryStatusText,                                                 DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                                                DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_CELL_SERIES, BatterySeriesCellsAccessor>,        SwitchScreenAction<BatteryCellSeriesChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_CELL_PARALLEL, BatteryParallelCellsAccessor>,    SwitchScreenAction<BatteryCellParallelChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BATTERY_WHKM, BatteryWHperKMAccessor>,           SwitchScreenAction<BatteryWHperKMChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECT_CELL_TYPE>,                                        SwitchScreenAction<BatteryTypeMenu>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                                                DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_CALIBRATE>,                                       SwitchScreenAction<CalibrateVoltageDisplay>, StaticMenuItemIcon<&bobbyicons::settings>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                                    SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string BatteryMenu::text() const
{
    return TEXT_BATTERY;
}

void BatteryMenu::start()
{
    Base::start();
    m_doubleProgressBarBatPercentage.start();
}

void BatteryMenu::redraw()
{
    Base::redraw();

    float avgVoltage = 0;
    for (auto &controller : controllers)
    {
        avgVoltage += controller.getCalibratedVoltage();
    }
    avgVoltage = avgVoltage / controllers.size();

    const auto batPercent = getBatteryPercentage(avgVoltage, BatteryCellType(configs.battery.cellType.value));
    if (battery::bootBatPercentage)
        m_doubleProgressBarBatPercentage.redraw(batPercent, *battery::bootBatPercentage);
}

void BatteryMenu::back()
{
    espgui::switchScreen<MainMenu>();
}
