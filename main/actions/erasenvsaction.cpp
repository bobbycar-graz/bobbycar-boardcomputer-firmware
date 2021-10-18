#include "erasenvsaction.h"

// esp-idf includes
#include <esp_log.h>

// local includes
#include "globals.h"
#include "presets.h"

void EraseNvsAction::triggered()
{
    const auto profile = settingsPersister.currentlyOpenProfileIndex();

    if (!settingsPersister.erase())
    {
        ESP_LOGE("BOBBY", "erase() failed");
        //return;
    }

    settings = presets::defaultSettings;
    stringSettings = presets::makeDefaultStringSettings();

    if (!settingsPersister.openCommon())
    {
        ESP_LOGE("BOBBY", "openCommon() failed");
        //return;
    }

    if (profile)
    {
        if (!settingsPersister.openProfile(*profile))
        {
            ESP_LOGE("BOBBY", "openProfile(%hhu) failed", *profile);
            //return;
        }
    }

    if (!settingsPersister.load(settings))
    {
        ESP_LOGE("BOBBY", "load() for settings failed");
        //return;
    }

    if (!settingsPersister.load(stringSettings))
    {
        ESP_LOGE("BOBBY", "load() for stringSettings failed");
        //return;
    }
}
