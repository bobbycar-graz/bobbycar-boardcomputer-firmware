#include "boardcomputerhardwaresettingsmenu.h"

using namespace espgui;

struct GasText : public virtual TextInterface {
public:
    std::string text() const override
    {
        return fmt::format("{}: {}: {}",
                           "gas",
                           raw_gas ? std::to_string(*raw_gas) : "?",
                           gas ? fmt::format("{:.02f}", *gas) : "?");
    }
};
struct BremsText : public virtual TextInterface {
public:
    std::string text() const override
    {
        return fmt::format("{}: {}: {}",
                           "brems",
                           raw_brems ? std::to_string(*raw_brems) : "?",
                           brems ? fmt::format("{:.02f}", *brems) : "?");
    }
};

using SampleCountChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SAMPLECOUNT>,
    SampleCountAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GasMinChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_GASMIN>,
    GasMinAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GasMaxChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_GASMAX>,
    GasMaxAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using BremsMinChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_BREMSMIN>,
    BremsMinAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using BremsMaxChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_BREMSMAX>,
    BremsMaxAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;

#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW) || defined(FEATURE_DPAD_5WIRESW_2OUT) || defined (FEATURE_DPAD_6WIRESW)
using DPadDebounceChangeScreen = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_DPADDEBOUNCE>,
    DPadDebounceAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
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

using GametrakXMinChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETGAMETRAKXMIN>,
    GametrakXMinAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GametrakXMaxChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETGAMETRAKXMAX>,
    GametrakXMaxAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GametrakYMinChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETGAMETRAKYMIN>,
    GametrakYMinAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GametrakYMaxChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETGAMETRAKYMAX>,
    GametrakYMaxAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GametrakDistMinChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETGAMETRAKDISTMIN>,
    GametrakDistMinAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GametrakDistMaxChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETGAMETRAKDISTMAX>,
    GametrakDistMaxAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
#endif

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
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                        SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}
