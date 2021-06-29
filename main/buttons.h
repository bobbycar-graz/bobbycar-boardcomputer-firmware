#pragma once

// Arduino includes
#include <Arduino.h>

#include <espchrono.h>

// local includes
#include "settingsutils.h"

namespace {

int rotated{};
bool requestFullRedraw{};

bool confirmButtonPressed{};
bool confirmButtonLongPressed{};
bool backButtonPressed{};
bool backButtonLongPressed{};

class InputDispatcher
{
public:
    static void rotate(int offset)
    {
        rotated += offset;
    }

    static void confirmButton(bool pressed)
    {
        static espchrono::millis_clock::time_point pressBegin{};

        const auto now = espchrono::millis_clock::now();

        if (pressed)
            pressBegin = now;
        else
        {
            const auto duration = now - pressBegin;

            if (duration < 500ms)
                confirmButtonPressed = true;
            else if (duration < 2000ms)
                confirmButtonLongPressed = true;
            else
                requestFullRedraw = true;

            pressBegin = {};
        }
    }

    static void backButton(bool pressed)
    {
        static espchrono::millis_clock::time_point pressBegin{};

        const auto now = espchrono::millis_clock::now();

        if (pressed)
            pressBegin = now;
        else
        {
            const auto duration = now - pressBegin;

            if (duration < 500ms)
                backButtonPressed = true;
            else
                backButtonLongPressed = true;

            pressBegin = {};
        }
    }

    static void profileButton(uint8_t index, bool pressed)
    {
        if (!pressed)
            return;

        switchProfile(index);
    }
};
}
