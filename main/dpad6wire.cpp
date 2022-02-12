#include "dpad6wire.h"

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

    std::array<bool, 10> read();
};

template<pin_t OUT, pin_t IN1, pin_t IN2, pin_t IN3, pin_t IN4, pin_t IN5>
void Helper<OUT, IN1, IN2, IN3, IN4, IN5>::begin()
{
    pinMode(OUT, OUTPUT);
}

template<pin_t OUT, pin_t IN1, pin_t IN2, pin_t IN3, pin_t IN4, pin_t IN5>
std::array<bool, 10> Helper<OUT, IN1, IN2, IN3, IN4, IN5>::read()
{
    std::array<bool, 10> result;

    digitalWrite(OUT, LOW);

    pinMode(IN1, INPUT_PULLUP);
    pinMode(IN2, INPUT_PULLUP);
    pinMode(IN3, INPUT_PULLUP);
    pinMode(IN4, INPUT_PULLUP);
    pinMode(IN5, INPUT_PULLUP);

    if (configs.buttonReadDelay.value != 0) {
        delayMicroseconds(configs.buttonReadDelay.value);
    } else {
        vPortYield();
    }

    result[0] = digitalRead(IN1)==LOW;
    result[1] = digitalRead(IN2)==LOW;
    result[2] = digitalRead(IN3)==LOW;
    result[3] = digitalRead(IN4)==LOW;
    result[8] = digitalRead(IN5)==LOW;

    digitalWrite(OUT, HIGH);

    pinMode(IN1, INPUT_PULLDOWN);
    pinMode(IN2, INPUT_PULLDOWN);
    pinMode(IN3, INPUT_PULLDOWN);
    pinMode(IN4, INPUT_PULLDOWN);
    pinMode(IN5, INPUT_PULLDOWN);

    if (configs.buttonReadDelay.value != 0) {
        delayMicroseconds(configs.buttonReadDelay.value);
    } else {
        vPortYield();
    }

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

#ifdef FEATURE_DPAD_6WIRESW
Helper<PINS_DPAD_6WIRESW_OUT, PINS_DPAD_6WIRESW_IN1, PINS_DPAD_6WIRESW_IN2, PINS_DPAD_6WIRESW_IN3, PINS_DPAD_6WIRESW_IN4, PINS_DPAD_6WIRESW_IN5> helper;
std::array<bool, 10> lastState;
std::array<espchrono::millis_clock::time_point, 10> debounce;
#endif
} // namespace

#ifdef FEATURE_DPAD_6WIRESW
namespace dpad6wire {

void init()
{
    helper.begin();
    std::fill(std::begin(lastState), std::end(lastState), false);
    std::fill(std::begin(debounce), std::end(debounce), espchrono::millis_clock::now());
}

void update()
{
    const auto newState = helper.read();

#ifdef DPAD_6WIRESW_DEBUG
    lastState = newState;
    return;
#endif

    const auto now = espchrono::millis_clock::now();

    const std::chrono::milliseconds dpadDebounce{configs.dpadDebounce.value};

    for (auto i = 0; i < 10; i++)
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

} // namespace dpad6wire

#endif
