#include "ledstripcolorsdisplay.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <menuitem.h>
#include <screenmanager.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "globals.h"
#include "icons/back.h"
#include "icons/bobbycar.h"
#include "utils.h"

namespace bobby {

namespace {
constexpr char TEXT_LEDSTRIPCOLORMENU[] = "Customize Ledstrip";

int8_t selected_side = 7;
int8_t selected_color;
bool state_select_color{false};
bool last_state = {false};

bool state_draw_colors_needed{false};
bool state_erase_colors_needed{false};
bool state_redraw_side_needed{false};

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
    espgui::TFT_BLACK,
    espgui::TFT_WHITE,
    espgui::TFT_RED,
    espgui::TFT_YELLOW,
    espgui::TFT_GREEN,
    espgui::TFT_CYAN,
    espgui::TFT_BLUE,
    espgui::TFT_MAGENTA
};
} // namespace

std::string LedstripColorsDisplay::text() const
{
    return TEXT_LEDSTRIPCOLORMENU;
}

void LedstripColorsDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    tft.setSwapBytes(true);
    tft.pushImage(70, 60, bobbyicons::bobbycar.WIDTH, bobbyicons::bobbycar.HEIGHT, bobbyicons::bobbycar.buffer);
    tft.setSwapBytes(false);
}

void LedstripColorsDisplay::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    auto y_pos = ((tft.width() - 40) / 8 + 4) + 240;
    if (last_state != state_select_color)
    {
        tft.fillRect(0,y_pos - 1, tft.width(), 20, espgui::TFT_BLACK);
        last_state = state_select_color;
    }

    tft.drawString(state_select_color ?
                       "Please select a color!" :
                       "Please select a side!", 50, y_pos, espgui::TFT_WHITE, espgui::TFT_BLACK, 2);

    if (!already_drew_circle)
    {
        drawSide(tft, static_cast<Bobbycar_Side>(selected_side), espgui::TFT_GOLD);
        already_drew_circle = true;
    }

    if (state_draw_colors_needed)
    {
        drawColors(tft);
        state_draw_colors_needed = false;
    }

    if (state_erase_colors_needed)
    {
        tft.fillRect(0, 228, tft.width(), ((tft.width() - 40) / 8) + 4, espgui::TFT_BLACK);
        state_erase_colors_needed = false;
    }

    if (state_redraw_side_needed)
    {
        tft.fillRect(0, 228, tft.width(), ((tft.width() - 40) / 8) + 4, espgui::TFT_BLACK);
        clearSides(tft);
        drawSide(tft, static_cast<Bobbycar_Side>(selected_side), espgui::TFT_GOLD);
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
            state_erase_colors_needed = true;
        }

        break;
    case Button::Right:
        if(!state_select_color)
        {
            state_select_color = true;
            state_draw_colors_needed = true;
        }
        else
        {
            configs.ledstrip.custom_color[selected_side].write(configs.nvs_handle_user, Colors[selected_color]);
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
            // drawColors();
            state_draw_colors_needed = true;
        }
        else
        {
            // drawSide();
            state_redraw_side_needed = true;
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
            state_draw_colors_needed = true;
        }
        else
        {
            state_redraw_side_needed = true;
        }

        break;
    }
}

void LedstripColorsDisplay::drawColors(espgui::TftInterface &tft)
{
    uint16_t width = (tft.width() - 40);
    auto cube_width = width / 8;

    tft.fillRect(0, 228, tft.width(), cube_width + 4, espgui::TFT_BLACK);
    tft.fillRect(21, 231, width - 1, cube_width - 1, espgui::TFT_WHITE);

    tft.fillRect(20 + (selected_color * cube_width - 1), 228, cube_width + 4, cube_width + 4, espgui::TFT_YELLOW);
    for (int index = 0; index < 8; index++)
    {
        auto offset = index * (cube_width);
        tft.fillRect(22 + offset, 232, cube_width - 4, cube_width - 4, tft_colors[index]);
    }
}

void LedstripColorsDisplay::clearSides(espgui::TftInterface &tft)
{
    for(int index = 0; index < 8; index++)
    {
        drawSide(tft, static_cast<Bobbycar_Side>(index), TFT_BLACK);
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
    // espgui::tft.fillCircle(espgui::tft.width() / 2, 140, 100, TFT_BLACK);
}

} // namespace bobby
