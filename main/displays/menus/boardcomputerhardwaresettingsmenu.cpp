#include "boardcomputerhardwaresettingsmenu.h"

// 3rdparty lib includes
#include <fmt/core.h>
#include <changevaluedisplay.h>
#include <actions/dummyaction.h>
#include <actions/switchscreenaction.h>
#include <actions/toggleboolaction.h>
#include <icons/back.h>
#include <checkboxicon.h>

// local includes
#include "utilities/utils.h"
#include "icons/lock.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/lockscreensettingsmenu.h"
#include "displays/calibratedisplay.h"
#include "displays/menus/timersmenu.h"
#include "displays/menus/settingsmenu.h"

namespace {
struct GasText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("{}: {}: {}",
                           "gas",
                           raw_gas ? std::to_string(*raw_gas) : "?",
                           gas ? fmt::format("{:.02f}", *gas) : "?");
    }
};
struct BremsText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("{}: {}: {}",
                           "brems",
                           raw_brems ? std::to_string(*raw_brems) : "?",
                           brems ? fmt::format("{:.02f}", *brems) : "?");
    }
};

using SampleCountChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SAMPLECOUNT>,
    SampleCountAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GasMinChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_GASMIN>,
    GasMinAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GasMaxChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_GASMAX>,
    GasMaxAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using BremsMinChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_BREMSMIN>,
    BremsMinAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using BremsMaxChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_BREMSMAX>,
    BremsMaxAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;

#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW) || defined(FEATURE_DPAD_5WIRESW_2OUT) || defined (FEATURE_DPAD_6WIRESW)
using DPadDebounceChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_DPADDEBOUNCE>,
    DPadDebounceAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
#endif

#ifdef FEATURE_GAMETRAK
struct GametrakXText : public virtual TextInterface {
public:
    std::string text() const override { return fmt::format("gametrakX: {}: {:.02f}", raw_gametrakX, gametrakX); }
};
struct GametrakYText : public virtual TextInterface {
public:
    std::string text() const override { return fmt::format("gametrakY: {}: {:.02f}", raw_gametrakY, gametrakY); }
};
struct GametrakDistText : public virtual TextInterface {
public:
    std::string text() const override { return fmt::format("gametrakDist: {}: {:.02f}", raw_gametrakDist, gametrakDist); }
};

using GametrakXMinChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKXMIN>,
    GametrakXMinAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GametrakXMaxChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKXMAX>,
    GametrakXMaxAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GametrakYMinChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKYMIN>,
    GametrakYMinAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GametrakYMaxChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKYMAX>,
    GametrakYMaxAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GametrakDistMinChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKDISTMIN>,
    GametrakDistMinAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GametrakDistMaxChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKDISTMAX>,
    GametrakDistMaxAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
#endif
} // namespace

using namespace espgui;

BoardcomputerHardwareSettingsMenu::BoardcomputerHardwareSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LOCKSCREENSETTINGS>,                          SwitchScreenAction<LockscreenSettingsMenu>, StaticMenuItemIcon<&bobbyicons::lock>>>();
    constructMenuItem<makeComponent<MenuItem, GasText,                                                      DisabledColor, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BremsText,                                                    DisabledColor, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CALIBRATE>,                                   SwitchScreenAction<CalibrateDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SAMPLECOUNT, SampleCountAccessor>,   SwitchScreenAction<SampleCountChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_GASMIN, GasMinAccessor>,             SwitchScreenAction<GasMinChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_GASMAX, GasMaxAccessor>,             SwitchScreenAction<GasMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BREMSMIN, BremsMinAccessor>,         SwitchScreenAction<BremsMinChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BREMSMAX, BremsMaxAccessor>,         SwitchScreenAction<BremsMaxChangeScreen>>>();
#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW) || defined(FEATURE_DPAD_5WIRESW_2OUT) || defined (FEATURE_DPAD_6WIRESW)
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_DPADDEBOUNCE, DPadDebounceAccessor>, SwitchScreenAction<DPadDebounceChangeScreen>>>();
#endif
#ifdef FEATURE_GAMETRAK
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                                    DummyAction>,
    constructMenuItem<makeComponent<MenuItem, GametrakXText,                                                DisabledColor, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, GametrakYText,                                                DisabledColor, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, GametrakDistText,                                             DisabledColor, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GAMETRAKCALIBRATE>,                           SwitchScreenAction<GametrakCalibrateDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKXMIN>,                             SwitchScreenAction<GametrakXMinChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKXMAX>,                             SwitchScreenAction<GametrakXMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKYMIN>,                             SwitchScreenAction<GametrakYMinChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKYMAX>,                             SwitchScreenAction<GametrakYMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKDISTMIN>,                          SwitchScreenAction<GametrakDistMinChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKDISTMAX>,                          SwitchScreenAction<GametrakDistMaxChangeScreen>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                                    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TIMERS>,                                      SwitchScreenAction<TimersMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                        SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&icons::back>>>();
}

void BoardcomputerHardwareSettingsMenu::back()
{
    switchScreen<SettingsMenu>();
}
