#include "crashmenu.h"

// local includes
#include "actions/assertaction.h"
#include "actions/dividebyzeroaction.h"
#include "actions/switchscreenaction.h"
#include "displays/menus/settingsmenu.h"
#include "icons/back.h"

CrashMenu::CrashMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CRASH_ASSERT>,    AssertAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CRASH_DIVZERO>,   DivideByZeroAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,            SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void CrashMenu::back()
{
    switchScreen<SettingsMenu>();
}
