#pragma once

// esp-idf includes
#include <esp_log.h>

// local includes
#include "globals.h"
#include "presets.h"

namespace
{
void switchProfile(uint8_t index)
{
#ifdef SIMPLIFIED_TRIGGER_TRIGGERONPRESET
  if (index == SIMPLIFIED_TRIGGER_TRIGGERONPRESET)
  {
    simplified = true;
#ifdef SETTINGSUTILS_PLUGIN
#include SETTINGSUTILS_PLUGIN
#endif
    return;
  }
#endif

  settings       = presets::defaultSettings;
  stringSettings = presets::makeDefaultStringSettings();

  if (!settingsPersister.openProfile(index))
  {
    ESP_LOGE("BOBBY", "openProfile() failed");
    return;
  }

  if (!settingsPersister.load(settings))
  {
    ESP_LOGE("BOBBY", "load() for settings failed");
    return;
  }

  if (!settingsPersister.load(stringSettings))
  {
    ESP_LOGE("BOBBY", "load() for stringSettings failed");
    return;
  }
}
} // namespace
