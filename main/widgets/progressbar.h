#pragma once

#include "globals.h"
#include "utils.h"

namespace {
class ProgressBar
{
public:
    ProgressBar(int x, int y, int width, int height, int min, int max, uint32_t color=TFT_YELLOW);

    void start();
    void redraw(int value);

private:
    const int m_x;
    const int m_y;
    const int m_width;
    const int m_height;
    const int m_min;
    const int m_max;
    const uint32_t m_color;

    int m_lastValue;
};

ProgressBar::ProgressBar(int x, int y, int width, int height, int min, int max, uint32_t color) :
    m_x{x}, m_y{y}, m_width{width}, m_height{height}, m_min{min}, m_max{max}, m_color{color}
{
}

void ProgressBar::start()
{
    m_lastValue = m_x+1;
    tft.drawRect(m_x, m_y, m_width, m_height, TFT_WHITE);
}

void ProgressBar::redraw(int value)
{
    value = scaleBetween(value, m_min, m_max, m_x+1, m_x+m_width-1);

    if (value < m_lastValue)
        tft.fillRect(value, m_y+1, m_lastValue-value, m_height-2, TFT_BLACK);
    else if (value > m_lastValue)
        tft.fillRect(m_lastValue, m_y+1, value-m_lastValue, m_height-2, m_color);

    m_lastValue = value;
}

}
