#pragma once

#include "actioninterface.h"
#include "globals.h"
#include "modes/defaultmode.h"
#include "modes/tempomatmode.h"
#include "modes/larsmmode.h"
#include "modes/gametrakmode.h"
#include "displays/menus/defaultmodesettingsmenu.h"
#include "displays/menus/tempomatmodesettingsmenu.h"
#include "displays/menus/larsmmodesettingsmenu.h"
#include "displays/menus/gametrakmodesettingsmenu.h"

namespace {
class ModeSettingsAction : public virtual ActionInterface
{
public:
    void triggered() override;
};

void ModeSettingsAction::triggered()
{
    if (currentMode == &modes::defaultMode)
        switchScreen<DefaultModeSettingsMenu>();
    else if (currentMode == &modes::tempomatMode)
        switchScreen<TempomatModeSettingsMenu>();
    else if (currentMode == &modes::larsmMode)
        switchScreen<LarsmModeSettingsMenu>();
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
