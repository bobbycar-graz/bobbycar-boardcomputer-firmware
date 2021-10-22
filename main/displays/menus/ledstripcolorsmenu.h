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

using namespace espgui;

namespace  {
    class LedstripMenu;
}

namespace {

static uint8_t ledstrip_side;
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
    uint8_t side_to_crgb_index(uint8_t side);
    void drawColors();
    LedstripColorsMenu() {}
};

uint8_t LedstripColorsMenu::side_to_crgb_index(uint8_t side)
{
    return 0;
}

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
    } else if (offset > 0)
    {
        if (state_select_color)
        {
            selected_color--;
            if (selected_color < 0)
            {
                selected_color = 7;
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
        ledstrip_custom_colors[side_to_crgb_index(ledstrip_side)] = Colors[selected_color];
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

} // Namespace
