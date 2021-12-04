#include "alertdisplay.h"

// 3rdparty lib includes
#include <tftinstance.h>
#include <screenmanager.h>

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

    const auto width = espgui::tft.width() - leftMargin - rightMargin;
    const auto height = espgui::tft.height() - topMargin - bottomMargin;
    const auto right = espgui::tft.width() - rightMargin;
    const auto bottom = espgui::tft.height() - bottomMargin;

    //espgui::tft.drawRect(leftMargin, topMargin, espgui::tft.width() - leftMargin - rightMargin, espgui::tft.height() - topMargin - bottomMargin, TFT_WHITE);
    //espgui::tft.fillRect(leftMargin + 1, topMargin + 1, espgui::tft.width() - leftMargin - rightMargin - 2, espgui::tft.height() - topMargin - bottomMargin - 2, TFT_BLACK);

    espgui::tft.drawSunkenRect(leftMargin, topMargin, width, height,
                               espgui::tft.color565(240, 240, 240),
                               espgui::tft.color565(100, 100, 100),
                               espgui::tft.color565(30, 30, 30));

    espgui::tft.setTextColor(TFT_WHITE, espgui::tft.color565(30, 30, 30));

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

        if (y >= espgui::tft.height() - bottomMargin)
            break;
    }

    espgui::tft.setTextColor(TFT_BLACK, espgui::tft.color565(170, 170, 170));

    espgui::tft.drawSunkenRect(leftMargin + 15, bottom - 40,
                               (width - 15 - 10 - 15) / 2,
                               30,
                               espgui::tft.color565(240, 240, 240),
                               espgui::tft.color565(100, 100, 100),
                               espgui::tft.color565(170, 170, 170));

    espgui::tft.drawString("Yes", leftMargin + 18, bottom - 37);

    espgui::tft.drawSunkenRect(leftMargin + 15 + ((width - 15 - 30 - 15) / 2) + 15, bottom - 40,
                               (width - 15 - 10 - 15) / 2,
                               30,
                               espgui::tft.color565(240, 240, 240),
                               espgui::tft.color565(100, 100, 100),
                               espgui::tft.color565(170, 170, 170));

    espgui::tft.drawString("No", leftMargin + 18 + ((width - 15 - 30 - 15) / 2) + 15 + 1, bottom - 37);
}

void AlertDisplay::closeOverlay()
{
    auto guard = std::move(espgui::currentDisplay);
    espgui::currentDisplay = std::move(m_lastDisplay);
    espgui::currentDisplay->initScreen();
}
