#include "dpad5wire_2out.h"

// system includes
#include <array>

// Arduino includes
#include <Arduino.h>

// 3rdparty lib includes
#include <espchrono.h>
#include <tickchrono.h>
#include <screenmanager.h>

// local includes
#include "types.h"
#include "globals.h"
#include "newsettings.h"
#include "bobbybuttons.h"

namespace {
template<pin_t OUT, pin_t IN1, pin_t IN2, pin_t IN3, pin_t IN4>
class Helper
{
public:
    static constexpr auto OutPin = OUT;
    static constexpr auto In1Pin = IN1;
    static constexpr auto In2Pin = IN2;
    static constexpr auto In3Pin = IN3;
    static constexpr auto In4Pin = IN4;

    void begin();

    std::array<bool, 12> read();
};

template<pin_t OUT1, pin_t OUT2, pin_t IN1, pin_t IN2, pin_t IN3>
void Helper<OUT1, OUT2, IN1, IN2, IN3>::begin()
{
}

template<pin_t OUT1, pin_t OUT2, pin_t IN1, pin_t IN2, pin_t IN3>
std::array<bool, 12> Helper<OUT1, OUT2, IN1, IN2, IN3>::read()
{
    std::array<bool, 12> result;

    pinMode(OUT1, OUTPUT);
    pinMode(OUT2, INPUT);

    digitalWrite(OUT1, LOW);

    pinMode(IN1, INPUT_PULLUP);
    pinMode(IN2, INPUT_PULLUP);
    pinMode(IN3, INPUT_PULLUP);

    if (configs.buttonReadDelay.value != 0) {
        delayMicroseconds(configs.buttonReadDelay.value);
    } else {
        vPortYield();
    }

    result[0] = digitalRead(IN1)==LOW;
    result[1] = digitalRead(IN2)==LOW;
    result[2] = digitalRead(IN3)==LOW;

    digitalWrite(OUT1, HIGH);

    pinMode(IN1, INPUT_PULLDOWN);
    pinMode(IN2, INPUT_PULLDOWN);
    pinMode(IN3, INPUT_PULLDOWN);

    if (configs.buttonReadDelay.value != 0) {
        delayMicroseconds(configs.buttonReadDelay.value);
    } else {
        vPortYield();
    }

    result[3] = digitalRead(IN1);
    result[4] = digitalRead(IN2);
    result[5] = digitalRead(IN3);

    pinMode(OUT1, INPUT);
    pinMode(OUT2, OUTPUT);

    digitalWrite(OUT2, LOW);

    pinMode(IN1, INPUT_PULLUP);
    pinMode(IN2, INPUT_PULLUP);
    pinMode(IN3, INPUT_PULLUP);

    if (configs.buttonReadDelay.value != 0) {
        delayMicroseconds(configs.buttonReadDelay.value);
    } else {
        vPortYield();
    }

    result[6] = digitalRead(IN1)==LOW;
    result[7] = digitalRead(IN2)==LOW;
    result[8] = digitalRead(IN3)==LOW;

    digitalWrite(OUT2, HIGH);

    pinMode(IN1, INPUT_PULLDOWN);
    pinMode(IN2, INPUT_PULLDOWN);
    pinMode(IN3, INPUT_PULLDOWN);

    if (configs.buttonReadDelay.value != 0) {
        delayMicroseconds(configs.buttonReadDelay.value);
    } else {
        vPortYield();
    }

    result[9] = digitalRead(IN1);
    result[10] = digitalRead(IN2);
    result[11] = digitalRead(IN3);

    return result;
}

#ifdef FEATURE_DPAD_5WIRESW_2OUT
Helper<PINS_DPAD_5WIRESW_OUT1, PINS_DPAD_5WIRESW_OUT2, PINS_DPAD_5WIRESW_IN1, PINS_DPAD_5WIRESW_IN2, PINS_DPAD_5WIRESW_IN3> helper;
std::array<bool, 12> lastState;
std::array<espchrono::millis_clock::time_point, 12> debounce;
#endif
} // namespace

#ifdef FEATURE_DPAD_5WIRESW_2OUT

namespace dpad5wire_2out {
void init()
{
    helper.begin();
    std::fill(std::begin(lastState), std::end(lastState), false);
    std::fill(std::begin(debounce), std::end(debounce), espchrono::millis_clock::now());
}

void update()
{
    const auto newState = helper.read();

    const auto now = espchrono::millis_clock::now();

    const std::chrono::milliseconds dpadDebounce{configs.dpadDebounce.value};

    for (auto i = 0; i < 12; i++)
        if (lastState[i] != newState[i] && now - debounce[i] > dpadDebounce)
        {
            lastState[i] = newState[i];
            if (espgui::currentDisplay)
            {
                if (newState[i])
                    espgui::currentDisplay->rawButtonPressed(i);
                else
                    espgui::currentDisplay->rawButtonReleased(i);
            }
            debounce[i] = now;
        }
}

} // namespace dpad5wire_2out

#endif
