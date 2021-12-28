#pragma once

// 3rdparty lib includes
#include "buttonsinterface.h"

enum BobbyButton
{
    Profile0 = espgui::Button::ButtonMax + 1,
    Profile1,
    Profile2,
    Profile3,
    Left2,
    Right2,
    Up2,
    Down2,
    Confirm = espgui::Button::Right,
    Back = espgui::Button::Left
};

class BobbyButtons : public virtual espgui::ButtonsInterface
{
public:
    void rawButtonPressed(uint8_t button) override;
    void rawButtonReleased(uint8_t button) override;

    void buttonPressed(espgui::Button button) override;
    void buttonReleased(espgui::Button button) override;
};
