#include "rebootscreen.h"

// esp-idf includes
#include <esp_system.h>

// 3rdparty lib includes
#include <tftinterface.h>
#include <tftcolors.h>
#include <fontrenderer.h>

void RebootScreen::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    espgui::FontRenderer fontRenderer{tft};

    fontRenderer.drawString("Rebooting now...", 0, 50, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    esp_restart();
}

std::string RebootScreen::title() const
{
    return "Reboot";
}

void RebootScreen::buttonPressed(espgui::Button button)
{

}

void RebootScreen::buttonReleased(espgui::Button button)
{

}
