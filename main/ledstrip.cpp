#include "ledstrip.h"

// 3rdparty lib includes

// local includes
#include "globals.h"
#include "cpputils.h"
#include "espchrono.h"
#include "ledstripdefines.h"
#include "utils.h"
#include "ota.h"
#include "newsettings.h"

using namespace std::chrono_literals;

std::vector<CRGB> leds;
uint8_t gHue = 0;

uint16_t blinkAnimation = LEDSTRIP_OVERWRITE_NONE;


void initLedStrip()
{
    if (configs.feature.ledstrip.value)
    {
        leds.resize(configs.ledstrip.ledsCount.value);
        FastLED.addLeds<NEOPIXEL, PINS_LEDSTRIP>(&*std::begin(leds), leds.size())
            .setCorrection(TypicalSMD5050);
    }
}

void updateLedStrip()
{
    if(!configs.feature.ledstrip.value)
            return;

    EVERY_N_MILLISECONDS( 20 ) { gHue++; }
    static bool have_disabled_beeper = false;
    const bool enAnim = configs.ledstrip.enableAnimBlink.value;

    if (cpputils::is_in(blinkAnimation, LEDSTRIP_OVERWRITE_BLINKLEFT, LEDSTRIP_OVERWRITE_BLINKRIGHT, LEDSTRIP_OVERWRITE_BLINKBOTH))
    {
        std::fill(std::begin(leds), std::end(leds), CRGB{0, 0, 0});
        if (espchrono::utc_clock::now().time_since_epoch() % 750ms < 375ms || enAnim)
        {
            const auto anim_to_fill = time_to_percent(750ms, 500ms, 100ms, configs.ledstrip.enableFullBlink.value ? (leds.size() / 2) : configs.ledstrip.bigOffset.value - configs.ledstrip.smallOffset.value, configs.ledstrip.enableFullBlink.value);
            if (configs.ledstrip.enableBeepWhenBlink.value)
            {
                if (espchrono::utc_clock::now().time_since_epoch() % 750ms < 375ms)
                    for (Controller &controller : controllers)
                        controller.command.buzzer.freq = 3;
                else
                    for (Controller &controller : controllers)
                        controller.command.buzzer.freq = 0;
            }
            auto color = CRGB{255, 200, 0};
            const auto center = (std::begin(leds) + (leds.size() / 2) + configs.ledstrip.centerOffset.value);

            if (configs.ledstrip.enableFullBlink.value)
            {
                // Full
                if (BLINK_LEFT_EXPR)
                {
                    // Blink left
                    if (!enAnim)
                    {
                        std::fill(std::begin(leds), center, color);
                    }
                    else
                    {
                        std::fill(std::begin(leds)+anim_to_fill, center, color);
                    }
                }
                if (BLINK_RIGHT_EXPR)
                {
                    // Blink right
                    if (!enAnim)
                    {
                        std::fill(center, std::end(leds), color);
                    }
                    else
                    {
                        std::fill(center, std::end(leds) - anim_to_fill, color);
                    }
                }
            }
            else
            {
                // Only in the back
                if (BLINK_LEFT_EXPR)
                {
                    // Blink left
                    if (!enAnim)
                    {
                        std::fill(center - configs.ledstrip.bigOffset.value, center - configs.ledstrip.smallOffset.value, color);
                    }
                    else
                    {
                        std::fill(center - configs.ledstrip.smallOffset.value - anim_to_fill, center - configs.ledstrip.smallOffset.value, color);
                    }
                }
                if (BLINK_RIGHT_EXPR)
                {
                    // Blink right
                    if (!enAnim)
                    {
                        std::fill(center + configs.ledstrip.smallOffset.value, center + configs.ledstrip.bigOffset.value, color);
                    }
                    else
                    {
                        std::fill(center + configs.ledstrip.smallOffset.value, center + configs.ledstrip.smallOffset.value + anim_to_fill, color);
                    }
                }
            }
        } else {
            if (configs.ledstrip.enableBeepWhenBlink.value)
            {
                for (Controller &controller : controllers)
                    controller.command.buzzer.freq = 0;
            }
        }
    }
    else
    {
        if (configs.ledstrip.enableBrakeLights.value)
        {
            float avgPwm{};
            for (const Controller &controller : controllers)
            {
                avgPwm +=
                    controller.command.left.pwm * (controller.invertLeft ? -1 : 1) +
                    controller.command.right.pwm * (controller.invertRight ? -1 : 1);
            }
            avgPwm /= 4;

            if (avgPwm < -1.f)
            {
                auto color = avgSpeedKmh < -0.1f ? CRGB{255, 255, 255} : CRGB{255, 0, 0};

                const auto center = (std::begin(leds) + (leds.size() / 2) + configs.ledstrip.centerOffset.value);

                std::fill(std::begin(leds), std::end(leds), CRGB{0, 0, 0});
                if (configs.ledstrip.enableFullBlink.value)
                {
                    std::fill(std::begin(leds), std::end(leds), color);
                }
                else if(!configs.ledstrip.enableAnimBlink.value)
                {
                    std::fill(center - configs.ledstrip.bigOffset.value - 2, center - configs.ledstrip.smallOffset.value + 2, color);
                    std::fill(center + configs.ledstrip.smallOffset.value - 2, center + configs.ledstrip.bigOffset.value + 2, color);
                }
            }
            else
            {
                showAnimation();
            }
        }
        else
        {
            showAnimation();
        }
    }

    if (have_disabled_beeper == false && (!(cpputils::is_in(blinkAnimation, LEDSTRIP_OVERWRITE_BLINKLEFT, LEDSTRIP_OVERWRITE_BLINKRIGHT, LEDSTRIP_OVERWRITE_BLINKBOTH)) || !configs.ledstrip.enableBeepWhenBlink.value))
    {
        for (Controller &controller : controllers)
            controller.command.buzzer.freq = 0;
        have_disabled_beeper = true;
    }
    else if ((cpputils::is_in(blinkAnimation, LEDSTRIP_OVERWRITE_BLINKLEFT, LEDSTRIP_OVERWRITE_BLINKRIGHT, LEDSTRIP_OVERWRITE_BLINKBOTH)) && configs.ledstrip.enableBeepWhenBlink.value) have_disabled_beeper = false;

    if (simplified || configs.ledstrip.enableStVO.value)
    {
       const auto center = (std::begin(leds) + (leds.size() / 2) + configs.ledstrip.centerOffset.value);

       if (!(blinkAnimation == LEDSTRIP_OVERWRITE_BLINKLEFT || blinkAnimation == LEDSTRIP_OVERWRITE_BLINKBOTH) || !(espchrono::utc_clock::now().time_since_epoch() % 750ms < 375ms) || configs.ledstrip.enableFullBlink.value) // Condition for right
       {
           std::fill(center - configs.ledstrip.bigOffset.value, center - configs.ledstrip.smallOffset.value, CRGB{0, 0, 0});
           std::fill(center - configs.ledstrip.bigOffset.value - 1U, center - configs.ledstrip.smallOffset.value - 1U, CRGB{255, 0, 0}); // Right
       }
       if (!(blinkAnimation == LEDSTRIP_OVERWRITE_BLINKRIGHT || blinkAnimation == LEDSTRIP_OVERWRITE_BLINKBOTH) || !(espchrono::utc_clock::now().time_since_epoch() % 750ms < 375ms) || configs.ledstrip.enableFullBlink.value) // Condition for left
       {
           std::fill(center + configs.ledstrip.smallOffset.value, center + configs.ledstrip.bigOffset.value, CRGB{0, 0, 0});
           std::fill(center + configs.ledstrip.smallOffset.value + 1U, center + configs.ledstrip.bigOffset.value + 1U, CRGB{255, 0, 0}); // Left
       }

       if (configs.ledstrip.stvoFrontEnable.value)
       {
       std::fill(std::begin(leds) + configs.ledstrip.stvoFrontOffset.value, std::begin(leds) + configs.ledstrip.stvoFrontOffset.value + configs.ledstrip.stvoFrontLength.value, CRGB{255, 255, 255});
       std::fill(std::end(leds) - configs.ledstrip.stvoFrontOffset.value - configs.ledstrip.stvoFrontLength.value, std::end(leds) - configs.ledstrip.stvoFrontOffset.value, CRGB{255, 255, 255});
       }
    }

    FastLED.setMaxPowerInVoltsAndMilliamps(5, configs.ledstrip.maxMilliamps.value);
    FastLED.setBrightness(configs.ledstrip.brightness.value);
    FastLED.show();
}

void showAnimation()
{
    if (configs.ledstrip.enableLedAnimation.value
        && !simplified
#ifdef FEATURE_OTA
        && !(asyncOtaTaskStarted && configs.ledstrip.otaMode.value != OtaAnimationModes::None)
#endif
        )
    {
        switch (configs.ledstrip.animationType.value)
        {
        case LedstripAnimation::DefaultRainbow: showDefaultLedstrip(); break;
        case LedstripAnimation::BetterRainbow: showBetterRainbow(); break;
        case LedstripAnimation::SpeedSync: showSpeedSyncAnimation(); break;
        case LedstripAnimation::CustomColor: showCustomColor(); break;
        default: showDefaultLedstrip();
        }
    }
#ifdef FEATURE_OTA
    else if (asyncOtaTaskStarted && configs.ledstrip.otaMode.value != OtaAnimationModes::None)
    {
        // show ota animation
        showOtaAnimation();
    }
#endif
    else
    {
        std::fill(std::begin(leds), std::end(leds), CRGB{0, 0, 0});
    }
}

#ifdef FEATURE_OTA
void showOtaAnimation()
{
    std::fill(std::begin(leds), std::end(leds), CRGB{0,0,0});
    const auto leds_count = leds.size();
    float percentage = 0;

    const auto progress = asyncOta->progress();
    if (const auto totalSize = asyncOta->totalSize(); totalSize && *totalSize > 0)
    {
        percentage = (float(progress) / *totalSize * 100);
        if (configs.ledstrip.otaMode.value == OtaAnimationModes::GreenProgressBar)
        {
            int numLeds = (leds_count * percentage) / 100;
            if (numLeds >= leds_count)
            {
                numLeds = leds_count - 1;
            }
            std::fill(std::begin(leds), std::begin(leds) + numLeds, CRGB{0,255,0});
        }
        else if (configs.ledstrip.otaMode.value == OtaAnimationModes::ColorChangeAll)
        {
            const uint8_t redChannel = 255 - (2.55 * percentage);
            const uint8_t greenChannel = 2.55 * percentage;

            std::fill(std::begin(leds), std::end(leds), CRGB{redChannel, greenChannel, 0});
        }
    }
}
#endif

void showBetterRainbow()
{
    fill_rainbow(&*std::begin(leds), leds.size(), gHue);
}

void fill_rainbow_invert_at( struct CRGB * pFirstLED, int numToFill, int invertAtLed,
                  uint8_t initialhue,
                  float deltahue )
{
    float huecalc = initialhue;
    CHSV hsv;
    hsv.hue = initialhue;
    hsv.val = 255;
    hsv.sat = 240;
    for( int i = 0; i < numToFill; i++) {
        hsv.hue = huecalc;
        pFirstLED[i] = hsv;
        if(i>invertAtLed){
            huecalc -= deltahue;
        }else{
            huecalc += deltahue;
        }
    }
}

void showSpeedSyncAnimation()
{
#ifdef LEDS_PER_METER
    const float leds_per_meter = LEDS_PER_METER;
#else
    const float leds_per_meter = 144;
#endif

    static auto last_interval = espchrono::millis_clock::now();
    const auto difference_ms = espchrono::ago(last_interval) / 1ms;

    static float hue_result = 0;

    const float hue_per_led = 1. / std::max(uint8_t(1), uint8_t(configs.ledstrip.animationMultiplier.value));
    const float meter_per_second = avgSpeedKmh / 3.6;
    const float leds_per_second = meter_per_second * leds_per_meter;
    const float hue_per_second = leds_per_second * hue_per_led;

    hue_result += hue_per_second * difference_ms / 1000.f;

    fill_rainbow_invert_at(&*std::begin(leds), leds.size(),leds.size()/2, hue_result,-hue_per_led);

    last_interval = espchrono::millis_clock::now();
}

void showDefaultLedstrip()
{
    fadeToBlackBy(&*std::begin(leds), leds.size(), 20);

    uint8_t dothue = 0;
    for (int i = 0; i < 8; i++)
    {
        leds[beatsin16(i + 7, 0, leds.size())] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}

void showCustomColor()
{
    const auto eighth_length = leds.size() / 8;
    const auto center = (std::begin(leds) + (leds.size() / 2) + configs.ledstrip.centerOffset.value);

    std::fill(std::begin(leds), std::end(leds), configs.ledstrip.custom_color[int(Bobbycar_Side::FRONT)].value); // Front
    std::fill(center - (eighth_length / 2), center + (eighth_length / 2), configs.ledstrip.custom_color[int(Bobbycar_Side::BACK)].value); // Back

#ifdef LEDSTRIP_WRONG_DIRECTION
    std::fill(center + (eighth_length / 2), center + (eighth_length / 2) + eighth_length, configs.ledstrip.custom_color[int(Bobbycar_Side::BACK_LEFT)].value);  // Back Left
    std::fill(center - (eighth_length / 2) - eighth_length, center - (eighth_length / 2), configs.ledstrip.custom_color[int(Bobbycar_Side::BACK_RIGHT)].value); // Back Right
#else
    std::fill(center + (eighth_length / 2), center + (eighth_length / 2) + eighth_length, configs.ledstrip.custom_color[int(Bobbycar_Side::BACK_RIGHT)].value);  // Back Right
    std::fill(center - (eighth_length / 2) - eighth_length, center - (eighth_length / 2), configs.ledstrip.custom_color[int(Bobbycar_Side::BACK_LEFT)].value);   // Back Left
#endif

#ifdef LEDSTRIP_WRONG_DIRECTION
    std::fill(center + (eighth_length / 2) + eighth_length, center + (eighth_length / 2) + eighth_length + eighth_length, configs.ledstrip.custom_color[int(Bobbycar_Side::LEFT)].value);  // Left
    std::fill(center - (eighth_length / 2) - eighth_length - eighth_length, center - (eighth_length / 2) - eighth_length, configs.ledstrip.custom_color[int(Bobbycar_Side::RIGHT)].value); // Right
#else
    std::fill(center + (eighth_length / 2) + eighth_length, center + (eighth_length / 2) + eighth_length + eighth_length, configs.ledstrip.custom_color[int(Bobbycar_Side::RIGHT)].value);  // Right
    std::fill(center - (eighth_length / 2) - eighth_length - eighth_length, center - (eighth_length / 2) - eighth_length, configs.ledstrip.custom_color[int(Bobbycar_Side::LEFT)].value);   // Left
#endif

#ifdef LEDSTRIP_WRONG_DIRECTION
    std::fill(center + (eighth_length / 2) + eighth_length + eighth_length, center + (eighth_length / 2) + eighth_length + eighth_length + eighth_length, configs.ledstrip.custom_color[int(Bobbycar_Side::FRONT_LEFT)].value);  // Front Left
    std::fill(center - (eighth_length / 2) - eighth_length - eighth_length - eighth_length, center - (eighth_length / 2) - eighth_length - eighth_length, configs.ledstrip.custom_color[int(Bobbycar_Side::FRONT_RIGHT)].value); // Front Right
#else
    std::fill(center + (eighth_length / 2) + eighth_length + eighth_length, center + (eighth_length / 2) + eighth_length + eighth_length + eighth_length, configs.ledstrip.custom_color[int(Bobbycar_Side::FRONT_RIGHT)].value);  // Front Right
    std::fill(center - (eighth_length / 2) - eighth_length - eighth_length - eighth_length, center - (eighth_length / 2) - eighth_length - eighth_length, configs.ledstrip.custom_color[int(Bobbycar_Side::FRONT_LEFT)].value);   // Front Left
#endif
}
