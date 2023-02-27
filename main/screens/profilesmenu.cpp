#include "profilesmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <menudisplay.h>

// local includes
#include "actions/switchprofileaction.h"
#include "icons/back.h"

namespace bobby {

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
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,     PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string ProfilesMenu::text() const
{
    return TEXT_PROFILES;
}

void ProfilesMenu::back()
{
    popScreen();
}
} // namespace bobby
