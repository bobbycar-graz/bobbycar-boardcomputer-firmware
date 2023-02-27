#include "rebootscreen.h"

// esp-idf includes
#include <esp_system.h>

// 3rdparty lib includes
#include <tftcolors.h>
#include <tftinterface.h>

namespace bobby {
void RebootScreen::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    tft.drawString("Rebooting...", 0, 50, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    esp_restart();
}

std::string RebootScreen::text() const
{
    return "Reboot";
}

void RebootScreen::buttonPressed(espgui::Button button) {}
void RebootScreen::buttonReleased(espgui::Button button) {}
} // namespace bobby
