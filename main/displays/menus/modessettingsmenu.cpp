#include "modessettingsmenu.h"

using namespace espgui;

ModesSettingsMenu::ModesSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULTMODESETTIGNS>,   SwitchScreenAction<DefaultModeSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TEMPOMATMODESETTINGS>,  SwitchScreenAction<TempomatModeSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LARSMMODESETTINGS>,     SwitchScreenAction<LarsmModeSettingsMenu>>>();
#ifdef FEATURE_GAMETRAK
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GAMETRAKMODESETTINGS>,  SwitchScreenAction<GametrakModeSettingsMenu>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                  SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}
