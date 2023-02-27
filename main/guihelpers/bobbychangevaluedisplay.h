#pragma once

// 3rdparty lib includes
#include <changevaluedisplay.h>

// local includes
#include "bobbybuttons.h"
#include "bobbyerrorhandler.h"

namespace bobby {

template<typename Tvalue>
class BobbyChangeValueDisplay :
        public espgui::ChangeValueDisplay<Tvalue>,
        public virtual BobbyErrorHandler
{
    using Base = espgui::ChangeValueDisplay<Tvalue>;

public:
    void rawButtonPressed(uint8_t button) override;

    void rawButtonReleased(uint8_t button) override;

    void buttonPressed(espgui::Button button) override;

    void buttonReleased(espgui::Button button) override;
};

template<typename Tvalue>
void BobbyChangeValueDisplay<Tvalue>::rawButtonPressed(uint8_t button)
{
    //Base::rawButtonPressed(button);
    if (const auto translated = translateRawButton(button))
        buttonPressed(*translated);
}

template<typename Tvalue>
void BobbyChangeValueDisplay<Tvalue>::rawButtonReleased(uint8_t button)
{
    //Base::rawButtonReleased(button);
    if (const auto translated = translateRawButton(button))
        buttonReleased(*translated);
}

template<typename Tvalue>
void BobbyChangeValueDisplay<Tvalue>::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);
    buttonPressedCommon(button);
}

template<typename Tvalue>
void BobbyChangeValueDisplay<Tvalue>::buttonReleased(espgui::Button button)
{
    Base::buttonReleased(button);
    buttonReleasedCommon(button);
}

} // namespace bobby
