#pragma once

#include "globals.h"
#include "utils.h"

namespace {
class VerticalMeter
{
public:
    VerticalMeter(const char *text, const char *format, int x, int y);

    void start();
    void redraw(float value, float min, float max);

private:
    const char * const m_text;
    const char * const m_format;
    const int m_x;
    const int m_y;

    float m_oldValue{};
};

VerticalMeter::VerticalMeter(const char *text, const char *format, int x, int y) :
    m_text{text}, m_format{format}, m_x{x}, m_y{y}
{
}

void VerticalMeter::start()
{
    int w = 36;
    tft.drawRect(m_x, m_y, w, 155, TFT_GREY);
    tft.fillRect(m_x + 2, m_y + 19, w - 3, 155 - 38, TFT_WHITE);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawCentreString(m_text, m_x + w / 2, m_y + 2, 2);

    for (int i = 0; i < 110; i += 10)
        tft.drawFastHLine(m_x + 20, m_y + 27 + i, 6, TFT_BLACK);

    for (int i = 0; i < 110; i += 50)
        tft.drawFastHLine(m_x + 20, m_y + 27 + i, 9, TFT_BLACK);

    tft.fillTriangle(m_x + 3, m_y + 127, m_x + 3 + 16, m_y + 127, m_x + 3, m_y + 127 - 5, TFT_RED);
    tft.fillTriangle(m_x + 3, m_y + 127, m_x + 3 + 16, m_y + 127, m_x + 3, m_y + 127 + 5, TFT_RED);

    tft.drawCentreString("---", m_x + w / 2, m_y + 155 - 18, 2);
}

void VerticalMeter::redraw(float value, float min, float max)
{
    tft.setTextColor(TFT_GREEN, TFT_BLACK);

    char buf[16];
    snprintf(&buf[0], 16, m_format, value);
    tft.drawRightString(buf, m_x + 36 - 5, 187 - 27 + 155 - 18, 2);

    const int dx = 3 + m_x;
    value = scaleBetween<float>(value, min, max, 0.f, 100.f);

    while (m_oldValue > value)
    {
        const int dy = 187 + 100 - m_oldValue;
        tft.drawLine(dx, dy - 5, dx + 16, dy, TFT_WHITE);
        m_oldValue--;
        tft.drawLine(dx, dy + 6, dx + 16, dy + 1, TFT_RED);
    }

    while (m_oldValue < value)
    {
        const int dy = 187 + 100 - m_oldValue;
        tft.drawLine(dx, dy + 5, dx + 16, dy, TFT_WHITE);
        m_oldValue++;
        tft.drawLine(dx, dy - 6, dx + 16, dy - 1, TFT_RED);
    }
}


}
