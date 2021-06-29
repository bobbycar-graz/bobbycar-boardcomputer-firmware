#pragma once

#include <esp_system.h>

#include "actioninterface.h"
#include "globals.h"
#include "texts.h"

namespace {
class RebootAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_YELLOW);

        tft.drawString(TEXT_REBOOT, 5, 5, 4);

        tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

        tft.setTextColor(TFT_WHITE);
        tft.drawString("Rebooting now...", 0, 50, 4);

        esp_restart();
    }
};
}
