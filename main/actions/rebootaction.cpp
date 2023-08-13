#include "rebootaction.h"

// esp-idf includes
#include <esp_system.h>

// 3rdparty lib includes
#include <TFT_eSPI.h>

void RebootAction::triggered()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_YELLOW);

    tft.drawString("Reboot", 5, 5, 4);

    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    tft.setTextColor(TFT_WHITE);
    tft.drawString("Rebooting now...", 0, 50, 4);

    esp_restart();
}
