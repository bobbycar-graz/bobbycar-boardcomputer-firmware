#pragma once

#include "actioninterface.h"
#include "globals.h"
#include "modes/defaultmode.h"
#include "modes/tempomatmode.h"
#include "modes/larsmmode.h"
#include "modes/gametrakmode.h"
#include "modes/motortestmode.h"
#include "modes/wheelchairmode.h"
#include "displays/menus/defaultmodesettingsmenu.h"
#include "displays/menus/tempomatmodesettingsmenu.h"
#include "displays/menus/larsmmodesettingsmenu.h"
#include "displays/menus/gametrakmodesettingsmenu.h"
#include "displays/menus/motortestmodesettingsmenu.h"
#include "screenmanager.h"

using namespace espgui;

namespace {
class ModeSettingsAction : public virtual ActionInterface
{
public:
    void triggered() override;
};

void ModeSettingsAction::triggered()
{
    if (
            currentMode == &modes::defaultMode
        #ifdef FEATURE_JOYSTICK
         || currentMode == &modes::wheelchairMode
        #endif
       )
        switchScreen<DefaultModeSettingsMenu>();
    else if (currentMode == &modes::tempomatMode)
        switchScreen<TempomatModeSettingsMenu>();
    else if (currentMode == &modes::larsmMode)
        switchScreen<LarsmModeSettingsMenu>();
    else if (currentMode == &modes::motortestMode)
        switchScreen<MotortestModeSettingsMenu>();
#ifdef FEATURE_GAMETRAK
    else if (currentMode == &modes::gametrakMode)
        switchScreen<GametrakModeSettingsMenu>();
#endif
    else
    {
        //Serial.println("Unknown mode, cannot open settings for it");
    }
}
}
