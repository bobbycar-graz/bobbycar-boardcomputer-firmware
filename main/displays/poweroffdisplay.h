#pragma once

// 3rdparty lib includes
#include <espchrono.h>

// local includes
#include "display.h"
#include "utils.h"
#include "globals.h"
#include "texts.h"

namespace {
class MainMenu;
}

namespace {
class PoweroffDisplay : public Display, public DummyConfirm, public DummyBack
{
public:
    void start() override;
    void initScreen() override;
    void update() override;
    void redraw() override {};
    void stop() override;

private:
    espchrono::millis_clock::time_point m_startTime;
};

void PoweroffDisplay::start()
{
    m_startTime = espchrono::millis_clock::now();

    for (Controller &controller : controllers)
        controller.command.poweroff = true;
}

void PoweroffDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_YELLOW);

    tft.drawString(TEXT_POWEROFF, 5, 5, 4);

    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    tft.setTextColor(TFT_WHITE);
    tft.drawString("Trying to turn off", 15, 50, 4);
    tft.drawString("both controllers", 25, 75, 4);
    tft.drawString("Please stand still...", 20, 125, 4);
}

void PoweroffDisplay::update()
{
    if (espchrono::millis_clock::now() - m_startTime >= 1000ms)
        switchScreen<MainMenu>();
}

void PoweroffDisplay::stop()
{
    for (Controller &controller : controllers)
        controller.command.poweroff = false;
}
}
