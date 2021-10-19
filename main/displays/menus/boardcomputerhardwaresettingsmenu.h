#pragma once

// 3rdparty lib includes
#include <fmt/core.h>

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "changevaluedisplay.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "actions/toggleboolaction.h"
#include "icons/lock.h"
#include "icons/back.h"
#include "checkboxicon.h"
#include "texts.h"
#include "accessors/settingsaccessors.h"

// forward declares
class BoardcomputerHardwareSettingsMenu;
class LockscreenSettingsMenu;
class CalibrateDisplay;
class GametrakCalibrateDisplay;
class TimersMenu;
class SettingsMenu;

class BoardcomputerHardwareSettingsMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_BOARDCOMPUTERHARDWARESETTINGS>,
    public espgui::BackActionInterface<espgui::SwitchScreenAction<SettingsMenu>>
{
public:
    BoardcomputerHardwareSettingsMenu();
};
