#include "bobbypopupdisplay.h"

// local includes
#include "bobbybuttons.h"

namespace bobby {

void BobbyPopupDisplay::rawButtonPressed(uint8_t button)
{
    //Base::rawButtonPressed(button);
    if (const auto translated = translateRawButton(button))
        buttonPressed(*translated);
}

void BobbyPopupDisplay::rawButtonReleased(uint8_t button)
{
    //Base::rawButtonReleased(button);
    if (const auto translated = translateRawButton(button))
        buttonReleased(*translated);
}

void BobbyPopupDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);
    buttonPressedCommon(button);
}

void BobbyPopupDisplay::buttonReleased(espgui::Button button)
{
    //Base::buttonReleased(button);
    buttonReleasedCommon(button);
}

} // namespace bobby
