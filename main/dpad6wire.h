#pragma once
// system includes
#include <array>

// Arduino includes
#include <Arduino.h>

// 3rdparty lib includes
#include <espchrono.h>
#include <tickchrono.h>

// local includes
#include "buttons.h"
#include "types.h"

namespace
{
namespace dpad6wire
{
#ifdef FEATURE_DPAD_6WIRESW
class State : public std::array<bool, 10>
{
  public:
    State() :
        std::array<bool, 10> { false, false, false, false, false, false, false, false, false, false } {}
    State(const std::array<bool, 10> &other) :
        std::array<bool, 10> {} {}

    State &operator=(const std::array<bool, 10> &other)
    {
        std::array<bool, 10>::operator=(other);
        return *this;
    }

    State &operator=(const State &other)
    {
        std::array<bool, 10>::operator=(other);
        return *this;
    }

    bool &up { this->at(DPAD_6WIRESW_UP) };
    bool &down { this->at(DPAD_6WIRESW_DOWN) };
    bool &confirm { this->at(DPAD_6WIRESW_CONFIRM) };
    bool &back { this->at(DPAD_6WIRESW_BACK) };
    bool &profile0 { this->at(DPAD_6WIRESW_PROFILE0) };
    bool &profile1 { this->at(DPAD_6WIRESW_PROFILE1) };
    bool &profile2 { this->at(DPAD_6WIRESW_PROFILE2) };
    bool &profile3 { this->at(DPAD_6WIRESW_PROFILE3) };

    bool &blink_left { this->at(DPAD_6WIRESW_BLINK_LEFT) };
    bool &blink_right { this->at(DPAD_6WIRESW_BLINK_RIGHT) };
};

template<pin_t OUT, pin_t IN1, pin_t IN2, pin_t IN3, pin_t IN4, pin_t IN5>
class Helper
{
  public:
    static constexpr auto OutPin = OUT;
    static constexpr auto In1Pin = IN1;
    static constexpr auto In2Pin = IN2;
    static constexpr auto In3Pin = IN3;
    static constexpr auto In4Pin = IN4;
    static constexpr auto In5Pin = IN5;

    void begin();

    State read();
};

template<pin_t OUT, pin_t IN1, pin_t IN2, pin_t IN3, pin_t IN4, pin_t IN5>
void Helper<OUT, IN1, IN2, IN3, IN4, IN5>::begin()
{
    pinMode(OUT, OUTPUT);
}

template<pin_t OUT, pin_t IN1, pin_t IN2, pin_t IN3, pin_t IN4, pin_t IN5>
State Helper<OUT, IN1, IN2, IN3, IN4, IN5>::read()
{
    State result;

    digitalWrite(OUT, LOW);

    pinMode(IN1, INPUT_PULLUP);
    pinMode(IN2, INPUT_PULLUP);
    pinMode(IN3, INPUT_PULLUP);
    pinMode(IN4, INPUT_PULLUP);
    pinMode(IN5, INPUT_PULLUP);

    vPortYield();

    result[0] = digitalRead(IN1) == LOW;
    result[1] = digitalRead(IN2) == LOW;
    result[2] = digitalRead(IN3) == LOW;
    result[3] = digitalRead(IN4) == LOW;
    result[8] = digitalRead(IN5) == LOW;

    digitalWrite(OUT, HIGH);

    pinMode(IN1, INPUT_PULLDOWN);
    pinMode(IN2, INPUT_PULLDOWN);
    pinMode(IN3, INPUT_PULLDOWN);
    pinMode(IN4, INPUT_PULLDOWN);
    pinMode(IN5, INPUT_PULLDOWN);

    vPortYield();

    result[4] = digitalRead(IN1);
    result[5] = digitalRead(IN2);
    result[6] = digitalRead(IN3);
    result[7] = digitalRead(IN4);
    result[9] = digitalRead(IN5);

#ifdef FEATURE_GSCHISSENE_DIODE
    if (result[8] && result[9]) result[9] = 0;
#endif

    return result;
}

Helper<PINS_DPAD_6WIRESW_OUT, PINS_DPAD_6WIRESW_IN1, PINS_DPAD_6WIRESW_IN2, PINS_DPAD_6WIRESW_IN3, PINS_DPAD_6WIRESW_IN4, PINS_DPAD_6WIRESW_IN5> helper;
State lastState;
espchrono::millis_clock::time_point debounceUp, debounceDown, debounceConfirm, debounceBack, debounceProfile0, debounceProfile1, debounceProfile2, debounceProfile3, debounceBlinkLeft, debounceBlinkRight;

void init()
{
    helper.begin();
    debounceUp = debounceDown = debounceConfirm = debounceBack = debounceProfile0 = debounceProfile1 = debounceProfile2 = debounceProfile3 = debounceBlinkLeft = debounceBlinkRight = espchrono::millis_clock::now();
}

void update()
{
    const auto newState = helper.read();

#ifdef DPAD_6WIRESW_DEBUG
    lastState = newState;
    return;
#endif

    const auto now = espchrono::millis_clock::now();

    const std::chrono::milliseconds dpadDebounce { settings.boardcomputerHardware.dpadDebounce };

    if (lastState.up != newState.up && now - debounceUp > dpadDebounce)
    {
        lastState.up = newState.up;
        InputDispatcher::upButton(newState.up);
        debounceUp = now;
    }

    if (lastState.down != newState.down && now - debounceDown > dpadDebounce)
    {
        lastState.down = newState.down;
        InputDispatcher::downButton(newState.down);
        debounceDown = now;
    }

    if (lastState.confirm != newState.confirm && now - debounceConfirm > dpadDebounce)
    {
        lastState.confirm = newState.confirm;
        InputDispatcher::confirmButton(newState.confirm);
        debounceConfirm = now;
    }

    if (lastState.back != newState.back && now - debounceBack > dpadDebounce)
    {
        lastState.back = newState.back;
        InputDispatcher::backButton(newState.back);
        debounceBack = now;
    }

    if (lastState.profile0 != newState.profile0 && now - debounceProfile0 > dpadDebounce)
    {
        lastState.profile0 = newState.profile0;
        InputDispatcher::profileButton(0, newState.profile0);
        debounceProfile0 = now;
    }

    if (lastState.profile1 != newState.profile1 && now - debounceProfile1 > dpadDebounce)
    {
        lastState.profile1 = newState.profile1;
        InputDispatcher::profileButton(1, newState.profile1);
        debounceProfile1 = now;
    }

    if (lastState.profile2 != newState.profile2 && now - debounceProfile2 > dpadDebounce)
    {
        lastState.profile2 = newState.profile2;
        InputDispatcher::profileButton(2, newState.profile2);
        debounceProfile2 = now;
    }

    if (lastState.profile3 != newState.profile3 && now - debounceProfile3 > dpadDebounce)
    {
        lastState.profile3 = newState.profile3;
        InputDispatcher::profileButton(3, newState.profile3);
        debounceProfile3 = now;
    }

    if (lastState.blink_left != newState.blink_left && now - debounceBlinkLeft > dpadDebounce)
    {
        lastState.blink_left = newState.blink_left;
        InputDispatcher::blinkLeftButton(newState.blink_left);
        debounceBlinkLeft = now;
    }

    if (lastState.blink_right != newState.blink_right && now - debounceBlinkRight > dpadDebounce)
    {
        lastState.blink_right = newState.blink_right;
        InputDispatcher::blinkRightButton(newState.blink_right);
        debounceBlinkRight = now;
    }
}
#endif

} // namespace dpad6wire
} // namespace
