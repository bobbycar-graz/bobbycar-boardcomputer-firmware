#pragma once

// system includes
#include <cstdint>

// 3rdparty lib includes
#include <tftinterface.h>

// local includes
#include "guihelpers/bobbydisplay.h"

namespace bobby {

class PingPongDisplay : public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    PingPongDisplay();

    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;
    void stop() override;

    void buttonPressed(espgui::Button button) override;

private:
    void midline(espgui::TftInterface &tft);
    void lpaddle(espgui::TftInterface &tft);
    void rpaddle(espgui::TftInterface &tft);
    void calc_target_y();
    void ball(espgui::TftInterface &tft);

    int16_t h = 240;
    int16_t w = 320;

    int dly = 5;

    int16_t paddle_h = 30;
    int16_t paddle_w = 4;

    int16_t lpaddle_x = 0;
    int16_t rpaddle_x = w - paddle_w;

    int16_t lpaddle_y = 0;
    int16_t rpaddle_y = h - paddle_h;

    int16_t lpaddle_d = 1;
    int16_t rpaddle_d = -1;

    int16_t lpaddle_ball_t = w - w / 4;
    int16_t rpaddle_ball_t = w / 4;

    int16_t target_y = 0;

    int16_t ball_x = 2;
    int16_t ball_y = 2;
    int16_t oldball_x = 2;
    int16_t oldball_y = 2;

    int16_t ball_dx = 1;
    int16_t ball_dy = 1;

    int16_t ball_w = 6;
    int16_t ball_h = 6;

    int16_t dashline_h = 4;
    int16_t dashline_w = 2;
    int16_t dashline_n = h / dashline_h;
    int16_t dashline_x = w / 2 - 1;
    int16_t dashline_y = dashline_h / 2;

    int16_t lscore = 12;
    int16_t rscore = 4;

    static const constexpr auto BLACK = 0x0000;
    static const constexpr auto WHITE = 0xFFFF;
    static const constexpr auto GREY = 0x5AEB;
};

} // namespace bobby
