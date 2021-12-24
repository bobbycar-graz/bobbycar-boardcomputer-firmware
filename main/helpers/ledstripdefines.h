#pragma once
#include <cstdint>
/*
 * This file contains a few defines, so you don't have to remember which ledstrip animation is which number
 */

#define LEDSTRIP_OVERWRITE_NONE 0
#ifndef LEDSTRIP_WRONG_DIRECTION
#define LEDSTRIP_OVERWRITE_BLINKLEFT  1
#define LEDSTRIP_OVERWRITE_BLINKRIGHT 2
#else
#define LEDSTRIP_OVERWRITE_BLINKLEFT  2
#define LEDSTRIP_OVERWRITE_BLINKRIGHT 1
#endif
#define LEDSTRIP_OVERWRITE_BLINKBOTH  3

enum LedstripAnimation : uint16_t {
    DefaultRainbow,
    BetterRainbow,
    SpeedSync,
    CustomColor
};

#define BLINK_LEFT_EXPR blinkAnimation != LEDSTRIP_OVERWRITE_BLINKRIGHT
#define BLINK_RIGHT_EXPR blinkAnimation != LEDSTRIP_OVERWRITE_BLINKLEFT
