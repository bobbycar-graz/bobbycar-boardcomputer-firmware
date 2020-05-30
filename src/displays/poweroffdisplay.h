#pragma once

#include <Arduino.h>

#include "display.h"
#include "utils.h"
#include "globals.h"
#include "texts.h"

#ifdef GLUMP_CONTROLLER
namespace {
class MainMenu;
}

namespace {
class PoweroffDisplay : public Display
{
public:
    void start() override;
    void initScreen() override;
    void update() override;
    void redraw() override {};
    void stop() override;

private:
    unsigned int m_startTime{};
};

void PoweroffDisplay::start()
{
    m_startTime = millis();

    for (Controller &controller : controllers())
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
    if (millis() - m_startTime >= 1000)
        switchScreen<MainMenu>();
}

void PoweroffDisplay::stop()
{
    for (Controller &controller : controllers())
        controller.command.poweroff = false;
}
}
#endif