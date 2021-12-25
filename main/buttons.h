#pragma once

// system includes
#include <optional>

// 3rdparty lib includes
#include <espchrono.h>

// local includes
#include "settingsutils.h"
#include "ledstripdefines.h"

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

extern bool disableAllButtons;

extern std::optional<espchrono::millis_clock::time_point> upPressedSince;
extern int upPressRepeat;
extern std::optional<espchrono::millis_clock::time_point> downPressedSince;
extern int downPressRepeat;

enum Buttons : uint8_t {
    Confirm,
    Back,
    Up,
    Down,
    BlinkLeft,
    BlinkRight,
    QuickActionUp,
    QuickActionDown
};

class InputDispatcher
{
public:
    static void init();

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

    static std::array<std::function<void(bool)>, 8> ButtonHandlers;
private:
    static void handleUp(bool pressed);
    static void handleDown(bool pressed);
    static void handleConfirm(bool pressed);
    static void handleBack(bool pressed);
    static void handleBlinkLeft(bool pressed);
    static void handleBlinkRight(bool pressed);
    static void handleQuickActionUp(bool pressed);
    static void handleQuickActionDown(bool pressed);
};
