#pragma once

// 3rdparty lib includes
#include <splitgraphdisplay.h>

// local includes
#include "bobbybuttons.h"

namespace bobby {

template<std::size_t COUNT0, std::size_t COUNT1>
class BobbySplitGraphDisplay : public espgui::SplitGraphDisplay<COUNT0, COUNT1>
{
    using Base = espgui::SplitGraphDisplay<COUNT0, COUNT1>;

public:
    void rawButtonPressed(uint8_t button) override;
    void rawButtonReleased(uint8_t button) override;

    void buttonPressed(espgui::Button button) override;
    void buttonReleased(espgui::Button button) override;
};

template<std::size_t COUNT0, std::size_t COUNT1>
void BobbySplitGraphDisplay<COUNT0, COUNT1>::rawButtonPressed(uint8_t button)
{
    //Base::rawButtonPressed(button);
    if (const auto translated = translateRawButton(button))
        buttonPressed(*translated);
}

template<std::size_t COUNT0, std::size_t COUNT1>
void BobbySplitGraphDisplay<COUNT0, COUNT1>::rawButtonReleased(uint8_t button)
{
    //Base::rawButtonReleased(button);
    if (const auto translated = translateRawButton(button))
        buttonReleased(*translated);
}

template<std::size_t COUNT0, std::size_t COUNT1>
void BobbySplitGraphDisplay<COUNT0, COUNT1>::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);
    buttonPressedCommon(button);
}

template<std::size_t COUNT0, std::size_t COUNT1>
void BobbySplitGraphDisplay<COUNT0, COUNT1>::buttonReleased(espgui::Button button)
{
    //Base::buttonReleased(button);
    buttonReleasedCommon(button);
}
} // namespace bobby

