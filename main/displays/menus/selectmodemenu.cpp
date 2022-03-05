#include "selectmodemenu.h"

// 3rdparty lib includes
#include <actions/multiaction.h>
#include <actions/popscreenaction.h>
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
#include "accessors/globalaccessors.h"

namespace {
constexpr char TEXT_SELECTMODE[] = "Select mode";
constexpr char TEXT_DEFAULT[] = "Default";
constexpr char TEXT_TEMPOMAT[] = "Tempomat";
constexpr char TEXT_LARSM[] = "Larsm";
constexpr char TEXT_REMOTECONTROL[] = "Remote control";
constexpr char TEXT_GAMETRAK[] = "Gametrak";
constexpr char TEXT_MOTORTEST[] = "Motortest";
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
} // namespace

using namespace espgui;

SelectModeMenu::SelectModeMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULT>,       MultiAction<SetDefaultModeAction, PopScreenAction>>>();
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_TEMPOMAT, AvgSpeedAccessor>,  MultiAction<SetTempomatModeAction, PopScreenAction>>>(); }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LARSM>,         MultiAction<SetLarsmModeAction, PopScreenAction>>>();
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REMOTECONTROL>, MultiAction<SetRemoteControlModeAction, PopScreenAction>>>(); }
#ifdef FEATURE_GAMETRAK
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GAMETRAK>,  MultiAction<SetGametrakModeAction, PopScreenAction>>>();
#endif
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MOTORTEST>, MultiAction<SetMotorTestModeAction, PopScreenAction>>>(); }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,      PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
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
    else
    {
        //Serial.printf("Unknown mode: %s", currentMode?currentMode->displayName():"");
        setSelectedIndex(4);
    }
}

void SelectModeMenu::back()
{
    popScreen();
}
