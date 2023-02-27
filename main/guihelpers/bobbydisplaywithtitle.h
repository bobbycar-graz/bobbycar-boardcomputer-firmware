#pragma once

// 3rdparty lib includes
#include "displaywithtitle.h"

namespace bobby {

class BobbyDisplayWithTitle : public espgui::DisplayWithTitle
{
    using Base = espgui::DisplayWithTitle;

public:
    void rawButtonPressed(uint8_t button) override;
    void rawButtonReleased(uint8_t button) override;

    void buttonPressed(espgui::Button button) override;
    void buttonReleased(espgui::Button button) override;
};

} // namespace bobby
