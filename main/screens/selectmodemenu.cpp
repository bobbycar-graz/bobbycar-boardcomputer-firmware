#include "selectmodemenu.h"

// 3rdparty lib includes
#include <actions/multiaction.h>
#include <actions/popscreenaction.h>
#include <textwithvaluehelper.h>
#include <actions/pushscreenaction.h>

// local includes
#include "utils.h"
#include "globals.h"
#include "modes/defaultmode.h"
#include "modes/tempomatmode.h"
#include "modes/larsmmode.h"
#include "modes/remotecontrolmode.h"
#include "modes/gametrakmode.h"
#include "modes/mickmode.h"
#include "modes/motortestmode.h"
#ifdef FEATURE_JOYSTICK
#include "modes/wheelchairmode.h"
#endif
#include "accessors/globalaccessors.h"
#include "mainmenu.h"
#include "icons/back.h"

namespace bobby {

namespace {
constexpr char TEXT_SELECTMODE[] = "Select mode";
constexpr char TEXT_DEFAULT[] = "Default";
constexpr char TEXT_TEMPOMAT[] = "Tempomat";
constexpr char TEXT_LARSM[] = "Larsm";
constexpr char TEXT_MICK[] = "mick";
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
using SetMickModeAction = SetterAction<ModeInterface*, currentMode, MickMode*, &modes::mickMode>;
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
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULT>,       MultiAction<SetDefaultModeAction, PopScreenAction>>>();
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_TEMPOMAT, AvgSpeedAccessor>,  MultiAction<SetTempomatModeAction, PopScreenAction>>>(); }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LARSM>,         MultiAction<SetLarsmModeAction, PopScreenAction>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MICK>,         MultiAction<SetMickModeAction, PopScreenAction>>>();
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REMOTECONTROL>, MultiAction<SetRemoteControlModeAction, PopScreenAction>>>(); }
#ifdef FEATURE_GAMETRAK
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GAMETRAK>,  MultiAction<SetGametrakModeAction, PopScreenAction>>>(); }
#endif
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOTORTEST>, MultiAction<SetMotorTestModeAction, PopScreenAction>>>(); }
#ifdef FEATURE_JOYSTICK
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WHEELCHAIR>, MultiAction<SetWheelchairModeAction, PopScreenAction>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,      PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
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
    else if (currentMode == &modes::mickMode)
        setSelectedIndex(3);
    else if (currentMode == &modes::remoteControlMode)
        setSelectedIndex(4);
    else if (currentMode == &modes::motortestMode)
        setSelectedIndex(5);
#ifdef FEATURE_JOYSTICK
    else if (currentMode == &modes::wheelchairMode)
        setSelectedIndex(6);
#endif
    else
    {
        //Serial.printf("Unknown mode: %s", currentMode?currentMode->displayName():"");
#ifdef FEATURE_JOYSTICK
        setSelectedIndex(6);
#else
        setSelectedIndex(5);
#endif
    }
}

void SelectModeMenu::back()
{
    espgui::popScreen();
}
} // namespace bobby
