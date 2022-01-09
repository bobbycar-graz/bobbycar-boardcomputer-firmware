constexpr const char * const TAG = "BOBBY";

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
#ifdef FEATURE_JOYSTICK
#include "modes/wheelchairmode.h"
#else
#include "modes/defaultmode.h"
#endif
#include "displays/statusdisplay.h"
#include "displays/lockscreen.h"
#include "displays/potiscalibratedisplay.h"
#include "displays/buttoncalibratedisplay.h"
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

    profileSettings = presets::defaultProfileSettings;

    if (settingsPersister.init())
    {
        if (!settingsPersister.openProfile(0))
            ESP_LOGE("BOBBY", "openProfile(0) failed");

        loadProfileSettings();
    }
    else
        ESP_LOGE("BOBBY", "init() failed");

    for (const auto &task : schedulerTasks)
    {
        bootLabel.redraw(task.name());
        task.setup();
    }

#ifdef FEATURE_JOYSTICK
    currentMode = &modes::wheelchairMode;
#else
    currentMode = &modes::defaultMode;
#endif

    bootLabel.redraw("switchScreen");

    if (configs.dpadMappingLeft.value == INPUT_MAPPING_NONE ||
        configs.dpadMappingRight.value == INPUT_MAPPING_NONE ||
        configs.dpadMappingUp.value == INPUT_MAPPING_NONE ||
        configs.dpadMappingDown.value == INPUT_MAPPING_NONE)
    {
        espgui::switchScreen<ButtonCalibrateDisplay>(true);
    }
    else if (configs.lockscreen.keepLockedAfterReboot.value && configs.lockscreen.locked.value)
    {
        espgui::switchScreen<Lockscreen>();
    }
    else
    {
        if (!gas || !brems || *gas > 200.f || *brems > 200.f)
            espgui::switchScreen<PotisCalibrateDisplay>(true);
        else
        {
            espgui::switchScreen<StatusDisplay>();
        }
    }

    while (true)
    {
        const auto now = espchrono::millis_clock::now();

//       if (!heap_caps_check_integrity_all(true))
//            ESP_LOGW(TAG, "OIS IM OARSCH!!!!!");

        for (auto &schedulerTask : schedulerTasks)
        {
            schedulerTask.loop();
        }

        if (!lastStatsUpdate || now - *lastStatsUpdate >= 1000ms/configs.boardcomputerHardware.timersSettings.statsUpdateRate.value)
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

        if (!battery::bootBatPercentage)
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
                {
                    battery::bootBatPercentage = getBatteryPercentage(avgVoltage, BatteryCellType(configs.battery.cellType.value));
                    battery::bootBatWh = getRemainingWattHours();
                }
            }
        }
    }
}
