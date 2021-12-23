#pragma once

// system includes
#include <array>

// 3rdparty lib includes
#include <displaywithtitle.h>
#include <FastLED.h>

// local includes
#include "hardware/ledstrip.h"

#ifdef FEATURE_LEDSTRIP
extern int8_t selected_side;
extern int8_t selected_color;
extern bool state_select_color;
extern bool last_state;

extern const std::array<CRGB, 8> Colors;

extern const std::array<uint16_t, 8> tft_colors;

class LedstripColorsDisplay : public espgui::DisplayWithTitle
{
    using Base = espgui::DisplayWithTitle;

public:
    std::string text() const override;
    void back() override;
    void initScreen() override;
    void redraw() override;
    void rotate(int offset) override;
    void confirm() override;

    void drawColors();
    void drawSide(Bobbycar_Side side, unsigned int color);
    void clearSides();

private:
    bool already_drew_circle{false};
};
#endif
