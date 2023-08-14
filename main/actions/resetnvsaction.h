#pragma once

// system includes
#include <esp_system.h>

// 3rdparty lib includes
#include <actioninterface.h>
#include <tftcolors.h>

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
            tft.fillScreen(espgui::TFT_BLACK);
            tft.setTextColor(espgui::TFT_YELLOW);

            tft.drawString("Reboot", 5, 5, 4);

            tft.fillRect(0, 34, tft.width(), 3, espgui::TFT_WHITE);

            tft.setTextColor(espgui::TFT_WHITE);
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
