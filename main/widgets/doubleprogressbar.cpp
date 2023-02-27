#include "doubleprogressbar.h"

// 3rdparty lib includes
#include <cpputils.h>
#include <tftcolors.h>
#include <tftinterface.h>

using namespace espgui;

namespace bobby {
DoubleProgressBar::DoubleProgressBar(int x, int y, int width, int height, int min, int max, uint32_t color1, uint32_t color2) :
    m_x{x}, m_y{y}, m_width{width}, m_height{height}, m_min{min}, m_max{max}, m_color1{color1}, m_color2{color2}
{
}

void DoubleProgressBar::start(espgui::TftInterface &tft)
{
    m_lastValue1 = m_x+1;
    m_lastValue2 = m_x+1;
    tft.drawRect(m_x, m_y, m_width, m_height, espgui::TFT_WHITE);
}

void DoubleProgressBar::redraw(espgui::TftInterface &tft, int value1, int value2)
{
    value1 = cpputils::mapValueClamped(value1, m_min, m_max, m_x+1, m_x+m_width-1);
    value2 = cpputils::mapValueClamped(value2, m_min, m_max, m_x+1, m_x+m_width-1);

    if (value1 < m_lastValue1)
        tft.fillRect(value1, m_y+1, m_lastValue1-value1, (m_height-2) / 2, espgui::TFT_BLACK);
    else if (value1 > m_lastValue1)
        tft.fillRect(m_lastValue1, m_y+1, value1-m_lastValue1, (m_height-2) / 2, m_color1);

    if (value2 < m_lastValue2)
        tft.fillRect(value2, m_y+1+(m_height-2)/2, m_lastValue2-value2, (m_height-2) / 2, espgui::TFT_BLACK);
    else if (value2 > m_lastValue2)
        tft.fillRect(m_lastValue2, m_y+1+(m_height-2)/2, value2-m_lastValue2, (m_height-2) / 2, m_color2);

    m_lastValue1 = value1;
    m_lastValue2 = value2;
}
} // namespace bobby
