#include "bobbyquickactions.h"

// local includes
#include "espnowfunctions.h"
#include "handbremse.h"
#include "newsettings.h"
#include "tempomat.h"
#include "wifi_bobbycar.h"
#include "bobbyhupe.h"

namespace quickactions {

void handle_bobby_quickaction(espgui::Button button, bool pressed)
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
    case Extra1:
        config = &configs.quickActionExtra1;
        break;
    case Extra2:
        config = &configs.quickActionExtra2;
        break;
    case Extra3:
        config = &configs.quickActionExtra3;
        break;
    case Extra4:
        config = &configs.quickActionExtra4;
        break;
    default:
        return;
    }

    if (pressed)
    {
        switch (config->value()) {
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
            case BobbyQuickActions::PWMOMAT:
                handle_pwmomat();
                break;
            case BobbyQuickActions::HUPE:
                bobbyhupe::activate_hupe();
                break;
            case BobbyQuickActions::COMPRESSOR_TOGGLE:
                bobbyhupe::toggle_compressor();
                break;
            default:
                return;
        }
    }
    else
    {
        switch (config->value()) {
            case BobbyQuickActions::HUPE:
                bobbyhupe::deactivate_hupe();
                break;
            default:
                return;
        }
    }
}

void open_garage()
{
    if (!configs.feature.esp_now.isEnabled.value())
        return;

    if (!espnow::espnow_init_allowed())
        return;
    for (const auto &config : configs.wireless_door_configs)
    {
        if (config.doorId.value().empty() || config.doorToken.value().empty())
            continue;

        if (const auto error = espnow::send_espnow_message(fmt::format("BOBBYOPEN:{}:{}", config.doorId.value(), config.doorToken.value())); error != ESP_OK)
        {
            ESP_LOGE("BOBBY", "send_espnow_message() failed with: %s", esp_err_to_name(error));
            continue;
        }
    }
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
    if (configs.feature.ledstrip.isEnabled.value())
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
    if(configs.feature.ledstrip.isEnabled.value())
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
    if (configs.handbremse.enable.value())
    {
        using StateWish = handbremse::StateWish;
        if (handbremse::stateWish == StateWish::brake || handbremse::angezogen)
            handbremse::stateWish = StateWish::release;
        else
            handbremse::stateWish = StateWish::brake;
        handbremse::wishTimer = espchrono::millis_clock::now();
    }
}

void handle_pwmomat()
{
    if (pwmomat::tempomat_pwm)
    {
        pwmomat::wish = pwmomat::WISH::WISH_DISABLE;
    }
    else
    {
        pwmomat::wish = pwmomat::WISH::WISH_ENABLE;
    }
}

} // namespace quickactions
