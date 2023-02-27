#pragma once

// 3rdparty lib includes
#include <graphdisplay.h>

// local includes
#include "bobbybuttons.h"

namespace bobby {

template<size_t COUNT>
class BobbyGraphDisplay : public espgui::GraphDisplay<COUNT>
{
    using Base = espgui::GraphDisplay<COUNT>;

public:
    void rawButtonPressed(uint8_t button) override;
    void rawButtonReleased(uint8_t button) override;

    void buttonPressed(espgui::Button button) override;
    void buttonReleased(espgui::Button button) override;
};

template<size_t COUNT>
void BobbyGraphDisplay<COUNT>::rawButtonPressed(uint8_t button)
{
    //Base::rawButtonPressed(button);
    if (const auto translated = translateRawButton(button))
        buttonPressed(*translated);
}

template<size_t COUNT>
void BobbyGraphDisplay<COUNT>::rawButtonReleased(uint8_t button)
{
    //Base::rawButtonReleased(button);
    if (const auto translated = translateRawButton(button))
        buttonReleased(*translated);
}

template<size_t COUNT>
void BobbyGraphDisplay<COUNT>::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);
    buttonPressedCommon(button);
}

template<size_t COUNT>
void BobbyGraphDisplay<COUNT>::buttonReleased(espgui::Button button)
{
    //Base::buttonReleased(button);
    buttonReleasedCommon(button);
}

} // namespace bobby
