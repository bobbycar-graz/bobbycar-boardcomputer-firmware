#include "alertdisplay.h"

// 3rdparty lib includes
#include <tftinstance.h>
#include <screenmanager.h>

AlertDisplay::AlertDisplay(std::unique_ptr<Display> &&lastDisplay) :
    m_lastDisplay{std::move(lastDisplay)}
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
    espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);

    espgui::tft.drawRect(20, 50, espgui::tft.width() - 40, espgui::tft.height() - 100, TFT_WHITE);
    espgui::tft.fillRect(21, 51, espgui::tft.width() - 42, espgui::tft.height() - 102, TFT_BLACK);

    espgui::tft.drawString("oida", 25, 60);
    espgui::tft.drawString("oida", 25, 85);
    espgui::tft.drawString("oida", 25, 110);
}

void AlertDisplay::closeOverlay()
{
    auto guard = std::move(espgui::currentDisplay);
    espgui::currentDisplay = std::move(m_lastDisplay);
    espgui::currentDisplay->initScreen();
}
