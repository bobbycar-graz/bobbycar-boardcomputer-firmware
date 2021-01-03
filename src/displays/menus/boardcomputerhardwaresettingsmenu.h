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
    String text() const override { return String{"gas: "} + raw_gas + ": " + gas; }
};
struct BremsText : public virtual TextInterface {
public:
    String text() const override { return String{"brems: "} + raw_brems + ": " + brems; }
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
    String text() const override { return String{"gametrakX: "} + raw_gametrakX + ": " + gametrakX; }
};
struct GametrakYText : public virtual TextInterface {
public:
    String text() const override { return String{"gametrakY: "} + raw_gametrakY + ": " + gametrakY; }
};
struct GametrakDistText : public virtual TextInterface {
public:
    String text() const override { return String{"gametrakDist: "} + raw_gametrakDist + ": " + gametrakDist; }
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
        constructMenuItem<NewMenuItem>(std::make_unique<GasText>(),                             std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<BremsText>(),                           std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_CALIBRATE>>(),          std::make_unique<SwitchScreenAction<CalibrateDisplay>>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SETSAMPLECOUNT>>(),     std::make_unique<SwitchScreenAction<SampleCountChangeScreen>>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SETGASMIN>>(),          std::make_unique<SwitchScreenAction<GasMinChangeScreen>>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SETGASMAX>>(),          std::make_unique<SwitchScreenAction<GasMaxChangeScreen>>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SETBREMSMIN>>(),        std::make_unique<SwitchScreenAction<BremsMinChangeScreen>>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SETBREMSMAX>>(),        std::make_unique<SwitchScreenAction<BremsMaxChangeScreen>>());
#if defined(FEATURE_DPAD) || defined(FEATURE_DPAD_3WIRESW) || defined(FEATURE_DPAD_5WIRESW)
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SETDPADDEBOUNCE>>(),    std::make_unique<SwitchScreenAction<DPadDebounceChangeScreen>>());
#endif
#ifdef FEATURE_GAMETRAK
        constructMenuItem<NewMenuItem>(),
        constructMenuItem<NewMenuItem>(std::make_unique<GametrakXText>(),                       std::make_unique<DisabledColor>(), std::make_unique<ConstantFont>(2));
        constructMenuItem<NewMenuItem>(std::make_unique<GametrakYText>(),                       std::make_unique<DisabledColor>(), std::make_unique<ConstantFont>(2));
        constructMenuItem<NewMenuItem>(std::make_unique<GametrakDistText>(),                    std::make_unique<DisabledColor>(), std::make_unique<ConstantFont>(2));
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_GAMETRAKCALIBRATE>>(),  std::make_unique<SwitchScreenAction<GametrakCalibrateDisplay>>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SETGAMETRAKXMIN>>(),    std::make_unique<SwitchScreenAction<GametrakXMinChangeScreen>>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SETGAMETRAKXMAX>>(),    std::make_unique<SwitchScreenAction<GametrakXMaxChangeScreen>>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SETGAMETRAKYMIN>>(),    std::make_unique<SwitchScreenAction<GametrakYMinChangeScreen>>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SETGAMETRAKYMAX>>(),    std::make_unique<SwitchScreenAction<GametrakYMaxChangeScreen>>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SETGAMETRAKDISTMIN>>(), std::make_unique<SwitchScreenAction<GametrakDistMinChangeScreen>>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SETGAMETRAKDISTMAX>>(), std::make_unique<SwitchScreenAction<GametrakDistMaxChangeScreen>>());
#endif
        constructMenuItem<NewMenuItem>();
        //constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_SWAPSCREENBYTES>>(),    std::make_unique<ToggleBoolAction>(std::make_unique<SwapScreenBytesAccessor>()), std::make_unique<CheckboxIcon>(std::make_unique<SwapScreenBytesAccessor>())); // TODO feedc0de
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_TIMERS>>(),             std::make_unique<SwitchScreenAction<TimersMenu>>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_BACK>>(),               std::make_unique<SwitchScreenAction<SettingsMenu>>(), std::make_unique<StaticMenuItemIcon<&icons::back>>());
    }
};
} // namespace
