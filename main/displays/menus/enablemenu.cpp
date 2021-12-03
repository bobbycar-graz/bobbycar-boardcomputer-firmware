#include "enablemenu.h"

// local includes
#include "accessors/settingsaccessors.h"
#include "actions/switchscreenaction.h"
#include "actions/toggleboolaction.h"
#include "checkboxicon.h"
#include "displays/menus/controllerhardwaresettingsmenu.h"
#include "icons/back.h"

EnableMenu::EnableMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLEFRONTLEFT>,  ToggleBoolAction, CheckboxIcon, FrontLeftEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLEFRONTRIGHT>, ToggleBoolAction, CheckboxIcon, FrontRightEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLEBACKLEFT>,   ToggleBoolAction, CheckboxIcon, BackLeftEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLEBACKRIGHT>,  ToggleBoolAction, CheckboxIcon, BackRightEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,             SwitchScreenAction<ControllerHardwareSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void EnableMenu::back()
{
    switchScreen<ControllerHardwareSettingsMenu>();
}
