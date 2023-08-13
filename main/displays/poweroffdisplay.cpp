#include "poweroffdisplay.h"

// 3rdparty lib includes
#include <screenmanager.h>

// local includes
#include "utils.h"
#include "globals.h"

using namespace std::chrono_literals;

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

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_YELLOW);

    tft.drawString("Poweroff", 5, 5, 4);

    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    tft.setTextColor(TFT_WHITE);
    tft.drawString("Trying to turn off", 15, 50, 4);
    tft.drawString("both controllers", 25, 75, 4);
    tft.drawString("Please stand still...", 20, 125, 4);
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
