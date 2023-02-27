#include "pingpongdisplay.h"

// 3rdparty lib includes
#include <esprandom.h>
#include <randomutils.h>
#include <screenmanager.h>
#include <tftinterface.h>
#include <tftcolors.h>

// local includes
#include "screens.h"

namespace bobby {

PingPongDisplay::PingPongDisplay() :
    lpaddle_y(cpputils::randomNumber<uint8_t>(0, h - paddle_h, espcpputils::esp_random_device{})),
    rpaddle_y(cpputils::randomNumber<uint8_t>(0, h - paddle_h, espcpputils::esp_random_device{})),
    // ball is placed on the center of the left paddle
    ball_y(lpaddle_y + (paddle_h / 2))
{
    calc_target_y();
}

void PingPongDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    disableScreenFlip(true);

    tft.fillScreen(espgui::TFT_BLACK);
    tft.setRotation(1);

    midline(tft);
}

void PingPongDisplay::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    lpaddle(tft);
    rpaddle(tft);

    midline(tft);

    ball(tft);
}

void PingPongDisplay::stop()
{
    Base::stop();

    disableScreenFlip(false);
}

void PingPongDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
        using espgui::Button;
    case Button::Left:
        espgui::popScreen();
        break;
    default:;
    }
}

void PingPongDisplay::midline(espgui::TftInterface &tft)
{
    // If the ball is not on the line then don't redraw the line
    if ((ball_x<dashline_x-ball_w) && (ball_x > dashline_x+dashline_w)) return;

    tft.startWrite();

    // Quick way to draw a dashed line
    tft.setAddrWindow(dashline_x, 0, dashline_w, h);

    for (int16_t i = 0; i < dashline_n; i+=2)
    {
        tft.pushColor(WHITE, dashline_w*dashline_h); // push dash pixels
        tft.pushColor(BLACK, dashline_w*dashline_h); // push gap pixels
    }

    tft.endWrite();
}

void PingPongDisplay::lpaddle(espgui::TftInterface &tft)
{
    if (lpaddle_d == 1)
    {
        tft.fillRect(lpaddle_x, lpaddle_y, paddle_w, 1, BLACK);
    }
    else if (lpaddle_d == -1)
    {
        tft.fillRect(lpaddle_x, lpaddle_y + paddle_h - 1, paddle_w, 1, BLACK);
    }

    lpaddle_y = lpaddle_y + lpaddle_d;

    if (ball_dx == 1)
        lpaddle_d = 0;
    else
    {
        if (lpaddle_y + paddle_h / 2 == target_y) lpaddle_d = 0;
        else if (lpaddle_y + paddle_h / 2 > target_y) lpaddle_d = -1;
        else lpaddle_d = 1;
    }

    if (lpaddle_y + paddle_h >= h && lpaddle_d == 1) lpaddle_d = 0;
    else if (lpaddle_y <= 0 && lpaddle_d == -1) lpaddle_d = 0;

    tft.fillRect(lpaddle_x, lpaddle_y, paddle_w, paddle_h, WHITE);
}

void PingPongDisplay::rpaddle(espgui::TftInterface &tft)
{
    if (rpaddle_d == 1)
        tft.fillRect(rpaddle_x, rpaddle_y, paddle_w, 1, BLACK);
    else if (rpaddle_d == -1)
        tft.fillRect(rpaddle_x, rpaddle_y + paddle_h - 1, paddle_w, 1, BLACK);

    rpaddle_y = rpaddle_y + rpaddle_d;

    if (ball_dx == -1)
        rpaddle_d = 0;
    else
    {
        if (rpaddle_y + paddle_h / 2 == target_y) rpaddle_d = 0;
        else if (rpaddle_y + paddle_h / 2 > target_y) rpaddle_d = -1;
        else rpaddle_d = 1;
    }

    if (rpaddle_y + paddle_h >= h && rpaddle_d == 1)
        rpaddle_d = 0;
    else if (rpaddle_y <= 0 && rpaddle_d == -1)
        rpaddle_d = 0;

    tft.fillRect(rpaddle_x, rpaddle_y, paddle_w, paddle_h, WHITE);
}

void PingPongDisplay::calc_target_y()
{
    int16_t target_x;
    int16_t reflections;
    int16_t y;

    if (ball_dx == 1)
        target_x = w - ball_w;
    else
        target_x = -1 * (w - ball_w);

    y = abs(target_x * (ball_dy / ball_dx) + ball_y);

    reflections = floor(y / h);

    if (reflections % 2 == 0)
        target_y = y % h;
    else
        target_y = h - (y % h);
}

void PingPongDisplay::ball(espgui::TftInterface &tft)
{
    ball_x = ball_x + ball_dx;
    ball_y = ball_y + ball_dy;

    if (ball_dx == -1 && ball_x == paddle_w && ball_y + ball_h >= lpaddle_y && ball_y <= lpaddle_y + paddle_h) {
        ball_dx = ball_dx * -1;
        dly = cpputils::randomNumber<uint8_t>(5, espcpputils::esp_random_device{}); // change speed of ball after paddle contact
        calc_target_y();
    } else if (ball_dx == 1 && ball_x + ball_w == w - paddle_w && ball_y + ball_h >= rpaddle_y && ball_y <= rpaddle_y + paddle_h) {
        ball_dx = ball_dx * -1;
        dly = cpputils::randomNumber<uint8_t>(5, espcpputils::esp_random_device{}); // change speed of ball after paddle contact
        calc_target_y();
    } else if ((ball_dx == 1 && ball_x >= w) || (ball_dx == -1 && ball_x + ball_w < 0)) {
        dly = 5;
    }

    if (ball_y > h - ball_w || ball_y < 0) {
        ball_dy = ball_dy * -1;
        ball_y += ball_dy; // Keep in bounds
    }

    //tft.fillRect(oldball_x, oldball_y, ball_w, ball_h, BLACK);
    tft.drawRect(oldball_x, oldball_y, ball_w, ball_h, BLACK); // Less TFT refresh aliasing than line above for large balls
    tft.fillRect(   ball_x,    ball_y, ball_w, ball_h, WHITE);
    oldball_x = ball_x;
    oldball_y = ball_y;
}
} // namespace bobby
