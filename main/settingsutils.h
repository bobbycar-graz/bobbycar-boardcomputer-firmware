#pragma once

// local includes
#include "globals.h"
#include "presets.h"

namespace {
void switchProfile(uint8_t index)
{
    settings = presets::defaultSettings;

    if (settingsPersister.openProfile(index))
    {
        if (!settingsPersister.load(settings))
        {
            //Serial.println("switchProfile() load failed");
        }
    }
    else
    {
        //Serial.println("switchProfile() openProfile failed");
    }
}
}
