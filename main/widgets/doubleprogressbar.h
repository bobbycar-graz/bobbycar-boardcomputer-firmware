#pragma once

// system includes
#include <cstdint>

// 3rdparty lib includes
#include <tftcolors.h>
#include <tftinterface.h>

namespace bobby {
class DoubleProgressBar
{
public:
    DoubleProgressBar(int x, int y, int width, int height, int min, int max, uint32_t color1=espgui::TFT_YELLOW, uint32_t color2=espgui::TFT_YELLOW);

    void start(espgui::TftInterface &tft);
    void redraw(espgui::TftInterface &tft, int value1, int value2);

private:
    const int m_x;
    const int m_y;
    const int m_width;
    const int m_height;
    const int m_min;
    const int m_max;
    const uint32_t m_color1;
    const uint32_t m_color2;

    int m_lastValue1{};
    int m_lastValue2{};
};
} // namespace espgui
