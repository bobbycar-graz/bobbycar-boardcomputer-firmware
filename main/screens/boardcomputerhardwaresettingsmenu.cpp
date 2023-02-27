#include "boardcomputerhardwaresettingsmenu.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay.h>
#include <fmt/core.h>
#include <textwithvaluehelper.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "actions/setupactions.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"
#include "icons/lock.h"
#include "screens/extrabuttoncalibratemenu.h"
#include "screens/lockscreensettingsmenu.h"
#include "screens/setupquickactionsmenu.h"
#include "screens/timersmenu.h"

#ifdef FEATURE_JOYSTICK
#include "screens/joystickdebugdisplay.h"
#endif

namespace bobby {

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
constexpr char TEXT_FLIPDISPLAY[] = "Flip Display";
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
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using GasMinChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_GASMIN>,
    GasMinAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using GasMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_GASMAX>,
    GasMaxAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using BremsMinChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_BREMSMIN>,
    BremsMinAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using BremsMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_BREMSMAX>,
    BremsMaxAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;


#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW) || defined(FEATURE_DPAD_5WIRESW_2OUT) || defined (FEATURE_DPAD_6WIRESW) || defined (DPAD_BOARDCOMPUTER_V2)
using DPadDebounceChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_DPADDEBOUNCE>,
    DPadDebounceAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using ButtonDelayChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint16_t>,
    espgui::StaticText<TEXT_BUTTONDELAY>,
    ButtonDelayAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
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
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using GametrakXMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKXMAX>,
    GametrakXMaxAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using GametrakYMinChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKYMIN>,
    GametrakYMinAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using GametrakYMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKYMAX>,
    GametrakYMaxAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using GametrakDistMinChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKDISTMIN>,
    GametrakDistMinAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using GametrakDistMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SETGAMETRAKDISTMAX>,
    GametrakDistMaxAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
#endif
} // namespace

using namespace espgui;

BoardcomputerHardwareSettingsMenu::BoardcomputerHardwareSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LOCKSCREENSETTINGS>,                          PushScreenAction<LockscreenSettingsMenu>, StaticMenuItemIcon<&bobbyicons::lock>>>();
    constructMenuItem<makeComponentArgs<MenuItem, PushButtonCalibrateDisplayAction,                         StaticText<TEXT_BUTTONCALIBRATE>>>(true);
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_EXTRABUTTONCALIBRATE>,                        PushScreenAction<ExtraButtonCalibrateMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_QUICKACTIONS>,                                PushScreenAction<SetupQuickActionsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, GasText,                                                      DisabledColor, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BremsText,                                                    DisabledColor, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponentArgs<MenuItem, PushPotiCalibrateDisplayAction,                           StaticText<TEXT_POTISCALIBRATE>>>(true);
#ifdef FEATURE_JOYSTICK
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_JOYSTICK>,                                    PushScreenAction<JoystickDebugDisplay>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FLIPDISPLAY>,                                 BobbyCheckbox, FlipScreenAccessor>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SAMPLECOUNT, SampleCountAccessor>,   PushScreenAction<SampleCountChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_GASMIN, GasMinAccessor>,             PushScreenAction<GasMinChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_GASMAX, GasMaxAccessor>,             PushScreenAction<GasMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BREMSMIN, BremsMinAccessor>,         PushScreenAction<BremsMinChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BREMSMAX, BremsMaxAccessor>,         PushScreenAction<BremsMaxChangeScreen>>>();
#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW) || defined(FEATURE_DPAD_5WIRESW_2OUT) || defined (FEATURE_DPAD_6WIRESW) || defined (DPAD_BOARDCOMPUTER_V2)
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_DPADDEBOUNCE, DPadDebounceAccessor>, PushScreenAction<DPadDebounceChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BUTTONDELAY, ButtonDelayAccessor>, PushScreenAction<ButtonDelayChangeScreen>>>();
#endif
#ifdef FEATURE_GAMETRAK
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                                    DummyAction>,
    constructMenuItem<makeComponent<MenuItem, GametrakXText,                                                DisabledColor, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, GametrakYText,                                                DisabledColor, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, GametrakDistText,                                             DisabledColor, StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GAMETRAKCALIBRATE>,                           PushScreenAction<GametrakCalibrateDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKXMIN>,                             PushScreenAction<GametrakXMinChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKXMAX>,                             PushScreenAction<GametrakXMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKYMIN>,                             PushScreenAction<GametrakYMinChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKYMAX>,                             PushScreenAction<GametrakYMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKDISTMIN>,                          PushScreenAction<GametrakDistMinChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKDISTMAX>,                          PushScreenAction<GametrakDistMaxChangeScreen>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                                    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TIMERS>,                                      PushScreenAction<TimersMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                        PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string BoardcomputerHardwareSettingsMenu::text() const
{
    return TEXT_BOARDCOMPUTERHARDWARESETTINGS;
}

void BoardcomputerHardwareSettingsMenu::back()
{
    popScreen();
}

} // namespace bobby
