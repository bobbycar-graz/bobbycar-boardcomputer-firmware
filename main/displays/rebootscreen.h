#pragma once

// local includes
#include "displays/bobbydisplaywithtitle.h"

class RebootScreen : public BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;

public:
    void initScreen(espgui::TftInterface &tft) override;

    std::string title() const override;

    void buttonPressed(espgui::Button button) override;
    void buttonReleased(espgui::Button button) override;
};
