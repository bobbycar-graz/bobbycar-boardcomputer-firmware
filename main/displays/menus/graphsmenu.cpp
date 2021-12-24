#include "graphsmenu.h"

// 3rdparty lib includes
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "graphdisplay.h"
#include "splitgraphdisplay.h"

// local includes
#include "utilities/utils.h"
#include "statistics.h"
#include "displays/menus/mainmenu.h"

namespace {
using GasGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<1>,
    espgui::StaticText<TEXT_GAS>,
    espgui::SingleGraphAccessor<GasStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;
using BremsGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<1>,
    espgui::StaticText<TEXT_BREMS>,
    espgui::SingleGraphAccessor<BremsStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;
using PotisGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<2>,
    espgui::StaticText<TEXT_POTIS>,
    espgui::DualGraphAccessor<GasStatistics, BremsStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;
using PotisSplitGraphDisplay = espgui::makeComponent<
    espgui::SplitGraphDisplay<1, 1>,
    espgui::StaticText<TEXT_POTIS>,
    espgui::SingleTopGraphAccessor<GasStatistics>,
    espgui::SingleBottomGraphAccessor<BremsStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;

using AvgSpeedGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<1>,
    espgui::StaticText<TEXT_AVGSPEED>,
    espgui::SingleGraphAccessor<AvgSpeedStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;
using AvgSpeedKmhGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<1>,
    espgui::StaticText<TEXT_AVGSPEEDKMH>,
    espgui::SingleGraphAccessor<AvgSpeedKmhStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;

using SumCurrentGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<1>,
    espgui::StaticText<TEXT_SUMCURRENT>,
    espgui::SingleGraphAccessor<SumCurrentStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;

using FrontVoltageGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<1>,
    espgui::StaticText<TEXT_FRONTVOLTAGE>,
    espgui::SingleGraphAccessor<FrontVoltageStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;
using BackVoltageGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<1>,
    espgui::StaticText<TEXT_BACKVOLTAGE>,
    espgui::SingleGraphAccessor<BackVoltageStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;
using VoltagesGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<2>,
    espgui::StaticText<TEXT_VOLTAGES>,
    espgui::DualGraphAccessor<FrontVoltageStatistics, BackVoltageStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;
using VoltagesSplitGraphDisplay = espgui::makeComponent<
    espgui::SplitGraphDisplay<1, 1>,
    espgui::StaticText<TEXT_VOLTAGES>,
    espgui::SingleTopGraphAccessor<FrontVoltageStatistics>,
    espgui::SingleBottomGraphAccessor<BackVoltageStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;

#ifdef FEATURE_BMS
using BmsVoltageGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<1>,
    espgui::StaticText<TEXT_BMSVOLTAGE>,
    espgui::SingleGraphAccessor<BmsVoltageStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;
using BmsCurrentGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<1>,
    espgui::StaticText<TEXT_BMSCURRENT>,
    espgui::SingleGraphAccessor<BmsCurrentStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;
using BmsPowerGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<1>,
    espgui::StaticText<TEXT_BMSPOWER>,
    espgui::SingleGraphAccessor<BmsPowerStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;
using SumCurrentsComparisonGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<2>,
    espgui::StaticText<TEXT_SUMCURRENTSCOMPARISON>,
    DualGraphAccessor<SumCurrentStatistics, BmsCurrentStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;
#endif

class MotorCurrentsStatistics : public virtual espgui::GraphAccessorInterface<4>
{
    std::array<std::reference_wrapper<const statistics::ContainerType>, 4> getBuffers() const override
    {
        return {FrontLeftCurrentStatistics{}.getBuffer(), FrontRightCurrentStatistics{}.getBuffer(), BackLeftCurrentStatistics{}.getBuffer(), BackRightCurrentStatistics{}.getBuffer()};
    }
};
using MotorCurrentsGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<4>,
    espgui::StaticText<TEXT_MOTORCURRENTS>,
    MotorCurrentsStatistics,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;

using RssiGraphDisplay = espgui::makeComponent<
    espgui::GraphDisplay<1>,
    espgui::StaticText<TEXT_RSSI>,
    espgui::SingleGraphAccessor<RssiStatistics>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<GraphsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<GraphsMenu>>
>;
} // namespace

using namespace espgui;


GraphsMenu::GraphsMenu()
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
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_RSSI>,                  SwitchScreenAction<RssiGraphDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                  SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void GraphsMenu::back()
{
    switchScreen<MainMenu>();
}
