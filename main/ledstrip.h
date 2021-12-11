#pragma once

// system includes
#include <vector>

// 3rdparty lib includes
#include <FastLED.h>

#ifdef FEATURE_LEDSTRIP
enum Bobbycar_Side
{
    FRONT_RIGHT,
    RIGHT,
    BACK_RIGHT,
    BACK,
    BACK_LEFT,
    LEFT,
    FRONT_LEFT,
    FRONT
};

#ifdef FEATURE_OTA
enum OtaAnimationModes
{
    None,
    GreenProgressBar,
    ColorChangeAll
};
#endif

extern std::vector<CRGB> leds;
extern uint8_t gHue;

extern uint16_t blinkAnimation;
extern uint16_t animation_type;

void showDefaultLedstrip();
void showAnimation();
void showBetterRainbow();
void showSpeedSyncAnimation();
void showCustomColor();
#ifdef FEATURE_OTA
void showOtaAnimation();
#endif

void initLedStrip();
void updateLedStrip();
#endif
