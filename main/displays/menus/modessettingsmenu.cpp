#include "modessettingsmenu.h"

// 3rdparty lib includes
#include <actions/pushscreenaction.h>
#include <icons/back.h>
#include <menuitem.h>

// local includes
#include "displays/menus/defaultmodesettingsmenu.h"
#include "displays/menus/gametrakmodesettingsmenu.h"
#include "displays/menus/larsmmodesettingsmenu.h"
#include "displays/menus/remotecontrolmodesettingsmenu.h"
#include "displays/menus/tempomatmodesettingsmenu.h"

namespace {
constexpr char TEXT_MODESSETTINGS[] = "Modes settings";
constexpr char TEXT_DEFAULTMODESETTIGNS[] = "Default mode settings";
constexpr char TEXT_TEMPOMATMODESETTINGS[] = "Tempomat mode settings";
constexpr char TEXT_LARSMMODESETTINGS[] = "Larsm mode settings";
constexpr char TEXT_REMOTECONTROLMODESETTINGS[] = "Remote mode settings";
constexpr char TEXT_GAMETRAKMODESETTINGS[] = "Gametrak mode settings";
constexpr char TEXT_BACK[] = "Back";
} // namespace

ModesSettingsMenu::ModesSettingsMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEFAULTMODESETTIGNS>,   PushScreenAction<DefaultModeSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TEMPOMATMODESETTINGS>,  PushScreenAction<TempomatModeSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LARSMMODESETTINGS>,     PushScreenAction<LarsmModeSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REMOTECONTROLMODESETTINGS>, PushScreenAction<RemoteControlModeSettingsMenu>>>();
#ifdef FEATURE_GAMETRAK
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GAMETRAKMODESETTINGS>,  SwitchScreenAction<GametrakModeSettingsMenu>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                  PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string ModesSettingsMenu::text() const
{
    return TEXT_MODESSETTINGS;
}

void ModesSettingsMenu::back()
{
    espgui::popScreen();
}
