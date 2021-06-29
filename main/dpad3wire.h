#pragma once

#include <Arduino.h>

#include <espchrono.h>

#include "dpad.h"
#include "buttons.h"
#include "types.h"

namespace {
namespace dpad3wire
{
template<pin_t OUT, pin_t IN1, pin_t IN2>
class Helper
{
public:
    static constexpr auto OutPin = OUT;
    static constexpr auto In1Pin = IN1;
    static constexpr auto In2Pin = IN2;

    void begin();

    dpad::State read();
};

template<pin_t OUT, pin_t IN1, pin_t IN2>
void Helper<OUT, IN1, IN2>::begin()
{
    pinMode(OUT, OUTPUT);
}

template<pin_t OUT, pin_t IN1, pin_t IN2>
dpad::State Helper<OUT, IN1, IN2>::read()
{
    digitalWrite(OUT, LOW);

    pinMode(IN1, INPUT_PULLUP);
    pinMode(IN2, INPUT_PULLUP);

    delay(1);

    const bool result0 = digitalRead(IN1)==LOW;
    const bool result1 = digitalRead(IN2)==LOW;

    digitalWrite(OUT, HIGH);

    pinMode(IN1, INPUT_PULLDOWN);
    pinMode(IN2, INPUT_PULLDOWN);

    delay(1);

    const bool result2 = digitalRead(IN1);
    const bool result3 = digitalRead(IN2);

    return std::make_tuple(result0, result1, result2, result3);
}

#ifdef FEATURE_DPAD_3WIRESW
Helper<PINS_DPAD_3WIRESW_OUT, PINS_DPAD_3WIRESW_IN1, PINS_DPAD_3WIRESW_IN2> helper;
dpad::State lastState;
espchrono::millis_clock::time_point debounceUp, debounceDown, debounceConfirm, debounceBack;

void init()
{
    helper.begin();
    debounceUp = debounceDown = debounceConfirm = debounceBack = espchrono::millis_clock::now();
}

void update()
{
    const auto state = helper.read();
    const auto now = espchrono::millis_clock::now();

    enum {
        ButtonUp = 3,
        ButtonDown = 0,
        ButtonConfirm = 1,
        ButtonBack = 2
    };

    if (std::get<ButtonUp>(lastState) != std::get<ButtonUp>(state) && now-debounceUp > settings.boardcomputerHardware.dpadDebounce)
    {
        if (std::get<ButtonUp>(state))
            InputDispatcher::rotate(-1);
        std::get<ButtonUp>(lastState) = std::get<ButtonUp>(state);
        debounceUp = now;
    }
    if (std::get<ButtonDown>(lastState) != std::get<ButtonDown>(state) && now-debounceDown > settings.boardcomputerHardware.dpadDebounce)
    {
        if (std::get<ButtonDown>(state))
            InputDispatcher::rotate(1);
        std::get<ButtonDown>(lastState) = std::get<ButtonDown>(state);
        debounceDown = now;
    }
    if (std::get<ButtonConfirm>(lastState) != std::get<ButtonConfirm>(state) && now-debounceConfirm > settings.boardcomputerHardware.dpadDebounce)
    {
        InputDispatcher::confirmButton(std::get<ButtonConfirm>(state));
        std::get<ButtonConfirm>(lastState) = std::get<ButtonConfirm>(state);
        debounceConfirm = now;
    }
    if (std::get<ButtonBack>(lastState) != std::get<ButtonBack>(state) && now-debounceBack > settings.boardcomputerHardware.dpadDebounce)
    {
        InputDispatcher::backButton(std::get<ButtonBack>(state));
        std::get<ButtonBack>(lastState) = std::get<ButtonBack>(state);
        debounceBack = now;
    }
}
#endif
}
}
