#include "poweroffdisplay.h"

// 3rdparty lib includes
#include <screenmanager.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "globals.h"

using namespace std::chrono_literals;

namespace bobby {

void PoweroffDisplay::start()
{
    Base::start();

    m_startTime = espchrono::millis_clock::now();

    for (Controller &controller : controllers)
        controller.command.poweroff = true;
}

void PoweroffDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    tft.fillScreen(espgui::TFT_BLACK);

    tft.drawString("Poweroff", 5, 5, espgui::TFT_YELLOW, espgui::TFT_BLACK, 4);

    tft.fillRect(0, 34, tft.width(), 3, espgui::TFT_WHITE);

    tft.drawString("Trying to turn off", 15, 50, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    tft.drawString("both controllers", 25, 75, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    tft.drawString("Please stand still...", 20, 125, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
}

void PoweroffDisplay::update()
{
    Base::update();

    if (espchrono::millis_clock::now() - m_startTime >= 1000ms)
        espgui::popScreen();
}

void PoweroffDisplay::stop()
{
    Base::stop();

    for (Controller &controller : controllers)
        controller.command.poweroff = false;
}
} // namespace bobby
