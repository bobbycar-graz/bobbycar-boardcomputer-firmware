#include "resetnvsscreen.h"

// esp-idf includes
#include <esp_system.h>

// 3rdparty lib includes
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "newsettings.h"

namespace bobby {
void ResetNVSScreen::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    tft.drawString("Rebooting...", 0, 50, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    configs.reset();

    esp_restart();
}

std::string ResetNVSScreen::text() const
{
    return "Reset NVS";
}

void ResetNVSScreen::buttonPressed(espgui::Button button) {}

void ResetNVSScreen::buttonReleased(espgui::Button button) {}
} // namespace bobby
