#include "boardcomputerhardwaresettingsmenu.h"

// 3rdparty lib includes
#include <fmt/core.h>
#include <changevaluedisplay.h>
#include <actions/dummyaction.h>
#include <actions/switchscreenaction.h>
#include <icons/back.h>
#include <textwithvaluehelper.h>

// local includes
#include "displays/bobbychangevaluedisplay.h"
#include "utils.h"
#include "icons/lock.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/lockscreensettingsmenu.h"
#include "displays/potiscalibratedisplay.h"
#include "displays/menus/timersmenu.h"
#include "displays/menus/settingsmenu.h"
#include "displays/buttoncalibratedisplay.h"
#include "displays/menus/extrabuttoncalibratemenu.h"
#include "displays/menus/setupquickactionsmenu.h"
#ifdef FEATURE_JOYSTICK
#include "displays/joystickdebugdisplay.h"
#endif

namespace {
constexpr char TEXT_BOARDCOMPUTERHARDWARESETTINGS[] = "Boardcomputer H/W settings";
constexpr char TEXT_BUTTONCALIBRATE[] = "Button Calibrate";
constexpr char TEXT_EXTRABUTTONCALIBRATE[] = "Cal other Buttons";
constexpr char TEXT_QUICKACTIONS[] = "Quick Actions";
constexpr char TEXT_LOCKSCREENSETTINGS[] = "Lockscreen Settings";
constexpr char TEXT_POTISCALIBRATE[] = "Potis Calibrate";
#ifdef FEATURE_JOYSTICK
constexpr char TEXT_JOYSTICK[] = "Debug Joystick";
#endif
constexpr char TEXT_SAMPLECOUNT[] = "sampleCount";
constexpr char TEXT_GASMIN[] = "gasMin";
constexpr char TEXT_GASMAX[] = "gasMax";
constexpr char TEXT_BREMSMIN[] = "bremsMin";
constexpr char TEXT_BREMSMAX[] = "bremsMax";
constexpr char TEXT_DPADDEBOUNCE[] = "dpadDebounce";
constexpr char TEXT_BUTTONDELAY[] = "button read delay";
constexpr char TEXT_GAMETRAKCALIBRATE[] = "Gametrak calibrate";
constexpr char TEXT_SETGAMETRAKXMIN[] = "Set gametrakXMin";
constexpr char TEXT_SETGAMETRAKXMAX[] = "Set gametrakXMax";
constexpr char TEXT_SETGAMETRAKYMIN[] = "Set gametrakYMin";
constexpr char TEXT_SETGAMETRAKYMAX[] = "Set gametrakYMax";
constexpr char TEXT_SETGAMETRAKDISTMIN[] = "Set gametrakDistMin";
constexpr char TEXT_SETGAMETRAKDISTMAX[] = "Set gametrakDistMax";
constexpr char TEXT_TIMERS[] = "Timers";
constexpr char TEXT_BACK[] = "Back";

struct GasText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("gas: {}: {}",
                           raw_gas ? std::to_string(*raw_gas) : "?",
                           gas ? fmt::format("{:.02f}", *gas) : "?");
    }
};
struct BremsText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("brems: {}: {}",
                           raw_brems ? std::to_string(*raw_brems) : "?",
                           brems ? fmt::format("{:.02f}", *brems) : "?");
    }
};

using SampleCountChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SAMPLECOUNT>,
    SampleCountAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
>;
using GasMinChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_GASMIN>,
    GasMinAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
>;
using GasMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_GASMAX>,
    GasMaxAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
>;
using BremsMinChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_BREMSMIN>,
    BremsMinAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
>;
using BremsMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_BREMSMAX>,
    BremsMaxAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
>;


#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW) || defined(FEATURE_DPAD_5WIRESW_2OUT) || defined (FEATURE_DPAD_6WIRESW)
using DPadDebounceChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_DPADDEBOUNCE>,
    DPadDebounceAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
>;
using ButtonDelayChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint16_t>,
    espgui::StaticText<TEXT_BUTTONDELAY>,
    ButtonDelayAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
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
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKXMIN>,
    GametrakXMinAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
>;
using GametrakXMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKXMAX>,
    GametrakXMaxAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
>;
using GametrakYMinChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKYMIN>,
    GametrakYMinAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
>;
using GametrakYMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKYMAX>,
    GametrakYMaxAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
>;
using GametrakDistMinChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKDISTMIN>,
    GametrakDistMinAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
>;
using GametrakDistMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKDISTMAX>,
    GametrakDistMaxAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
>;
#endif
} // namespace

using namespace espgui;

BoardcomputerHardwareSettingsMenu::BoardcomputerHardwareSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LOCKSCREENSETTINGS>,                          SwitchScreenAction<LockscreenSettingsMenu>, StaticMenuItemIcon<&bobbyicons::lock>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BUTTONCALIBRATE>,                             SwitchScreenAction<ButtonCalibrateDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_EXTRABUTTONCALIBRATE>,                        SwitchScreenAction<ExtraButtonCalibrateMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_QUICKACTIONS>,                                SwitchScreenAction<SetupQuickActionsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, GasText,                                                      DisabledColor, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BremsText,                                                    DisabledColor, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_POTISCALIBRATE>,                              SwitchScreenAction<PotisCalibrateDisplay>>>();
#ifdef FEATURE_JOYSTICK
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_JOYSTICK>,                                    SwitchScreenAction<JoystickDebugDisplay>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SAMPLECOUNT, SampleCountAccessor>,   SwitchScreenAction<SampleCountChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_GASMIN, GasMinAccessor>,             SwitchScreenAction<GasMinChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_GASMAX, GasMaxAccessor>,             SwitchScreenAction<GasMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BREMSMIN, BremsMinAccessor>,         SwitchScreenAction<BremsMinChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BREMSMAX, BremsMaxAccessor>,         SwitchScreenAction<BremsMaxChangeScreen>>>();
#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW) || defined(FEATURE_DPAD_5WIRESW_2OUT) || defined (FEATURE_DPAD_6WIRESW)
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_DPADDEBOUNCE, DPadDebounceAccessor>, SwitchScreenAction<DPadDebounceChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BUTTONDELAY, ButtonDelayAccessor>, SwitchScreenAction<ButtonDelayChangeScreen>>>();
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

std::string BoardcomputerHardwareSettingsMenu::text() const
{
    return TEXT_BOARDCOMPUTERHARDWARESETTINGS;
}

void BoardcomputerHardwareSettingsMenu::back()
{
    switchScreen<SettingsMenu>();
}
