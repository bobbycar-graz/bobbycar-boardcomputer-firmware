#include "batterymenu.h"

using namespace espgui;

using BatteryCellSeriesChangeScreen = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_CELL_SERIES>,
    BatterySeriesCellsAccessor,
    BackActionInterface<SwitchScreenAction<BatteryMenu>>,
    SwitchScreenAction<BatteryMenu>
>;

using BatteryCellParallelChangeScreen = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_CELL_PARALLEL>,
    BatteryParallelCellsAccessor,
    BackActionInterface<SwitchScreenAction<BatteryMenu>>,
    SwitchScreenAction<BatteryMenu>
>;

using BatteryWHperKMChangeScreen = makeComponent<
    ChangeValueDisplay<uint16_t>,
    StaticText<TEXT_BATTERY_WHKM>,
    BatteryWHperKMAccessor,
    BackActionInterface<SwitchScreenAction<BatteryMenu>>,
    SwitchScreenAction<BatteryMenu>
>;

BatteryMenu::BatteryMenu()
{
    constructMenuItem<makeComponent<MenuItem, currentBatteryStatus,                                                     DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                                                DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_CELL_SERIES, BatterySeriesCellsAccessor>,        SwitchScreenAction<BatteryCellSeriesChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_CELL_PARALLEL, BatteryParallelCellsAccessor>,    SwitchScreenAction<BatteryCellParallelChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BATTERY_WHKM, BatteryWHperKMAccessor>,           SwitchScreenAction<BatteryWHperKMChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECT_CELL_TYPE>,                                        SwitchScreenAction<BatteryTypeMenu>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                                                DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_CALIBRATE>,                                       SwitchScreenAction<CalibrateVoltageDisplay>, StaticMenuItemIcon<&bobbyicons::settings>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                                    SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}
