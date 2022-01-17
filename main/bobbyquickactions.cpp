#include "bobbyquickactions.h"

// local includes
#ifdef FEATURE_ESPNOW
#include "espnowfunctions.h"
#endif
#include "newsettings.h"
#include "wifi_bobbycar.h"

namespace quickactions {

void handle_bobby_quickaction(espgui::Button button)
{
    espconfig::ConfigWrapper<BobbyQuickActions> *config = nullptr;
    switch (BobbyButton(button))
    {
    case Left2:
        config = &configs.quickActionLeft2;
        break;
    case Right2:
        config = &configs.quickActionRight2;
        break;
    case Up2:
        config = &configs.quickActionUp2;
        break;
    case Down2:
        config = &configs.quickActionDown2;
        break;
    default:
        return;
    }

    if (config)
    {
        switch(config->value)
        {
        case BobbyQuickActions::BLINK_LEFT:
            blink_left();
            break;
        case BobbyQuickActions::BLINK_RIGHT:
            blink_right();
            break;
        case BobbyQuickActions::HANDBREMSE:
            handle_handbremse();
            break;
        case BobbyQuickActions::OPEN_GARAGE:
            open_garage();
            break;
        case BobbyQuickActions::WIFI_SCAN:
            action_wifi_scan();
            break;
        default:
            return;
        }
    }
}

void open_garage()
{
#ifdef FEATURE_ESPNOW
    if (!espnow::espnow_init_allowed())
        return;
    for (const auto &config : configs.wireless_door_configs)
    {
        if (const auto error = espnow::send_espnow_message(fmt::format("BOBBYOPEN:{}:{}", config.doorId.value, config.doorToken.value)); error != ESP_OK)
        {
            ESP_LOGE("BOBBY", "send_espnow_message() failed with: %s", esp_err_to_name(error));
            continue;
        }
    }
#endif
}

void action_wifi_scan()
{
    if (const auto result = wifi_scan(); result != ESP_OK)
    {
        ESP_LOGE("BOBBY", "wifi_scan() failed with %s", esp_err_to_name(result));
        return;
    }
}

void blink_left()
{
    if (configs.feature.ledstrip.value)
    {
        if (blinkAnimation == LEDSTRIP_OVERWRITE_NONE) //transition from off to left
        {
            blinkAnimation = LEDSTRIP_OVERWRITE_BLINKLEFT;
        }
        else if (blinkAnimation == LEDSTRIP_OVERWRITE_BLINKRIGHT) // transition to warning
        {
            blinkAnimation = LEDSTRIP_OVERWRITE_BLINKBOTH;
        }
        else // transition to off
        {
            blinkAnimation = LEDSTRIP_OVERWRITE_NONE;
        }
    }
}

void blink_right()
{
    if(configs.feature.ledstrip.value)
    {
        if (blinkAnimation == LEDSTRIP_OVERWRITE_NONE) //transition from off to right
        {
            blinkAnimation = LEDSTRIP_OVERWRITE_BLINKRIGHT;
        }
        else if (blinkAnimation == LEDSTRIP_OVERWRITE_BLINKLEFT) // transition to warning
        {
            blinkAnimation = LEDSTRIP_OVERWRITE_BLINKBOTH;
        }
        else // transition to off
        {
            blinkAnimation = LEDSTRIP_OVERWRITE_NONE;
        }
    }
}

void handle_handbremse()
{
    if (configs.handbremse.enable.value)
    {
        using namespace handbremse;
        if (stateWish == StateWish::brake || angezogen)
            stateWish = StateWish::release;
        else
            stateWish = StateWish::brake;
        wishTimer = espchrono::millis_clock::now();
    }
}

void batteryDebugPrint()
{
    ESP_LOGI("BATTERY", "");
}

} // namespace quickactions
