#pragma once

#include "actioninterface.h"
#include "globals.h"
#include "presets.h"

namespace {
class EraseNvsAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        const auto profile = settingsPersister.currentlyOpenProfileIndex();

        if (!settingsPersister.erase())
        {
            //Serial.println("EraseNvsAction::triggered() erase failed");
            return;
        }

        settings = presets::defaultSettings;

        if (!profile)
            return;

        if (!settingsPersister.openProfile(*profile))
        {
            //Serial.println("EraseNvsAction::triggered() openProfile failed");
            return;
        }

        if (!settingsPersister.load(settings))
        {
            //Serial.println("EraseNvsAction::triggered() load failed");
            return;
        }
    }
};
}
