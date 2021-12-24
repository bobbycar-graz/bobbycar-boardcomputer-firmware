#include "hardware/ledstrip.h"

// 3rdparty lib includes

// local includes
#include "globals.h"
#include "cpputils.h"
#include "espchrono.h"
#include "helpers/ledstripdefines.h"
#include "utilities/utils.h"
#include "ota.h"

using namespace std::chrono_literals;

#ifdef FEATURE_LEDSTRIP
std::vector<CRGB> leds;
uint8_t gHue = 0;

uint16_t blinkAnimation = LEDSTRIP_OVERWRITE_NONE;
uint16_t animation_type = LedstripAnimation::DefaultRainbow;


void initLedStrip()
{
    animation_type = settings.ledstrip.animationType;
    leds.resize(settings.ledstrip.ledsCount);
    FastLED.addLeds<NEOPIXEL, PINS_LEDSTRIP>(&*std::begin(leds), leds.size())
        .setCorrection(TypicalSMD5050);
}

void updateLedStrip()
{
    EVERY_N_MILLISECONDS( 20 ) { gHue++; }
    static bool have_disabled_beeper = false;
    const bool enAnim = settings.ledstrip.enableAnimBlink;

    if (cpputils::is_in(blinkAnimation, LEDSTRIP_OVERWRITE_BLINKLEFT, LEDSTRIP_OVERWRITE_BLINKRIGHT, LEDSTRIP_OVERWRITE_BLINKBOTH))
    {
        std::fill(std::begin(leds), std::end(leds), CRGB{0, 0, 0});
        if (espchrono::millis_clock::now().time_since_epoch() % 750ms < 375ms || enAnim)
        {
            const auto anim_to_fill = time_to_percent(750ms, 500ms, 100ms, settings.ledstrip.enableFullBlink ? (leds.size() / 2) : settings.ledstrip.bigOffset - settings.ledstrip.smallOffset, settings.ledstrip.enableFullBlink);
            if (settings.ledstrip.enableBeepWhenBlink)
            {
                if (espchrono::millis_clock::now().time_since_epoch() % 750ms < 375ms)
                    for (Controller &controller : controllers)
                        controller.command.buzzer.freq = 3;
                else
                    for (Controller &controller : controllers)
                        controller.command.buzzer.freq = 0;
            }
            auto color = CRGB{255, 200, 0};
            const auto center = (std::begin(leds) + (leds.size() / 2) + settings.ledstrip.centerOffset);

            if (settings.ledstrip.enableFullBlink)
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
                        std::fill(center - settings.ledstrip.bigOffset, center - settings.ledstrip.smallOffset, color);
                    }
                    else
                    {
                        std::fill(center - settings.ledstrip.smallOffset - anim_to_fill, center - settings.ledstrip.smallOffset, color);
                    }
                }
                if (BLINK_RIGHT_EXPR)
                {
                    // Blink right
                    if (!enAnim)
                    {
                        std::fill(center + settings.ledstrip.smallOffset, center + settings.ledstrip.bigOffset, color);
                    }
                    else
                    {
                        std::fill(center + settings.ledstrip.smallOffset, center + settings.ledstrip.smallOffset + anim_to_fill, color);
                    }
                }
            }

// Old way to blink
//            if (blinkAnimation != LEDSTRIP_OVERWRITE_BLINKRIGHT && !settings.ledstrip.enableFullBlink)
//            {
//                std::fill(center - settings.ledstrip.bigOffset, center - settings.ledstrip.smallOffset, color);
//            }
//            else if(blinkAnimation != LEDSTRIP_OVERWRITE_BLINKRIGHT && settings.ledstrip.enableFullBlink)
//            {
//                std::fill(std::begin(leds), center, color);
//            }
//            if (blinkAnimation != LEDSTRIP_OVERWRITE_BLINKLEFT && !settings.ledstrip.enableFullBlink)
//            {
//                std::fill(center + settings.ledstrip.smallOffset, center + settings.ledstrip.bigOffset, color);
//            }
//            else if(blinkAnimation != LEDSTRIP_OVERWRITE_BLINKLEFT && settings.ledstrip.enableFullBlink)
//            {
//                std::fill(center, std::end(leds), color);
//            }

        } else {
            if (settings.ledstrip.enableBeepWhenBlink)
            {
                for (Controller &controller : controllers)
                    controller.command.buzzer.freq = 0;
            }
        }
    }
    else
    {
        if (settings.ledstrip.enableBrakeLights)
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

                const auto center = (std::begin(leds) + (leds.size() / 2) + settings.ledstrip.centerOffset);

                std::fill(std::begin(leds), std::end(leds), CRGB{0, 0, 0});
                if (settings.ledstrip.enableFullBlink)
                {
                    std::fill(std::begin(leds), std::end(leds), color);
                }
                else if(!settings.ledstrip.enableAnimBlink)
                {
                    std::fill(center - settings.ledstrip.bigOffset - 2, center - settings.ledstrip.smallOffset + 2, color);
                    std::fill(center + settings.ledstrip.smallOffset - 2, center + settings.ledstrip.bigOffset + 2, color);
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

    if (have_disabled_beeper == false && (!(cpputils::is_in(blinkAnimation, LEDSTRIP_OVERWRITE_BLINKLEFT, LEDSTRIP_OVERWRITE_BLINKRIGHT, LEDSTRIP_OVERWRITE_BLINKBOTH)) || !settings.ledstrip.enableBeepWhenBlink))
    {
        for (Controller &controller : controllers)
            controller.command.buzzer.freq = 0;
        have_disabled_beeper = true;
    }
    else if ((cpputils::is_in(blinkAnimation, LEDSTRIP_OVERWRITE_BLINKLEFT, LEDSTRIP_OVERWRITE_BLINKRIGHT, LEDSTRIP_OVERWRITE_BLINKBOTH)) && settings.ledstrip.enableBeepWhenBlink) have_disabled_beeper = false;

    if (simplified || settings.ledstrip.enableStVO)
    {
       const auto center = (std::begin(leds) + (leds.size() / 2) + settings.ledstrip.centerOffset);

       if (!(blinkAnimation == LEDSTRIP_OVERWRITE_BLINKLEFT || blinkAnimation == LEDSTRIP_OVERWRITE_BLINKBOTH) || !(espchrono::millis_clock::now().time_since_epoch() % 750ms < 375ms) || settings.ledstrip.enableFullBlink) // Condition for right
       {
           std::fill(center - settings.ledstrip.bigOffset, center - settings.ledstrip.smallOffset, CRGB{0, 0, 0});
           std::fill(center - settings.ledstrip.bigOffset - 1U, center - settings.ledstrip.smallOffset - 1U, CRGB{255, 0, 0}); // Right
       }
       if (!(blinkAnimation == LEDSTRIP_OVERWRITE_BLINKRIGHT || blinkAnimation == LEDSTRIP_OVERWRITE_BLINKBOTH) || !(espchrono::millis_clock::now().time_since_epoch() % 750ms < 375ms) || settings.ledstrip.enableFullBlink) // Condition for left
       {
           std::fill(center + settings.ledstrip.smallOffset, center + settings.ledstrip.bigOffset, CRGB{0, 0, 0});
           std::fill(center + settings.ledstrip.smallOffset + 1U, center + settings.ledstrip.bigOffset + 1U, CRGB{255, 0, 0}); // Left
       }

       if (settings.ledstrip.stvoFrontEnable)
       {
       std::fill(std::begin(leds) + settings.ledstrip.stvoFrontOffset, std::begin(leds) + settings.ledstrip.stvoFrontOffset + settings.ledstrip.stvoFrontLength, CRGB{255, 255, 255});
       std::fill(std::end(leds) - settings.ledstrip.stvoFrontOffset - settings.ledstrip.stvoFrontLength, std::end(leds) - settings.ledstrip.stvoFrontOffset, CRGB{255, 255, 255});
       }
    }

    FastLED.setMaxPowerInVoltsAndMilliamps(5,settings.ledstrip.deziampere * 100);
    FastLED.setBrightness(settings.ledstrip.brightness);
    FastLED.show();
}

void showAnimation()
{
    if (settings.ledstrip.enableLedAnimation
        && !simplified
#ifdef FEATURE_OTA
        && !(asyncOtaTaskStarted && settings.ledstrip.otaMode != OtaAnimationModes::None)
#endif
        )
    {
        if (animation_type == LedstripAnimation::DefaultRainbow) showDefaultLedstrip();
        else if (animation_type == LedstripAnimation::BetterRainbow) showBetterRainbow();
        else if (animation_type == LedstripAnimation::SpeedSync) showSpeedSyncAnimation();
        else if (animation_type == LedstripAnimation::CustomColor) showCustomColor();
        else showDefaultLedstrip();
    }
#ifdef FEATURE_OTA
    else if (asyncOtaTaskStarted && settings.ledstrip.otaMode != OtaAnimationModes::None)
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
        if (settings.ledstrip.otaMode == OtaAnimationModes::GreenProgressBar)
        {
            int numLeds = (leds_count * percentage) / 100;
            if (numLeds >= leds_count)
            {
                numLeds = leds_count - 1;
            }
            std::fill(std::begin(leds), std::begin(leds) + numLeds, CRGB{0,255,0});
        }
        else if (settings.ledstrip.otaMode == OtaAnimationModes::ColorChangeAll)
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
    auto difference_ms = espchrono::ago(last_interval).count();

    static float hue_result = 0;

    const float hue_per_led = 1. / std::max(uint8_t(1), uint8_t(settings.ledstrip.animationMultiplier));
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
    const auto center = (std::begin(leds) + (leds.size() / 2) + settings.ledstrip.centerOffset);

    std::fill(std::begin(leds), std::end(leds), ledstrip_custom_colors[int(Bobbycar_Side::FRONT)]); // Front
    std::fill(center - (eighth_length / 2), center + (eighth_length / 2), ledstrip_custom_colors[int(Bobbycar_Side::BACK)]); // Back

#ifdef LEDSTRIP_WRONG_DIRECTION
    std::fill(center + (eighth_length / 2), center + (eighth_length / 2) + eighth_length, ledstrip_custom_colors[int(Bobbycar_Side::BACK_LEFT)]);  // Back Left
    std::fill(center - (eighth_length / 2) - eighth_length, center - (eighth_length / 2), ledstrip_custom_colors[int(Bobbycar_Side::BACK_RIGHT)]); // Back Right
#else
    std::fill(center + (eighth_length / 2), center + (eighth_length / 2) + eighth_length, ledstrip_custom_colors[int(Bobbycar_Side::BACK_RIGHT)]);  // Back Right
    std::fill(center - (eighth_length / 2) - eighth_length, center - (eighth_length / 2), ledstrip_custom_colors[int(Bobbycar_Side::BACK_LEFT)]);   // Back Left
#endif

#ifdef LEDSTRIP_WRONG_DIRECTION
    std::fill(center + (eighth_length / 2) + eighth_length, center + (eighth_length / 2) + eighth_length + eighth_length, ledstrip_custom_colors[int(Bobbycar_Side::LEFT)]);  // Left
    std::fill(center - (eighth_length / 2) - eighth_length - eighth_length, center - (eighth_length / 2) - eighth_length, ledstrip_custom_colors[int(Bobbycar_Side::RIGHT)]); // Right
#else
    std::fill(center + (eighth_length / 2) + eighth_length, center + (eighth_length / 2) + eighth_length + eighth_length, ledstrip_custom_colors[int(Bobbycar_Side::RIGHT)]);  // Right
    std::fill(center - (eighth_length / 2) - eighth_length - eighth_length, center - (eighth_length / 2) - eighth_length, ledstrip_custom_colors[int(Bobbycar_Side::LEFT)]);   // Left
#endif

#ifdef LEDSTRIP_WRONG_DIRECTION
    std::fill(center + (eighth_length / 2) + eighth_length + eighth_length, center + (eighth_length / 2) + eighth_length + eighth_length + eighth_length, ledstrip_custom_colors[int(Bobbycar_Side::FRONT_LEFT)]);  // Front Left
    std::fill(center - (eighth_length / 2) - eighth_length - eighth_length - eighth_length, center - (eighth_length / 2) - eighth_length - eighth_length, ledstrip_custom_colors[int(Bobbycar_Side::FRONT_RIGHT)]); // Front Right
#else
    std::fill(center + (eighth_length / 2) + eighth_length + eighth_length, center + (eighth_length / 2) + eighth_length + eighth_length + eighth_length, ledstrip_custom_colors[int(Bobbycar_Side::FRONT_RIGHT)]);  // Front Right
    std::fill(center - (eighth_length / 2) - eighth_length - eighth_length - eighth_length, center - (eighth_length / 2) - eighth_length - eighth_length, ledstrip_custom_colors[int(Bobbycar_Side::FRONT_LEFT)]);   // Front Left
#endif
}

#endif
