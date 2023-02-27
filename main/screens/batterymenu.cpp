#include "batterymenu.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <fmt/core.h>
#include <menuitem.h>
#include <textwithvaluehelper.h>
#include <tftcolors.h>
#include <tftinterface.h>

// Local includes
#include "accessors/settingsaccessors.h"
#include "battery.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "icons/back.h"
#include "icons/graph.h"
#include "icons/settings.h"
#include "screens/batterygraphdisplay.h"
#include "screens/calibratevoltagedisplay.h"
#include "typesafeenumchangemenu.h"

namespace bobby {

namespace {
constexpr char TEXT_BATTERY[] = "Battery";
constexpr char TEXT_CELL_SERIES[] = "Cells (Series)";
constexpr char TEXT_CELL_PARALLEL[] = "Cells (Parallel)";
constexpr char TEXT_SELECT_CELL_TYPE[] = "Select Cell Type";
constexpr char TEXT_SHOW_BATTERY_GRAPH[] = "Battery Graph";
constexpr char TEXT_BATTERY_CALIBRATE[] = "Calibrate Voltages";
constexpr char TEXT_BATTERY_WHKM[] = "Wh per km";
constexpr char TEXT_BATTERY_APPLYCALIB[] = "Apply calibration";
constexpr char TEXT_VOLTAGECALIBRATION_RESET[] = "Reset calibration";
constexpr char TEXT_BACK[] = "Back";

class CurrentBatteryStatusText : public virtual espgui::TextInterface { public: std::string text() const override { return getBatteryPercentageString(); } };
class WhStatisticsText : public virtual espgui::TextInterface { public: std::string text() const override
    {
        if (battery::bootBatWh)
        {
            return fmt::format("&s&2{}Wh => &1{}Wh &6({})", (int)*battery::bootBatWh, (int)getRemainingWattHours(), (int)getRemainingWattHours() - (int)battery::bootBatWh.value());
        }
        return "";
    }
};

using BatteryCellSeriesChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_CELL_SERIES>,
    BatterySeriesCellsAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using BatteryCellParallelChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_CELL_PARALLEL>,
    BatteryParallelCellsAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using BatteryWHperKMChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint16_t>,
    espgui::StaticText<TEXT_BATTERY_WHKM>,
    BatteryWHperKMAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

using namespace espgui;

BatteryMenu::BatteryMenu()
{
    constructMenuItem<makeComponent<MenuItem, CurrentBatteryStatusText,                                                 DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                                                DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_CELL_SERIES, BatterySeriesCellsAccessor>,        PushScreenAction<BatteryCellSeriesChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_CELL_PARALLEL, BatteryParallelCellsAccessor>,    PushScreenAction<BatteryCellParallelChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BATTERY_WHKM, BatteryWHperKMAccessor>,           PushScreenAction<BatteryWHperKMChangeScreen>>>();
    constructMenuItem<PushScreenTypeSafeChangeMenuItem<BatteryCellType, TEXT_SELECT_CELL_TYPE>>(&configs.battery.cellType);
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SHOW_BATTERY_GRAPH>,                                      PushScreenAction<BatteryGraphDisplay>, StaticMenuItemIcon<&bobbyicons::graph>>>();
    constructMenuItem<makeComponent<MenuItem, WhStatisticsText,                                                         DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_CALIBRATE>,                                       PushScreenAction<CalibrateVoltageDisplay>, StaticMenuItemIcon<&bobbyicons::settings>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                                    PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string BatteryMenu::text() const
{
    return TEXT_BATTERY;
}

void BatteryMenu::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);
    m_batPercentBootLabel.start(tft);
    m_batPercentNowLabel.start(tft);

    m_doubleProgressBarBatPercentage.start(tft);
}

void BatteryMenu::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    if (m_fullRedraw)
    {
        m_fullRedraw = false;
        m_doubleProgressBarBatPercentage.start(tft);
        m_batPercentBootLabel.clear(tft, espgui::TFT_BLACK);
        m_batPercentNowLabel.clear(tft, espgui::TFT_BLACK);
    }

    if (const auto avgVoltage = controllers.getAvgVoltage(); avgVoltage)
    {
        const auto batPercent = getBatteryPercentage(*avgVoltage, BatteryCellType(configs.battery.cellType.value()));
        if (battery::bootBatPercentage)
        {
            m_doubleProgressBarBatPercentage.redraw(tft, batPercent, *battery::bootBatPercentage);

            m_batPercentNowLabel.redraw(tft, fmt::format("{:.2f} %", batPercent), espgui::TFT_DARKGREY, espgui::TFT_BLACK, 2);
            m_batPercentBootLabel.redraw(tft, fmt::format("{:.2f} %", *battery::bootBatPercentage), espgui::TFT_DARKGREY, espgui::TFT_BLACK, 2);
        }
    }
}

void BatteryMenu::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    case espgui::Up:
    case espgui::Down:
        m_fullRedraw = true;
        break;
    default:;
    }
}

void BatteryMenu::back()
{
    espgui::popScreen();
}

} // namespace bobby
