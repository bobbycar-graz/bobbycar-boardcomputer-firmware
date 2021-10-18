#include "rebootaction.h"

// esp-idf includes
#include <esp_system.h>

// 3rdparty lib includes
#include <tftinstance.h>

// local includes
#include "globals.h"
#include "texts.h"

using namespace espgui;

void RebootAction::triggered()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_YELLOW);

    tft.drawString(TEXT_REBOOT, 5, 5, 4);

    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    tft.setTextColor(TFT_WHITE);
    tft.drawString("Rebooting now...", 0, 50, 4);

    esp_restart();
}
