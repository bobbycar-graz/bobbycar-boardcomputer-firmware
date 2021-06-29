#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "actions/multiaction.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"
#include "globals.h"

#include "modes/defaultmode.h"
#include "modes/tempomatmode.h"
#include "modes/larsmmode.h"
#include "modes/gametrakmode.h"

// forward declares
namespace {
class MainMenu;
} // namespace

namespace {
template<typename T1, T1 &target, typename T2, T2 value>
class SetterAction : public ActionInterface
{
public:
    void triggered() override { target = value; }
};
using SetDefaultModeAction = SetterAction<ModeInterface*, currentMode, DefaultMode*, &modes::defaultMode>;
using SetTempomatModeAction = SetterAction<ModeInterface*, currentMode, TempomatMode*, &modes::tempomatMode>;
using SetLarsmModeAction = SetterAction<ModeInterface*, currentMode, LarsmMode*, &modes::larsmMode>;
#ifdef FEATURE_GAMETRAK
using SetGametrakModeAction = SetterAction<ModeInterface*, currentMode, GametrakMode*, &modes::gametrakMode>;
#endif

class SelectModeMenu :
    public MenuDisplay,
    public StaticText<TEXT_SELECTMODE>,
    public BackActionInterface<SwitchScreenAction<MainMenu>>
{
    using Base = MenuDisplay;

public:
    SelectModeMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULT>,   MultiAction<SetDefaultModeAction, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TEMPOMAT>,  MultiAction<SetTempomatModeAction, SwitchScreenAction<MainMenu>>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LARSM>,     MultiAction<SetLarsmModeAction, SwitchScreenAction<MainMenu>>>>();
#ifdef FEATURE_GAMETRAK
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GAMETRAK>,  MultiAction<SetGametrakModeAction, SwitchScreenAction<MainMenu>>>>();
#endif
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,      SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&icons::back>>>();
    }

    void start() override;
};

void SelectModeMenu::start()
{
    Base::start();

    if (currentMode == &modes::defaultMode)
        setSelectedIndex(0);
    else if (currentMode == &modes::tempomatMode)
        setSelectedIndex(1);
    else if (currentMode == &modes::larsmMode)
        setSelectedIndex(2);
    else
    {
        //Serial.printf("Unknown mode: %s", currentMode?currentMode->displayName():"");
        setSelectedIndex(3);
    }
}
} // namespace
