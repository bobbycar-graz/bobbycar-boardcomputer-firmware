#include "bobbymenudisplay.h"

// local includes
#include "esp_log.h"
#include "bobbybuttons.h"

namespace bobby {

void BobbyMenuDisplay::rawButtonPressed(uint8_t button)
{
    //Base::rawButtonPressed(button);
    if (const auto translated = translateRawButton(button))
        buttonPressed(*translated);
}

void BobbyMenuDisplay::rawButtonReleased(uint8_t button)
{
    //Base::rawButtonReleased(button);
    if (const auto translated = translateRawButton(button))
        buttonReleased(*translated);
}

void BobbyMenuDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);
    buttonPressedCommon(button);
}

void BobbyMenuDisplay::buttonReleased(espgui::Button button)
{
    Base::buttonReleased(button);
    buttonReleasedCommon(button);
}
} // namespace bobby
