#include "garagedisplay.h"

// system includes

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <fmt/core.h>
#include <tftinstance.h>
#include <screenmanager.h>

// local includes
#include "displays/menus/mainmenu.h"
#include "globals.h"
#include "texts.h"
#include "utilities/espnowfunctions.h"

#ifdef FEATURE_GARAGE
void GarageDisplay::start()
{
}

void GarageDisplay::initScreen()
{
    espgui::tft.fillScreen(TFT_BLACK);
    espgui::tft.setTextFont(4);
    espgui::tft.setTextColor(TFT_YELLOW);

    espgui::tft.drawString(TEXT_GARAGE, 5, 5, 4);

    espgui::tft.fillRect(0, 34, espgui::tft.width(), 3, TFT_WHITE);

    espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);

    espgui::tft.drawString("Garage", 20, 100);
}

void GarageDisplay::redraw()
{
}

void GarageDisplay::confirm()
{
#ifdef FEATURE_ESPNOW
    if (const auto error = espnow::send_espnow_message(fmt::format("BOBBYOPEN:garage:{}", "TOKEN")); error != ESP_OK)
    {
        ESP_LOGE("BOBBY", "send_espnow_message() failed with: %s", esp_err_to_name(error));
        return;
    }
    espgui::switchScreen<MainMenu>();
#endif
}

void GarageDisplay::back()
{
    espgui::switchScreen<MainMenu>();
}
#endif
