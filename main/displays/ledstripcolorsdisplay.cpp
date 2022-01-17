#include "ledstripcolorsdisplay.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <TFT_eSPI.h>
#include <cpputils.h>
#include <menuitem.h>
#include <actions/switchscreenaction.h>
#include <actioninterface.h>
#include <tftinstance.h>
#include <screenmanager.h>

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "icons/back.h"
#include "icons/bobbycar.h"
#include "actions/dummyaction.h"
#include "globals.h"
#include "displays/menus/ledstripmenu.h"

namespace {
constexpr char TEXT_LEDSTRIPCOLORMENU[] = "Customize Ledstrip";

int8_t selected_side = 7;
int8_t selected_color;
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
} // namespace

std::string LedstripColorsDisplay::text() const
{
    return TEXT_LEDSTRIPCOLORMENU;
}

void LedstripColorsDisplay::initScreen()
{
    Base::initScreen();

    espgui::tft.setSwapBytes(true);
    espgui::tft.pushImage(70, 60, bobbyicons::bobbycar.WIDTH, bobbyicons::bobbycar.HEIGHT, bobbyicons::bobbycar.buffer);
    espgui::tft.setSwapBytes(false);
}

void LedstripColorsDisplay::redraw()
{
    Base::redraw();

    auto y_pos = ((espgui::tft.width() - 40) / 8 + 4) + 240;
    if (last_state != state_select_color)
    {
        espgui::tft.fillRect(0,y_pos - 1, espgui::tft.width(), 20, TFT_BLACK);
        last_state = state_select_color;
    }

    espgui::tft.setTextFont(2);
    espgui::tft.setTextColor(TFT_WHITE);

    espgui::tft.drawString(state_select_color ?
                       "Please select a color!" :
                       "Please select a side!", 50, y_pos);

    if (!already_drew_circle)
    {
        drawSide(static_cast<Bobbycar_Side>(selected_side), TFT_GOLD);
        already_drew_circle = true;
    }
}

void LedstripColorsDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    using espgui::Button;
    case Button::Left:
        if(!state_select_color)
        {
            espgui::switchScreen<LedstripMenu>();
        }
        else
        {
            state_select_color = false;
            espgui::tft.fillRect(0, 228, espgui::tft.width(), ((espgui::tft.width() - 40) / 8) + 4, TFT_BLACK);
        }

        break;
    case Button::Right:
        if(!state_select_color)
        {
            state_select_color = true;
            drawColors();
        }
        else
        {
            ledstrip_custom_colors[selected_side] = Colors[selected_color];
            // Uncomment to close select color menu on color select
            /*
        state_select_color = false;
        espgui::tft.fillRect(0, 228, espgui::tft.width(), ((espgui::tft.width() - 40) / 8) + 4, TFT_BLACK);
        */
        }
        break;
    case Button::Up:
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

        /* TODO commander: move into redraw method */
        if (state_select_color)
        {
            drawColors();
        }
        else
        {
            espgui::tft.fillRect(0, 228, espgui::tft.width(), ((espgui::tft.width() - 40) / 8) + 4, TFT_BLACK);
            clearSides();
            drawSide(static_cast<Bobbycar_Side>(selected_side), TFT_GOLD);
        }

        break;
    case Button::Down:
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

        /* TODO commander: move into redraw method */
        if (state_select_color)
        {
            drawColors();
        }
        else
        {
            espgui::tft.fillRect(0, 228, espgui::tft.width(), ((espgui::tft.width() - 40) / 8) + 4, TFT_BLACK);
            clearSides();
            drawSide(static_cast<Bobbycar_Side>(selected_side), TFT_GOLD);
        }

        break;
    }
}

void LedstripColorsDisplay::drawColors()
{
    uint16_t width = (espgui::tft.width() - 40);
    auto cube_width = width / 8;

    espgui::tft.fillRect(0, 228, espgui::tft.width(), cube_width + 4, TFT_BLACK);
    espgui::tft.fillRect(21, 231, width - 1, cube_width - 1, TFT_WHITE);

    espgui::tft.fillRect(20 + (selected_color * cube_width - 1), 228, cube_width + 4, cube_width + 4, TFT_YELLOW);
    for (int index = 0; index < 8; index++)
    {
        auto offset = index * (cube_width);
        espgui::tft.fillRect(22 + offset, 232, cube_width - 4, cube_width - 4, tft_colors[index]);
    }
}

void LedstripColorsDisplay::clearSides()
{
    for(int index = 0; index < 8; index++)
    {
        drawSide(static_cast<Bobbycar_Side>(index), TFT_BLACK);
    }
}

void LedstripColorsDisplay::drawSide(Bobbycar_Side side, unsigned int color)
{
    const auto middle = espgui::tft.width() / 2;
    const auto width = bobbyicons::bobbycar.WIDTH;
    const auto height = bobbyicons::bobbycar.HEIGHT;
    const auto left = middle - (width / 2);
    const auto right = middle + (width / 2);
    const auto above = 50;
    const auto bellow = above + 10 + bobbyicons::bobbycar.HEIGHT;

    switch (side) {
        case Bobbycar_Side::FRONT:
            espgui::tft.fillRect(left, above, width, 5, color);
            break;
        case Bobbycar_Side::FRONT_LEFT:
            espgui::tft.fillRect(left - 10, above + 10, 5, height / 2, color);
            espgui::tft.fillRect(left, above, width / 2, 5, color);
            break;
        case Bobbycar_Side::LEFT:
            espgui::tft.fillRect(left - 10, above + 10, 5, height, color);
            break;
        case Bobbycar_Side::BACK_LEFT:
            espgui::tft.fillRect(left - 10, above + 10 + (height / 2), 5, height / 2, color);
            espgui::tft.fillRect(left, bellow + 5, width / 2, 5, color);
            break;
        case Bobbycar_Side::BACK:
            espgui::tft.fillRect(left, bellow + 5, width, 5, color);
            break;
        case Bobbycar_Side::BACK_RIGHT:
            espgui::tft.fillRect(right + 5, above + 10 + (height / 2), 5, height / 2, color);
            espgui::tft.fillRect(middle, bellow + 5, width / 2, 5, color);
            break;
        case Bobbycar_Side::RIGHT:
            espgui::tft.fillRect(right + 5, above + 10, 5, height, color);
            break;
        case Bobbycar_Side::FRONT_RIGHT:
            espgui::tft.fillRect(right + 5, above + 10, 5, height / 2, color);
            espgui::tft.fillRect(middle, above, width / 2, 5, color);
            break;
    }
    // espgui::tft.fillCircle(espgui::tft.width() / 2, 140, 100, TFT_BLACK);
}
