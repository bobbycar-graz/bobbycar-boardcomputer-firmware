#pragma once

// system includes
#include <cstdint>

// local includes
#include "guihelpers/bobbydisplay.h"

namespace bobby {

class SpiroDisplay : public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;
    void stop() override;

    void buttonPressed(espgui::Button button) override;

private:
    constexpr static auto DEG2RAD = 0.0174532925;

    static unsigned int rainbow(int value);

    float sx = 0, sy = 0;
    uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;

    long i{0};
    int n{}, r{}, colour{};
};
} // namespace bobby
