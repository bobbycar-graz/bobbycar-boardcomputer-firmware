#pragma once

#include <string>

#include "globals.h"

namespace {
class Label
{
public:
    Label(int x, int y);

    int x() const { return m_x; };
    int y() const { return m_y; };

    void start();
    void redraw(const std::string &str, bool forceRedraw = false);
    void clear();

private:
    const int m_x;
    const int m_y;

    std::string m_lastStr;
    int m_lastFont;
    int m_lastColor;

    int m_lastWidth;
    int m_lastHeight;
};

Label::Label(int x, int y) :
    m_x{x},
    m_y{y}
{
}

void Label::start()
{
    m_lastStr.clear();
    m_lastFont = -1;
    m_lastColor = -1;

    m_lastWidth = 0;
    m_lastHeight = 0;
}

void Label::redraw(const std::string &str, bool forceRedraw)
{
    if (m_lastStr == str &&
        m_lastFont == tft.textfont &&
        m_lastColor == tft.textcolor &&
        !forceRedraw)
        return;

    const auto renderedWidth = tft.drawString(str.c_str(), m_x, m_y);
    const auto renderedHeight = tft.fontHeight();

    if (renderedWidth < m_lastWidth)
        tft.fillRect(m_x + renderedWidth, m_y,
                     m_lastWidth - renderedWidth, m_lastHeight,
                     tft.textbgcolor);

    if (renderedHeight < m_lastHeight)
        tft.fillRect(m_x, m_y + renderedHeight,
                     renderedWidth, m_lastHeight - renderedHeight,
                     tft.textbgcolor);

    m_lastStr = str;
    m_lastFont = tft.textfont;
    m_lastColor = tft.textcolor;

    m_lastWidth = renderedWidth;
    m_lastHeight = renderedHeight;
}

void Label::clear()
{
    if (m_lastWidth || m_lastHeight)
        tft.fillRect(m_x, m_y, m_lastWidth, m_lastHeight, tft.textbgcolor);

    start();
}
}
