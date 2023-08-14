#include "ledstripcolorsdisplay.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <TFT_eSPI.h>
#include <cpputils.h>
#include <menuitem.h>
#include <actioninterface.h>
#include <screenmanager.h>
#include <actions/dummyaction.h>
#include <fontrenderer.h>

// local includes
#include "utils.h"
#include "icons/back.h"
#include "icons/bobbycar.h"
#include "globals.h"

namespace {
constexpr char TEXT_LEDSTRIPCOLORMENU[] = "Customize Ledstrip";

int8_t selected_side = 7;
int8_t selected_color;
bool state_select_color{false};
bool last_state = {false};

const std::array<uint32_t, 8> Colors = {
    CRGB_TO_UINT32(CRGB{0,0,0}),
    CRGB_TO_UINT32(CRGB{255,255,255}),
    CRGB_TO_UINT32(CRGB{255,0,0}),
    CRGB_TO_UINT32(CRGB{255,255,0}),
    CRGB_TO_UINT32(CRGB{0,255,0}),
    CRGB_TO_UINT32(CRGB{0,255,255}),
    CRGB_TO_UINT32(CRGB{0,0,255}),
    CRGB_TO_UINT32(CRGB{255,0,255})
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

std::string LedstripColorsDisplay::title() const
{
    return TEXT_LEDSTRIPCOLORMENU;
}

void LedstripColorsDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    tft.pushImage(70, 60, bobbyicons::bobbycar.WIDTH, bobbyicons::bobbycar.HEIGHT, bobbyicons::bobbycar.buffer);
}

void LedstripColorsDisplay::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    auto y_pos = ((tft.width() - 40) / 8 + 4) + 240;
    if (last_state != state_select_color)
    {
        tft.fillRect(0,y_pos - 1, tft.width(), 20, TFT_BLACK);
        last_state = state_select_color;
    }

    espgui::FontRenderer fontRenderer{tft};
    fontRenderer.drawString(state_select_color ?
                                "Please select a color!" :
                                "Please select a side!", 50, y_pos,
                            espgui::TFT_WHITE, espgui::TFT_BLACK,
                            2);

    if (!already_drew_circle)
    {
        drawSide(tft, static_cast<Bobbycar_Side>(selected_side), TFT_GOLD);
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
            espgui::popScreen();
        }
        else
        {
            state_select_color = false;
            tft.fillRect(0, 228, tft.width(), ((tft.width() - 40) / 8) + 4, TFT_BLACK);
        }

        break;
    case Button::Right:
        if(!state_select_color)
        {
            state_select_color = true;
            drawColors(tft);
        }
        else
        {
            configs.ledstrip.custom_color[selected_side].write(configs.nvs_handle_user, Colors[selected_color]);
            // Uncomment to close select color menu on color select
            /*
        state_select_color = false;
        tft.fillRect(0, 228, tft.width(), ((tft.width() - 40) / 8) + 4, TFT_BLACK);
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
            drawColors(tft);
        }
        else
        {
            tft.fillRect(0, 228, tft.width(), ((tft.width() - 40) / 8) + 4, TFT_BLACK);
            clearSides();
            drawSide(tft, static_cast<Bobbycar_Side>(selected_side), TFT_GOLD);
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
            drawColors(tft);
        }
        else
        {
            tft.fillRect(0, 228, tft.width(), ((tft.width() - 40) / 8) + 4, TFT_BLACK);
            clearSides();
            drawSide(static_cast<Bobbycar_Side>(selected_side), TFT_GOLD);
        }

        break;
    }
}

void LedstripColorsDisplay::drawColors(espgui::TftInterface &tft)
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

void LedstripColorsDisplay::clearSides()
{
    for(int index = 0; index < 8; index++)
    {
        drawSide(static_cast<Bobbycar_Side>(index), TFT_BLACK);
    }
}

void LedstripColorsDisplay::drawSide(espgui::TftInterface &tft, Bobbycar_Side side, unsigned int color)
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
