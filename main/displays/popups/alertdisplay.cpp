#include "alertdisplay.h"

// 3rdparty lib includes
#include <tftinstance.h>
#include <screenmanager.h>
#include <fmt/core.h>

// local includes
#include "utils.h"

AlertDisplay::AlertDisplay(std::string &&message, std::unique_ptr<Display> &&lastDisplay) :
    m_message{std::move(message)}, m_lastDisplay{std::move(lastDisplay)}
{
}

void AlertDisplay::initScreen()
{
    m_lastDisplay->initScreen();

    initOverlay();
}

void AlertDisplay::confirm()
{
    closeOverlay();
}

void AlertDisplay::back()
{
    closeOverlay();
}

void AlertDisplay::initOverlay()
{
    constexpr auto leftMargin = 20;
    constexpr auto rightMargin = leftMargin;
    constexpr auto topMargin = 50;
    constexpr auto bottomMargin = topMargin;

    //espgui::tft.drawRect(leftMargin, topMargin, espgui::tft.width() - leftMargin - rightMargin, espgui::tft.height() - topMargin - bottomMargin, TFT_WHITE);
    //espgui::tft.fillRect(leftMargin + 1, topMargin + 1, espgui::tft.width() - leftMargin - rightMargin - 2, espgui::tft.height() - topMargin - bottomMargin - 2, TFT_BLACK);

    espgui::tft.drawSunkenRect(leftMargin, topMargin,
                               espgui::tft.width() - leftMargin - rightMargin,
                               espgui::tft.height() - topMargin - bottomMargin,
                               espgui::tft.color565(240, 240, 240),
                               espgui::tft.color565(100, 100, 100),
                               espgui::tft.color565(40, 40, 40));

    espgui::tft.setTextColor(TFT_WHITE, espgui::tft.color565(40, 40, 40));

    std::string _message{};
    std::vector<std::string> concated{};

    std::vector<std::string> lines{};
    str_split(m_message, lines, ' ');

    int line_width = 0;

    for (int i=0;i<lines.size();i++) {
        const auto line = lines[i];
        const auto line_length = espgui::tft.textWidth(line, 4);
        if (line_width + line_length > espgui::tft.width())
        {
            concated.push_back(fmt::format("{}\n", line));
            line_width = 0;
        }
        else
        {
            if (line_width > 0)
                line_width++;
            line_width += line_length;
            concated.push_back(fmt::format("{} ", line));
        }
    }


    int x = leftMargin + 5;
    int y = topMargin + 5;
    for (char c : m_message)
    {
        if (c == '\n' || x > espgui::tft.width() - rightMargin - 10)
        {
            x = leftMargin + 5;
            y += espgui::tft.fontHeight(4);
        }

        if (c != '\n')
        {
            const auto addedWidth = espgui::tft.drawChar(espgui::tft.decodeUTF8(c), x, y, 4);
            x += addedWidth;
        }
    }
}

void AlertDisplay::closeOverlay()
{
    auto guard = std::move(espgui::currentDisplay);
    espgui::currentDisplay = std::move(m_lastDisplay);
    espgui::currentDisplay->initScreen();
}
