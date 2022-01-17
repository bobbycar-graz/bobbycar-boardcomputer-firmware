#pragma once

// system includes
#include <array>

// 3rdparty lib includes
#include <FastLED.h>

// local includes
#include "bobbydisplaywithtitle.h"
#include "ledstrip.h"

class LedstripColorsDisplay : public BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;

public:
    std::string text() const override;
    void initScreen() override;
    void redraw() override;

    void buttonPressed(espgui::Button button) override;

    void drawColors();
    void drawSide(Bobbycar_Side side, unsigned int color);
    void clearSides();

private:
    bool already_drew_circle{false};
};
