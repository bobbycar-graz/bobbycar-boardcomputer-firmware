#pragma once

// system includes
#include <vector>

// 3rdparty lib includes
#include <cpptypesafeenum.h>
#include <FastLED.h>

// local includes
#include "ledstripdefines.h"

#define OtaAnimationModesValues(x) \
    x(None) \
    x(GreenProgressBar) \
    x(ColorChangeAll)
DECLARE_TYPESAFE_ENUM(OtaAnimationModes, : uint8_t, OtaAnimationModesValues)

#define LedstripAnimationValues(x) \
    x(DefaultRainbow) \
    x(BetterRainbow) \
    x(SpeedSync) \
    x(CustomColor)
DECLARE_TYPESAFE_ENUM(LedstripAnimation, : uint8_t, LedstripAnimationValues)

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

extern std::vector<CRGB> leds;
extern uint8_t gHue;

extern uint16_t blinkAnimation;

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
