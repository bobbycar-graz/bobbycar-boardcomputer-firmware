#pragma once

// system includes
#include <esp_system.h>

// 3rdparty lib includes
#include <actioninterface.h>

// local includes
#include "newsettings.h"

template<bool reboot>
class ResetNVSAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        if (reboot)
        {
            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_YELLOW);

            tft.drawString("Reboot", 5, 5, 4);

            tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

            tft.setTextColor(TFT_WHITE);
            tft.drawString("Rebooting now...", 0, 50, 4);

            configs.reset();

            esp_restart();
        }
        else
        {
            configs.reset();
        }
    }
};
