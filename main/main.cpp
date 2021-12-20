constexpr const char * const TAG = "BOBBY";

#ifndef OTA_USERNAME
#error No OTA username!
#endif

// system includes
#include <cstdio>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <espchrono.h>
using namespace std::chrono_literals;
#include <espwifistack.h>
#include <schedulertask.h>

// local includes
#include "bobbycar-common.h"
#include "bobbycar-serial.h"
#include "macros_bobbycar.h"
#include "globals.h"
#include "screens.h"
#include "presets.h"
#include "statistics.h"
#include "modes/defaultmode.h"
#include "displays/statusdisplay.h"
#include "displays/lockscreen.h"
#include "displays/calibratedisplay.h"
#include "newsettings.h"
#include "taskmanager.h"

namespace {
espchrono::millis_clock::time_point lastStatsPush;
std::optional<espchrono::millis_clock::time_point> lastStatsUpdate;
}

extern "C" void app_main()
{
#ifdef FEATURE_LEDBACKLIGHT
    pinMode(PINS_LEDBACKLIGHT, OUTPUT);
    digitalWrite(PINS_LEDBACKLIGHT, ledBacklightInverted ? LOW : HIGH);
#endif

    initScreen();

    bootLabel.redraw("settings");

    if (const auto result = configs.init("bobbycar"); result != ESP_OK)
        ESP_LOGE(TAG, "config_init_settings() failed with %s", esp_err_to_name(result));

    settings = presets::defaultSettings;
    stringSettings = presets::makeDefaultStringSettings();

    if (settingsPersister.init())
    {
        if (!settingsPersister.openCommon())
            ESP_LOGE("BOBBY", "openCommon() failed");

        if (!settingsPersister.openProfile(0))
            ESP_LOGE("BOBBY", "openProfile(0) failed");

        loadSettings();
    }
    else
        ESP_LOGE("BOBBY", "init() failed");

    bootLabel.redraw("deviceName");
    if (const auto result = wifi_stack::get_default_mac_addr())
        std::sprintf(deviceName, STRING(DEVICE_PREFIX) "_%02hhx%02hhx%02hhx", result->at(3), result->at(4), result->at(5));
    else
        ESP_LOGE("MAIN", "get_default_mac_addr() failed: %.*s", result.error().size(), result.error().data());

    for (const auto &task : schedulerTasks)
    {
        bootLabel.redraw(task.name());
        task.setup();
    }

    currentMode = &modes::defaultMode;

    bootLabel.redraw("switchScreen");

#if defined(FEATURE_DPAD_5WIRESW) && defined(DPAD_5WIRESW_DEBUG)
    switchScreen<DPad5WireDebugDisplay>();
#elif defined(FEATURE_DPAD_5WIRESW_2OUT) && defined(DPAD_5WIRESW_DEBUG)
    switchScreen<DPad5Wire2OutDebugDisplay>();
#elif defined(FEATURE_DPAD_6WIRESW) && defined(DPAD_6WIRESW_DEBUG)
    switchScreen<DPad6WireDebugDisplay>();
#else

    if (settings.lockscreen.keepLockedAfterReboot && settings.lockscreen.locked)
    {
        espgui::switchScreen<Lockscreen>();
    }
    else
    {
        if (!gas || !brems || *gas > 200.f || *brems > 200.f)
            espgui::switchScreen<CalibrateDisplay>(true);
        else
        {
            espgui::switchScreen<StatusDisplay>();
        }
    }
#endif

    while (true)
    {
        const auto now = espchrono::millis_clock::now();

        for (auto &schedulerTask : schedulerTasks)
        {
            schedulerTask.loop();
        }

        if (!lastStatsUpdate || now - *lastStatsUpdate >= 1000ms/settings.boardcomputerHardware.timersSettings.statsUpdateRate)
        {
            updateAccumulators();
            pushStats();
            lastStatsUpdate = now;
        }

        if (now - lastStatsPush >= 1s)
        {
            sched_pushStats(false);

            lastStatsPush = now;
        }

        if (battery::bootBatPercentage == -1)
        {
            if(controllers.front.feedbackValid && controllers.back.feedbackValid)
            {
                float avgVoltage = 0;
                for (auto &controller : controllers)
                {
                    avgVoltage += controller.getCalibratedVoltage();
                }
                avgVoltage = avgVoltage / controllers.size();
                if (avgVoltage > 30)
                    battery::bootBatPercentage = getBatteryPercentage(avgVoltage, BatteryCellType(settings.battery.cellType));
            }
        }
    }
}
