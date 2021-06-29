#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"
#include "displays/graphdisplay.h"
#include "displays/splitgraphdisplay.h"
#include "statistics.h"

// forward declares
namespace {
class MainMenu;
class GraphsMenu;
} // namespace

namespace {
using GasGraphDisplay = makeComponent<
    GraphDisplay<1>,
    StaticText<TEXT_GAS>,
    SingleGraphAccessor<GasStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;
using BremsGraphDisplay = makeComponent<
    GraphDisplay<1>,
    StaticText<TEXT_BREMS>,
    SingleGraphAccessor<BremsStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;
using PotisGraphDisplay = makeComponent<
    GraphDisplay<2>,
    StaticText<TEXT_POTIS>,
    DualGraphAccessor<GasStatistics, BremsStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;
using PotisSplitGraphDisplay = makeComponent<
    SplitGraphDisplay<1, 1>,
    StaticText<TEXT_POTIS>,
    SingleTopGraphAccessor<GasStatistics>,
    SingleBottomGraphAccessor<BremsStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;

using AvgSpeedGraphDisplay = makeComponent<
    GraphDisplay<1>,
    StaticText<TEXT_AVGSPEED>,
    SingleGraphAccessor<AvgSpeedStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;
using AvgSpeedKmhGraphDisplay = makeComponent<
    GraphDisplay<1>,
    StaticText<TEXT_AVGSPEEDKMH>,
    SingleGraphAccessor<AvgSpeedKmhStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;

using SumCurrentGraphDisplay = makeComponent<
    GraphDisplay<1>,
    StaticText<TEXT_SUMCURRENT>,
    SingleGraphAccessor<SumCurrentStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;

using FrontVoltageGraphDisplay = makeComponent<
    GraphDisplay<1>,
    StaticText<TEXT_FRONTVOLTAGE>,
    SingleGraphAccessor<FrontVoltageStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;
using BackVoltageGraphDisplay = makeComponent<
    GraphDisplay<1>,
    StaticText<TEXT_BACKVOLTAGE>,
    SingleGraphAccessor<BackVoltageStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;
using VoltagesGraphDisplay = makeComponent<
    GraphDisplay<2>,
    StaticText<TEXT_VOLTAGES>,
    DualGraphAccessor<FrontVoltageStatistics, BackVoltageStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;
using VoltagesSplitGraphDisplay = makeComponent<
    SplitGraphDisplay<1, 1>,
    StaticText<TEXT_VOLTAGES>,
    SingleTopGraphAccessor<FrontVoltageStatistics>,
    SingleBottomGraphAccessor<BackVoltageStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;

#ifdef FEATURE_BMS
using BmsVoltageGraphDisplay = makeComponent<
    GraphDisplay<1>,
    StaticText<TEXT_BMSVOLTAGE>,
    SingleGraphAccessor<BmsVoltageStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;
using BmsCurrentGraphDisplay = makeComponent<
    GraphDisplay<1>,
    StaticText<TEXT_BMSCURRENT>,
    SingleGraphAccessor<BmsCurrentStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;
using BmsPowerGraphDisplay = makeComponent<
    GraphDisplay<1>,
    StaticText<TEXT_BMSPOWER>,
    SingleGraphAccessor<BmsPowerStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;
using SumCurrentsComparisonGraphDisplay = makeComponent<
    GraphDisplay<2>,
    StaticText<TEXT_SUMCURRENTSCOMPARISON>,
    DualGraphAccessor<SumCurrentStatistics, BmsCurrentStatistics>,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;
#endif

class MotorCurrentsStatistics : public virtual GraphAccessorInterface<4>
{
    std::array<std::reference_wrapper<const statistics::ContainerType>, 4> getBuffers() const override
    {
        return {FrontLeftCurrentStatistics{}.getBuffer(), FrontRightCurrentStatistics{}.getBuffer(), BackLeftCurrentStatistics{}.getBuffer(), BackRightCurrentStatistics{}.getBuffer()};
    }
};
using MotorCurrentsGraphDisplay = makeComponent<
    GraphDisplay<4>,
    StaticText<TEXT_MOTORCURRENTS>,
    MotorCurrentsStatistics,
    ConfirmActionInterface<SwitchScreenAction<GraphsMenu>>,
    BackActionInterface<SwitchScreenAction<GraphsMenu>>
>;

class GraphsMenu :
    public MenuDisplay,
    public StaticText<TEXT_GRAPHS>,
    public BackActionInterface<SwitchScreenAction<MainMenu>>
{
public:
    GraphsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GAS>,                   SwitchScreenAction<GasGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BREMS>,                 SwitchScreenAction<BremsGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_POTIS>,                 SwitchScreenAction<PotisGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_POTIS>,                 SwitchScreenAction<PotisSplitGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_AVGSPEED>,              SwitchScreenAction<AvgSpeedGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_AVGSPEEDKMH>,           SwitchScreenAction<AvgSpeedKmhGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SUMCURRENT>,            SwitchScreenAction<SumCurrentGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTVOLTAGE>,          SwitchScreenAction<FrontVoltageGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKVOLTAGE>,           SwitchScreenAction<BackVoltageGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_VOLTAGES>,              SwitchScreenAction<VoltagesGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_VOLTAGES>,              SwitchScreenAction<VoltagesSplitGraphDisplay>>>();
#ifdef FEATURE_BMS
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BMSVOLTAGE>,            SwitchScreenAction<BmsVoltageGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BMSCURRENT>,            SwitchScreenAction<BmsCurrentGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BMSPOWER>,              SwitchScreenAction<BmsPowerGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SUMCURRENTSCOMPARISON>, SwitchScreenAction<SumCurrentsComparisonGraphDisplay>>>();
#endif
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOTORCURRENTS>,         SwitchScreenAction<MotorCurrentsGraphDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                  SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
} // namespace
