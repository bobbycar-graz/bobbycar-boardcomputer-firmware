#include "potis.h"

// 3rdparty lib includes
#include <Arduino.h>
#include <cpputils.h>

// local includes
#include "globals.h"
#ifdef FEATURE_CAN
#include "can.h"
#endif
#include "newsettings.h"
#include "utils.h"

using namespace std::chrono_literals;

void initPotis()
{
    raw_gas = std::nullopt;
    raw_brems = std::nullopt;
    gas = std::nullopt;
    brems = std::nullopt;

    readPotis();
}

void readPotis()
{
    [[maybe_unused]]
    constexpr auto sampleMultipleTimes = [](uint8_t pin){
          analogRead(pin);
          double sum{};
          const auto sampleCount = configs.sampleCount.value;
          for (int i = 0; i < sampleCount; i++)
              sum += analogRead(pin);
          return sum / sampleCount;
      };

    raw_gas = std::nullopt;
    raw_brems = std::nullopt;

#ifdef FEATURE_CAN
    const auto now = espchrono::millis_clock::now();

    if (can::can_gas)
    {
        if (now - can::last_can_gas < 100ms)
            raw_gas = *can::can_gas;
        else
            can::can_gas = std::nullopt;
    }

    if (can::can_brems)
    {
        if (now - can::last_can_brems < 100ms)
            raw_brems = *can::can_brems;
        else
            can::can_brems = std::nullopt;
    }
#endif

#ifdef FEATURE_ADC_IN
    if (!raw_gas)
        raw_gas = sampleMultipleTimes(PINS_GAS);
    if (!raw_brems)
        raw_brems = sampleMultipleTimes(PINS_BREMS);
#endif

#ifndef FEATURE_JOYSTICK
    if (raw_gas)
        gas = cpputils::mapValueClamped<float>(*raw_gas, configs.gasMin.value, configs.gasMax.value, 0., 1000.);
    else
        gas = std::nullopt;
    if (raw_brems)
        brems = cpputils::mapValueClamped<float>(*raw_brems, configs.bremsMin.value, configs.bremsMax.value, 0., 1000.);
    else
        brems = std::nullopt;
#else
    if (raw_gas)
    {
        gas = map_analog_stick(configs.gasMitte.value, configs.gasMin.value, configs.gasMax.value, *raw_gas);
    }
    else
        gas = std::nullopt;
    if (raw_brems)
    {
        brems = map_analog_stick(configs.bremsMitte.value, configs.bremsMin.value, configs.bremsMax.value, *raw_brems);
    }
    else
        brems = std::nullopt;
#endif

#ifdef FEATURE_GAMETRAK
    raw_gametrakX = sampleMultipleTimes(PINS_GAMETRAKX);
    gametrakX = cpputils::mapValueClamped<float>(raw_gametrakX, configs.boardcomputerHardware.gametrakXMin.value, configs.boardcomputerHardware.gametrakXMax.value, 0., 1000.);

    raw_gametrakY = sampleMultipleTimes(PINS_GAMETRAKY);
    gametrakY = cpputils::mapValueClamped<float>(raw_gametrakY, configs.boardcomputerHardware.gametrakYMin.value, configs.boardcomputerHardware.gametrakYMax.value, 0., 1000.);

    raw_gametrakDist = sampleMultipleTimes(PINS_GAMETRAKDIST);
    gametrakDist = cpputils::mapValueClamped<float>(raw_gametrakDist, configs.boardcomputerHardware.gametrakDistMin.value, configs.boardcomputerHardware.gametrakDistMax.value, 0., 1000.);
#endif
}
