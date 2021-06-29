#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "changevaluedisplay.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "actions/toggleboolaction.h"
#include "icons/back.h"
#include "checkboxicon.h"
#include "texts.h"
#include "settingsaccessors.h"

// forward declares
namespace {
class BoardcomputerHardwareSettingsMenu;
class CalibrateDisplay;
class GametrakCalibrateDisplay;
class TimersMenu;
class SettingsMenu;
} // namespace

namespace {
struct GasText : public virtual TextInterface {
public:
    std::string text() const override
    {
        return std::string{"gas: "} +
            (raw_gas ? std::to_string(*raw_gas) : "?") +
            ": " +
            (gas ? std::to_string(*gas) : "?");
    }
};
struct BremsText : public virtual TextInterface {
public:
    std::string text() const override
    {
        return std::string{"brems: "} +
            (raw_brems ? std::to_string(*raw_brems) : "?") +
            ": " +
            (brems ? std::to_string(*brems) : "?");
    }
};

using SampleCountChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETSAMPLECOUNT>,
    SampleCountAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GasMinChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETGASMIN>,
    GasMinAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using GasMaxChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETGASMAX>,
    GasMaxAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using BremsMinChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETBREMSMIN>,
    BremsMinAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
using BremsMaxChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETBREMSMAX>,
    BremsMaxAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;

#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW)
using DPadDebounceChangeScreen = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_SETDPADDEBOUNCE>,
    DPadDebounceAccessor,
    BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>,
    SwitchScreenAction<BoardcomputerHardwareSettingsMenu>
>;
#endif

#ifdef FEATURE_GAMETRAK
struct GametrakXText : public virtual TextInterface {
public:
    std::string text() const override { return std::string{"gametrakX: "} + std::to_string(raw_gametrakX) + ": " + std::to_string(gametrakX); }
};
struct GametrakYText : public virtual TextInterface {
public:
    std::string text() const override { return std::string{"gametrakY: "} + std::to_string(raw_gametrakY) + ": " + std::to_string(gametrakY); }
};
struct GametrakDistText : public virtual TextInterface {
public:
    std::string text() const override { return std::string{"gametrakDist: "} + std::to_string(raw_gametrakDist) + ": " + std::to_string(gametrakDist); }
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

class BoardcomputerHardwareSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_BOARDCOMPUTERHARDWARESETTINGS>,
    public BackActionInterface<SwitchScreenAction<SettingsMenu>>
{
public:
    BoardcomputerHardwareSettingsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, GasText,                             DisabledColor, StaticFont<2>, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, BremsText,                           DisabledColor, StaticFont<2>, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CALIBRATE>,          SwitchScreenAction<CalibrateDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETSAMPLECOUNT>,     SwitchScreenAction<SampleCountChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGASMIN>,          SwitchScreenAction<GasMinChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGASMAX>,          SwitchScreenAction<GasMaxChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETBREMSMIN>,        SwitchScreenAction<BremsMinChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETBREMSMAX>,        SwitchScreenAction<BremsMaxChangeScreen>>>();
#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW)
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETDPADDEBOUNCE>,    SwitchScreenAction<DPadDebounceChangeScreen>>>();
#endif
#ifdef FEATURE_GAMETRAK
        constructMenuItem<makeComponent<MenuItem, EmptyText,                           DummyAction>,
        constructMenuItem<makeComponent<MenuItem, GametrakXText,                       DisabledColor, StaticFont<2>, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, GametrakYText,                       DisabledColor, StaticFont<2>, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, GametrakDistText,                    DisabledColor, StaticFont<2>, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GAMETRAKCALIBRATE>,  SwitchScreenAction<GametrakCalibrateDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKXMIN>,    SwitchScreenAction<GametrakXMinChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKXMAX>,    SwitchScreenAction<GametrakXMaxChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKYMIN>,    SwitchScreenAction<GametrakYMinChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKYMAX>,    SwitchScreenAction<GametrakYMaxChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKDISTMIN>, SwitchScreenAction<GametrakDistMinChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAMETRAKDISTMAX>, SwitchScreenAction<GametrakDistMaxChangeScreen>>>();
#endif
        constructMenuItem<makeComponent<MenuItem, EmptyText,                           DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SWAPSCREENBYTES>,    ToggleBoolAction, CheckboxIcon, SwapScreenBytesAccessor>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TIMERS>,             SwitchScreenAction<TimersMenu>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,               SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
} // namespace
