#include "invertmenu.h"

// local includes
#include "accessors/settingsaccessors.h"
#include "actions/popscreenaction.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"

namespace bobby {

namespace {
constexpr char TEXT_SETINVERTED[] = "Set inverted";
constexpr char TEXT_INVERTFRONTLEFT[] = "Invert front left";
constexpr char TEXT_INVERTFRONTRIGHT[] = "Invert front right";
constexpr char TEXT_INVERTBACKLEFT[] = "Invert back left";
constexpr char TEXT_INVERTBACKRIGHT[] = "Invert back right";
constexpr char TEXT_BACK[] = "Back";
} // namespace

InvertMenu::InvertMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INVERTFRONTLEFT>,  BobbyCheckbox, FrontLeftInvertedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INVERTFRONTRIGHT>, BobbyCheckbox, FrontRightInvertedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INVERTBACKLEFT>,   BobbyCheckbox, BackLeftInvertedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INVERTBACKRIGHT>,  BobbyCheckbox, BackRightInvertedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,             PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string InvertMenu::text() const
{
    return TEXT_SETINVERTED;
}

void InvertMenu::back()
{
    espgui::popScreen();
}

} // namespace bobby
