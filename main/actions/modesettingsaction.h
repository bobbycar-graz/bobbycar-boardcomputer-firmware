#pragma once

#include "actioninterface.h"
#include "globals.h"
#include "modes/defaultmode.h"
#include "modes/tempomatmode.h"
#include "modes/larsmmode.h"
#include "modes/gametrakmode.h"
#include "modes/motortestmode.h"
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
    if (currentMode == &modes::defaultMode)
        pushScreen<DefaultModeSettingsMenu>();
    else if (currentMode == &modes::tempomatMode)
        pushScreen<TempomatModeSettingsMenu>();
    else if (currentMode == &modes::larsmMode)
        pushScreen<LarsmModeSettingsMenu>();
    else if (currentMode == &modes::motortestMode)
        pushScreen<MotortestModeSettingsMenu>();
#ifdef FEATURE_GAMETRAK
    else if (currentMode == &modes::gametrakMode)
        pushScreen<GametrakModeSettingsMenu>();
#endif
    else
    {
        //Serial.println("Unknown mode, cannot open settings for it");
    }
}
}
