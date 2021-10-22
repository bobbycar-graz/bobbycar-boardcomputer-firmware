#pragma once

#include <TFT_eSPI.h>
#include <FastLED.h>
#include <cpputils.h>

// Local includes
#include "menudisplay.h"
#include "utils.h"
#include "menuitem.h"
#include "ledstrip.h"
#include "icons/back.h"
#include "icons/bobbycar.h"
#include "texts.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "actioninterface.h"
#include "globals.h"
#include "esp_log.h"

using namespace espgui;

namespace  {
    class LedstripMenu;
}

namespace {

static int8_t selected_side = 7;
static int8_t selected_color;
bool state_select_color{false};
bool last_state = {false};

const std::array<CRGB, 8> Colors = {
    CRGB{0,0,0},
    CRGB{255,255,255},
    CRGB{255,0,0},
    CRGB{255,255,0},
    CRGB{0,255,0},
    CRGB{0,255,255},
    CRGB{0,0,255},
    CRGB{255,0,255}
};

const std::array<uint16_t, 8> tft_colors = {
    TFT_BLACK,
    TFT_WHITE,
    TFT_RED,
    TFT_YELLOW,
    TFT_GREEN,
    TFT_CYAN,
    TFT_BLUE,
    TFT_MAGENTA
};

class LedstripColorsMenuBackAction : ActionInterface
{
public:
    void triggered() {
        if(!state_select_color)
        {
            switchScreen<LedstripMenu>();
        }
        else
        {
            state_select_color = false;
            tft.fillRect(0, 228, tft.width(), ((tft.width() - 40) / 8) + 4, TFT_BLACK);
        }
    }
};

class LedstripColorsMenu :
        public MenuDisplay,
        public StaticText<TEXT_LEDSTRIPCOLORMENU>,
        public BackActionInterface<LedstripColorsMenuBackAction>
{
    using Base = MenuDisplay;
public:
    void redraw() override;
    void rotate(int offset) override;
    void confirm() override;
    void drawColors();
    void drawSide(Bobbycar_Side side, unsigned int color);
    void clearSides();
    LedstripColorsMenu() {}
private:
    bool already_drew_circle{false};
};

void LedstripColorsMenu::redraw()
{
    Base::redraw();
    tft.setSwapBytes(true);
    tft.pushImage(70, 60, bobbyicons::bobbycar.WIDTH, bobbyicons::bobbycar.HEIGHT, bobbyicons::bobbycar.buffer);
    tft.setSwapBytes(false);

    auto y_pos = ((tft.width() - 40) / 8 + 4) + 240;
    if (last_state != state_select_color)
    {
        tft.fillRect(0,y_pos - 1, tft.width(), 20, TFT_BLACK);
        last_state = state_select_color;
    }

    tft.setTextFont(2);
    tft.setTextColor(TFT_WHITE);
    if(state_select_color)
    {
        tft.drawString("Please select a color!", 50, y_pos);
    }
    else
    {
        tft.drawString("Please select a side!", 50, y_pos);
    }
    if(!already_drew_circle)
    {
        this->drawSide(static_cast<Bobbycar_Side>(selected_side), TFT_GOLD);
        already_drew_circle = true;
    }
}

void LedstripColorsMenu::rotate(int offset)
{
    if (offset < 0)
    {
        if (state_select_color)
        {
            selected_color++;
            if (selected_color > 7)
            {
                selected_color = 0;
            }
        }
        else
        {
            selected_side++;
            if (selected_side > 7)
            {
                selected_side = 0;
            }
        }
    }
    else if (offset > 0)
    {
        if (state_select_color)
        {
            selected_color--;
            if (selected_color < 0)
            {
                selected_color = 7;
            }
        }
        else
        {
            selected_side--;
            if (selected_side < 0)
            {
                selected_side = 7;
            }
        }
    }

    if (state_select_color)
    {
        this->drawColors();
    }
    else
    {
        tft.fillRect(0, 228, tft.width(), ((tft.width() - 40) / 8) + 4, TFT_BLACK);
        this->clearSides();
        this->drawSide(static_cast<Bobbycar_Side>(selected_side), TFT_GOLD);
    }
}

void LedstripColorsMenu::confirm()
{
    if(!state_select_color)
    {
        state_select_color = true;
        this->drawColors();
    }
    else
    {
        ledstrip_custom_colors[selected_side] = Colors[selected_color];
        // Uncomment to close select color menu on color select
        /*
        state_select_color = false;
        tft.fillRect(0, 228, tft.width(), ((tft.width() - 40) / 8) + 4, TFT_BLACK);
        */
    }
}

void LedstripColorsMenu::drawColors()
{
    uint16_t width = (tft.width() - 40);
    auto cube_width = width / 8;

    tft.fillRect(0, 228, tft.width(), cube_width + 4, TFT_BLACK);
    tft.fillRect(21, 231, width - 1, cube_width - 1, TFT_WHITE);

    tft.fillRect(20 + (selected_color * cube_width - 1), 228, cube_width + 4, cube_width + 4, TFT_YELLOW);
    for (int index = 0; index < 8; index++)
    {
        auto offset = index * (cube_width);
        tft.fillRect(22 + offset, 232, cube_width - 4, cube_width - 4, tft_colors[index]);
    }
}

void LedstripColorsMenu::clearSides()
{
    for(int index = 0; index < 8; index++)
    {
        this->drawSide(static_cast<Bobbycar_Side>(index), TFT_BLACK);
    }
}

void LedstripColorsMenu::drawSide(Bobbycar_Side side, unsigned int color)
{
    const auto middle = tft.width() / 2;
    const auto width = bobbyicons::bobbycar.WIDTH;
    const auto height = bobbyicons::bobbycar.HEIGHT;
    const auto left = middle - (width / 2);
    const auto right = middle + (width / 2);
    const auto above = 50;
    const auto bellow = above + 10 + bobbyicons::bobbycar.HEIGHT;

    switch (side) {
        case Bobbycar_Side::FRONT:
            tft.fillRect(left, above, width, 5, color);
            break;
        case Bobbycar_Side::FRONT_LEFT:
            tft.fillRect(left - 10, above + 10, 5, height / 2, color);
            tft.fillRect(left, above, width / 2, 5, color);
            break;
        case Bobbycar_Side::LEFT:
            tft.fillRect(left - 10, above + 10, 5, height, color);
            break;
        case Bobbycar_Side::BACK_LEFT:
            tft.fillRect(left - 10, above + 10 + (height / 2), 5, height / 2, color);
            tft.fillRect(left, bellow + 5, width / 2, 5, color);
            break;
        case Bobbycar_Side::BACK:
            tft.fillRect(left, bellow + 5, width, 5, color);
            break;
        case Bobbycar_Side::BACK_RIGHT:
            tft.fillRect(right + 5, above + 10 + (height / 2), 5, height / 2, color);
            tft.fillRect(middle, bellow + 5, width / 2, 5, color);
            break;
        case Bobbycar_Side::RIGHT:
            tft.fillRect(right + 5, above + 10, 5, height, color);
            break;
        case Bobbycar_Side::FRONT_RIGHT:
            tft.fillRect(right + 5, above + 10, 5, height / 2, color);
            tft.fillRect(middle, above, width / 2, 5, color);
            break;
    }
    // tft.fillCircle(tft.width() / 2, 140, 100, TFT_BLACK);
}

} // Namespace
