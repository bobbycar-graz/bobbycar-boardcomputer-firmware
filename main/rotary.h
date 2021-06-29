#pragma once

// 3rdparty lib includes
#include <espchrono.h>

// local includes
#include "globals.h"
#include "types.h"

namespace {
template<typename HANDLER, pin_t CLK, pin_t DT, pin_t SW>
class Rotary
{
public:
    static constexpr auto ClkPin = CLK;
    static constexpr auto DtPin = DT;
    static constexpr auto SwPin = SW;

    void begin();

    void updateRotate();
    void updateSwitch();

private:
    espchrono::millis_clock::time_point m_lastMillis;

    bool m_lastClk;
    bool m_lastSw{};
};

template<typename HANDLER, pin_t CLK, pin_t DT, pin_t SW>
void Rotary<HANDLER, CLK, DT, SW>::begin()
{
    m_lastMillis = espchrono::millis_clock::now();

    pinMode(ClkPin, INPUT_PULLUP);
    pinMode(DtPin, INPUT_PULLUP);
    pinMode(SwPin, INPUT_PULLUP);

    m_lastClk = digitalRead(ClkPin);
}

template<typename HANDLER, pin_t CLK, pin_t DT, pin_t SW>
void Rotary<HANDLER, CLK, DT, SW>::updateRotate()
{
    const auto now = espchrono::millis_clock::now();
    if (now - m_lastMillis < 1ms)
        return;
    m_lastMillis = now;

    const bool currentClk = digitalRead(ClkPin);
    if (currentClk != m_lastClk)
    {
        HANDLER::rotate(digitalRead(DtPin) != currentClk ? 1 : -1);
        m_lastClk = currentClk;
    }
}

template<typename HANDLER, pin_t CLK, pin_t DT, pin_t SW>
void Rotary<HANDLER, CLK, DT, SW>::updateSwitch()
{
    const auto now = espchrono::millis_clock::now();
    if (now - m_lastMillis < 1ms)
        return;
    m_lastMillis = now;

    const bool currentSw = !bool(digitalRead(SwPin));
    if (currentSw != m_lastSw)
    {
        HANDLER::button(currentSw);
        m_lastSw = currentSw;
    }
}

#ifdef FEATURE_ROTARY
Rotary<InputDispatcher, PINS_ROTARY_CLK, PINS_ROTARY_DT, PINS_ROTARY_SW> rotary;

void updateRotate() { rotary.updateRotate(); }
void updateSwitch() { rotary.updateSwitch(); }

void initRotary()
{
    rotary.begin();
    attachInterrupt(decltype(rotary)::ClkPin, updateRotate, CHANGE);
    attachInterrupt(decltype(rotary)::SwPin, updateSwitch, CHANGE);
}
#endif
}
