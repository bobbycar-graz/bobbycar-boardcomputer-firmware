#include "modesettingsaction.h"

// 3rdparty lib includes
#include <screenmanager.h>

// local includes
#include "globals.h"
#include "modes/defaultmode.h"
#include "modes/mickmode.h"
#include "modes/tempomatmode.h"
#include "modes/larsmmode.h"
#ifdef FEATURE_GAMETRAK
#include "modes/gametrakmode.h"
#endif
#include "modes/motortestmode.h"
#ifdef FEATURE_JOYSTICK
#include "modes/wheelchairmode.h"
#endif
#include "modes/remotecontrolmode.h"
#include "screens/defaultmodesettingsmenu.h"
#include "screens/tempomatmodesettingsmenu.h"
#include "screens/larsmmodesettingsmenu.h"
#ifdef FEATURE_GAMETRAK
#include "screens/gametrakmodesettingsmenu.h"
#endif
#include "screens/mickmodesettingsmenu.h"
#include "screens/motortestmodesettingsmenu.h"
#include "screens/remotecontrolmodesettingsmenu.h"

namespace bobby {

void ModeSettingsAction::triggered()
{
    if (currentMode == &modes::defaultMode
#ifdef FEATURE_JOYSTICK
        || currentMode == &modes::wheelchairMode
#endif
            )
        pushScreen<DefaultModeSettingsMenu>();
    else if (currentMode == &modes::tempomatMode)
        pushScreen<TempomatModeSettingsMenu>();
    else if (currentMode == &modes::larsmMode)
        pushScreen<LarsmModeSettingsMenu>();
    else if (currentMode == &modes::mickMode)
        pushScreen<MickModeSettingsMenu>();
    else if (currentMode == &modes::motortestMode)
        pushScreen<MotortestModeSettingsMenu>();
    else if (currentMode == &modes::remoteControlMode)
        pushScreen<RemoteControlModeSettingsMenu>();
#ifdef FEATURE_GAMETRAK
        else if (currentMode == &modes::gametrakMode)
        pushScreen<GametrakModeSettingsMenu>();
#endif
    else
    {
        //Serial.println("Unknown mode, cannot open settings for it");
    }
}
} // namespace bobby
