#include "batterymenu.h"

// 3rdparty lib includes
#include <menuitem.h>
#include <icons/back.h>
#include <actions/dummyaction.h>
#include <actions/pushscreenaction.h>
#include <actions/popscreenaction.h>
#include <changevaluedisplay.h>
#include <textwithvaluehelper.h>
#include <fmt/core.h>

#include <tftinstance.h>

// Local includes
#include "utils.h"
#include "icons/settings.h"
#include "battery.h"
#include "selectbatterytypemenu.h"
#include "displays/bobbychangevaluedisplay.h"
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
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECT_CELL_TYPE>,                                        PushScreenAction<BatteryTypeMenu>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                                                DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WhStatisticsText,                                                         DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_CALIBRATE>,                                       PushScreenAction<CalibrateVoltageDisplay>, StaticMenuItemIcon<&bobbyicons::settings>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                                    PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string BatteryMenu::text() const
{
    return TEXT_BATTERY;
}

void BatteryMenu::initScreen()
{
    Base::initScreen();
    m_batPercentBootLabel.start();
    m_batPercentNowLabel.start();
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
    {
        m_doubleProgressBarBatPercentage.redraw(batPercent, *battery::bootBatPercentage);

        tft.setTextFont(2);
        tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
        m_batPercentNowLabel.redraw(fmt::format("{:.2f} %", batPercent));
        m_batPercentBootLabel.redraw(fmt::format("{:.2f} %", *battery::bootBatPercentage));
        tft.setTextFont(4);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
}

void BatteryMenu::back()
{
    espgui::popScreen();
}
