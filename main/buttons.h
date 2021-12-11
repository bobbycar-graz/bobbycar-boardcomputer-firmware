#pragma once

// system includes
#include <optional>

// 3rdparty lib includes
#include <espchrono.h>

// local includes
#include "ledstripdefines.h"
#include "settingsutils.h"

#ifdef FEATURE_LEDSTRIP
#include "ledstrip.h"
#endif

extern int rotated;
extern bool requestFullRedraw;

extern bool confirmButtonPressed;
extern bool confirmButtonLongPressed;
extern bool backButtonPressed;
extern bool backButtonLongPressed;

extern bool profileButtonDisabled;

extern std::optional<espchrono::millis_clock::time_point> upPressedSince;
extern int upPressRepeat;
extern std::optional<espchrono::millis_clock::time_point> downPressedSince;
extern int downPressRepeat;

class InputDispatcher
{
public:
  static void update();

  static void rotate(int offset);

  static void upButton(bool pressed);

  static void downButton(bool pressed);

  static void confirmButton(bool pressed);

  static void backButton(bool pressed);

  static void profileButton(uint8_t index, bool pressed);

  static void blinkLeftButton(bool pressed);

  static void blinkRightButton(bool pressed);

  static void quickActionButtonDown(bool pressed);

  static void quickActionButtonUp(bool pressed);
};
