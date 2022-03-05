#include "profilesmenu.h"

// local includes
#include "actions/switchprofileaction.h"
#include "actions/popscreenaction.h"
#include "icons/back.h"
#include "menudisplay.h"

namespace {
constexpr char TEXT_PROFILES[] = "Profiles";
constexpr char TEXT_PROFILE0[] = "Profile 0";
constexpr char TEXT_PROFILE1[] = "Profile 1";
constexpr char TEXT_PROFILE2[] = "Profile 2";
constexpr char TEXT_PROFILE3[] = "Profile 3";
constexpr char TEXT_BACK[] = "Back";
} // namespace

ProfilesMenu::ProfilesMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_PROFILE0>, SwitchProfileAction<0>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_PROFILE1>, SwitchProfileAction<1>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_PROFILE2>, SwitchProfileAction<2>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_PROFILE3>, SwitchProfileAction<3>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,     PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string ProfilesMenu::text() const
{
    return TEXT_PROFILES;
}

void ProfilesMenu::back()
{
    popScreen();
}
