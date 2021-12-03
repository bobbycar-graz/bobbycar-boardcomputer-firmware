#include "invertmenu.h"

// local includes
#include "accessors/settingsaccessors.h"
#include "actions/switchscreenaction.h"
#include "actions/toggleboolaction.h"
#include "checkboxicon.h"
#include "displays/menus/controllerhardwaresettingsmenu.h"
#include "icons/back.h"

InvertMenu::InvertMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INVERTFRONTLEFT>,  ToggleBoolAction, CheckboxIcon, FrontLeftInvertedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INVERTFRONTRIGHT>, ToggleBoolAction, CheckboxIcon, FrontRightInvertedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INVERTBACKLEFT>,   ToggleBoolAction, CheckboxIcon, BackLeftInvertedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INVERTBACKRIGHT>,  ToggleBoolAction, CheckboxIcon, BackRightInvertedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,             SwitchScreenAction<ControllerHardwareSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}


void InvertMenu::back()
{
    switchScreen<ControllerHardwareSettingsMenu>();
}
