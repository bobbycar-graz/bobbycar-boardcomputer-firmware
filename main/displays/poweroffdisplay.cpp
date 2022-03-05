#include "poweroffdisplay.h"

// 3rdparty lib includes
#include <tftinstance.h>
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

void PoweroffDisplay::initScreen()
{
    Base::initScreen();

    espgui::tft.fillScreen(TFT_BLACK);
    espgui::tft.setTextColor(TFT_YELLOW);

    espgui::tft.drawString("Poweroff", 5, 5, 4);

    espgui::tft.fillRect(0, 34, espgui::tft.width(), 3, TFT_WHITE);

    espgui::tft.setTextColor(TFT_WHITE);
    espgui::tft.drawString("Trying to turn off", 15, 50, 4);
    espgui::tft.drawString("both controllers", 25, 75, 4);
    espgui::tft.drawString("Please stand still...", 20, 125, 4);
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
