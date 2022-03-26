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
#ifdef FEATURE_DPAD_5WIRESW_2OUT
std::array<bool, 12> lastState;
std::array<espchrono::millis_clock::time_point, 12> debounce;
#endif
} // namespace

#ifdef FEATURE_DPAD_5WIRESW_2OUT

namespace dpad5wire_2out {
void init()
{
    std::fill(std::begin(lastState), std::end(lastState), false);
    std::fill(std::begin(debounce), std::end(debounce), espchrono::millis_clock::now());
}

void update()
{
    constexpr pin_t OUT1 = PINS_DPAD_5WIRESW_OUT1;
    constexpr pin_t OUT2 = PINS_DPAD_5WIRESW_OUT2;
    constexpr pin_t IN1 = PINS_DPAD_5WIRESW_IN1;
    constexpr pin_t IN2 = PINS_DPAD_5WIRESW_IN2;
    constexpr pin_t IN3 = PINS_DPAD_5WIRESW_IN3;

    static std::array<bool, 12> newState;
    static uint8_t step{};

    switch (step++)
    {
    case 0:
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

        newState[0] = digitalRead(IN1)==LOW;
        newState[1] = digitalRead(IN2)==LOW;
        newState[2] = digitalRead(IN3)==LOW;
        break;

    case 1:
        digitalWrite(OUT1, HIGH);

        pinMode(IN1, INPUT_PULLDOWN);
        pinMode(IN2, INPUT_PULLDOWN);
        pinMode(IN3, INPUT_PULLDOWN);

        if (configs.buttonReadDelay.value != 0) {
            delayMicroseconds(configs.buttonReadDelay.value);
        } else {
            vPortYield();
        }

        newState[3] = digitalRead(IN1);
        newState[4] = digitalRead(IN2);
        newState[5] = digitalRead(IN3);
        break;

    case 2:
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

        newState[6] = digitalRead(IN1)==LOW;
        newState[7] = digitalRead(IN2)==LOW;
        newState[8] = digitalRead(IN3)==LOW;
        break;

    case 3:
        digitalWrite(OUT2, HIGH);

        pinMode(IN1, INPUT_PULLDOWN);
        pinMode(IN2, INPUT_PULLDOWN);
        pinMode(IN3, INPUT_PULLDOWN);

        if (configs.buttonReadDelay.value != 0) {
            delayMicroseconds(configs.buttonReadDelay.value);
        } else {
            vPortYield();
        }

        newState[9] = digitalRead(IN1);
        newState[10] = digitalRead(IN2);
        newState[11] = digitalRead(IN3);

        {
            const auto now = espchrono::millis_clock::now();

            const std::chrono::milliseconds &dpadDebounce{configs.dpadDebounce.value};

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

    default:
        step = 0;
    }
}

} // namespace dpad5wire_2out

#endif
