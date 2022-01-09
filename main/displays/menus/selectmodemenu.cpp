#include "selectmodemenu.h"

// 3rdparty lib includes
#include <actions/multiaction.h>
#include <actions/switchscreenaction.h>
#include <icons/back.h>
#include <textwithvaluehelper.h>

// local includes
#include "utils.h"
#include "globals.h"
#include "modes/defaultmode.h"
#include "modes/tempomatmode.h"
#include "modes/larsmmode.h"
#include "modes/remotecontrolmode.h"
#include "modes/gametrakmode.h"
#include "modes/motortestmode.h"
#ifdef FEATURE_JOYSTICK
#include "modes/wheelchairmode.h"
#endif
#include "accessors/globalaccessors.h"
#include "displays/menus/mainmenu.h"

namespace {
constexpr char TEXT_SELECTMODE[] = "Select mode";
constexpr char TEXT_DEFAULT[] = "Default";
constexpr char TEXT_TEMPOMAT[] = "Tempomat";
constexpr char TEXT_LARSM[] = "Larsm";
constexpr char TEXT_REMOTECONTROL[] = "Remote control";
constexpr char TEXT_GAMETRAK[] = "Gametrak";
constexpr char TEXT_MOTORTEST[] = "Motortest";
#ifdef FEATURE_JOYSTICK
constexpr char TEXT_WHEELCHAIR[] = "Wheelchair";
#endif
constexpr char TEXT_BACK[] = "Back";

template<typename T1, T1 &target, typename T2, T2 value>
class SetterAction : public espgui::ActionInterface
{
public:
    void triggered() override { target = value; }
};
using SetDefaultModeAction = SetterAction<ModeInterface*, currentMode, DefaultMode*, &modes::defaultMode>;
using SetTempomatModeAction = SetterAction<ModeInterface*, currentMode, TempomatMode*, &modes::tempomatMode>;
using SetLarsmModeAction = SetterAction<ModeInterface*, currentMode, LarsmMode*, &modes::larsmMode>;
using SetRemoteControlModeAction = SetterAction<ModeInterface*, currentMode, RemoteControlMode*, &modes::remoteControlMode>;
using SetMotorTestModeAction = SetterAction<ModeInterface*, currentMode, MotortestMode*, &modes::motortestMode>;
#ifdef FEATURE_GAMETRAK
using SetGametrakModeAction = SetterAction<ModeInterface*, currentMode, GametrakMode*, &modes::gametrakMode>;
#endif
#ifdef FEATURE_JOYSTICK
using SetWheelchairModeAction = SetterAction<ModeInterface*, currentMode, WheelchairMode*, &modes::wheelchairMode>;
#endif
} // namespace

using namespace espgui;

SelectModeMenu::SelectModeMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULT>,       MultiAction<SetDefaultModeAction, SwitchScreenAction<MainMenu>>>>();
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_TEMPOMAT, AvgSpeedAccessor>,  MultiAction<SetTempomatModeAction, SwitchScreenAction<MainMenu>>>>(); }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LARSM>,         MultiAction<SetLarsmModeAction, SwitchScreenAction<MainMenu>>>>();
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REMOTECONTROL>, MultiAction<SetRemoteControlModeAction, SwitchScreenAction<MainMenu>>>>(); }
#ifdef FEATURE_GAMETRAK
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GAMETRAK>,  MultiAction<SetGametrakModeAction, SwitchScreenAction<MainMenu>>>>();
#endif
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOTORTEST>, MultiAction<SetMotorTestModeAction, SwitchScreenAction<MainMenu>>>>(); }
#ifdef FEATURE_JOYSTICK
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WHEELCHAIR>, MultiAction<SetWheelchairModeAction, SwitchScreenAction<MainMenu>>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,      SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string SelectModeMenu::text() const
{
    return TEXT_SELECTMODE;
}

void SelectModeMenu::start()
{
    Base::start();

    if (currentMode == &modes::defaultMode)
        setSelectedIndex(0);
    else if (currentMode == &modes::tempomatMode)
        setSelectedIndex(1);
    else if (currentMode == &modes::larsmMode)
        setSelectedIndex(2);
    else if (currentMode == &modes::motortestMode)
        setSelectedIndex(3);
#ifdef FEATURE_JOYSTICK
    else if (currentMode == &modes::wheelchairMode)
        setSelectedIndex(4);
#endif
    else
    {
        //Serial.printf("Unknown mode: %s", currentMode?currentMode->displayName():"");
#ifdef FEATURE_JOYSTICK
        setSelectedIndex(5);
#else
        setSelectedIndex(4);
#endif
    }
}

void SelectModeMenu::back()
{
    switchScreen<MainMenu>();
}
