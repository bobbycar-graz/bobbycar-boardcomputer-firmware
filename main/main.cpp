constexpr const char * const TAG = "BOBBY";

// system includes
#include <cstdio>

// esp-idf includes
#include <esp32/pm.h>
#include <esp_pm.h>
#include <esp_chip_info.h>
#include <esp_log.h>

// 3rdparty lib includes
#include <espchrono.h>
using namespace std::chrono_literals;
#include <espwifistack.h>
#include <schedulertask.h>
#include <screenmanager.h>
#include <tickchrono.h>
#include <espstrutils.h>

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
#include "displays/buttoncalibratedisplay.h"
#include "displays/lockscreen.h"
#include "displays/menus/recoverymenu.h"
#include "displays/potiscalibratedisplay.h"
#include "displays/statusdisplay.h"
#include "newsettings.h"
#include "taskmanager.h"
#include "buzzer.h"

namespace {
espchrono::millis_clock::time_point lastStatsPush;
std::optional<espchrono::millis_clock::time_point> lastStatsUpdate;
RTC_NOINIT_ATTR bool recovery;
} // namespace

extern "C" void app_main()
{
#ifdef FEATURE_LEDBACKLIGHT
    pinMode(PINS_LEDBACKLIGHT, OUTPUT);
    digitalWrite(PINS_LEDBACKLIGHT, ledBacklightInverted ? LOW : HIGH);
#endif

    if (const auto reset_reason = esp_reset_reason(); reset_reason == ESP_RST_POWERON)
    {
        recovery = false;
    }

    if (recovery)
    {
        initScreen();

        ESP_LOGE(TAG, "Recovery mode (%s)", espcpputils::toString(esp_reset_reason()).c_str());
        bootLabel.redraw("Entering recovery mode");

        if (const auto result = configs.init("bobbycar"); result != ESP_OK)
            ESP_LOGE(TAG, "config_init_settings() failed with %s", esp_err_to_name(result));

        for (auto &task : schedulerTasks)
        {
            task.setup(recovery);
        }

        espgui::switchScreen<RecoveryMenu>();

        recovery = false;

        while (true)
        {
            const auto now = espchrono::millis_clock::now();

            for (auto &schedulerTask : schedulerTasks)
            {
                if (schedulerTask.isInitialized())
                    schedulerTask.loop();
            }

            espcpputils::delay(1ms);
        };
    }
    else
    {
        recovery = true;
    }

    bootLabel.redraw("settings");

    if (const auto result = configs.init("bobbycar"); result != ESP_OK)
        ESP_LOGE(TAG, "config_init_settings() failed with %s", esp_err_to_name(result));

    initScreen();

    profileSettings = presets::defaultProfileSettings;

    if (settingsPersister.init())
    {
        if (!settingsPersister.openProfile(0))
            ESP_LOGE("BOBBY", "openProfile(0) failed");

        loadProfileSettings();
    }
    else
        ESP_LOGE("BOBBY", "init() failed");

    for (auto &task : schedulerTasks)
    {
        if (checkEnabledByName(task.name()))
        {
            bootLabel.redraw(task.name());
            task.setup(false);
        }
    }

#ifdef FEATURE_JOYSTICK
    currentMode = &modes::wheelchairMode;
#else
    currentMode = &modes::defaultMode;
#endif

    bootLabel.redraw("switchScreen");

    if (configs.dpadMappingLeft.value() == INPUT_MAPPING_NONE ||
        configs.dpadMappingRight.value() == INPUT_MAPPING_NONE ||
        configs.dpadMappingUp.value() == INPUT_MAPPING_NONE ||
        configs.dpadMappingDown.value() == INPUT_MAPPING_NONE)
    {
        espgui::switchScreen<ButtonCalibrateDisplay>(true);
    }
    else if (configs.lockscreen.keepLockedAfterReboot.value() && configs.lockscreen.locked.value())
    {
        espgui::switchScreen<StatusDisplay>();
        espgui::pushScreen<Lockscreen>();
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

    esp_chip_info(&chip_info);
    esp_pm_get_configuration(&pm_config);

    buzzer::init();

    while (true)
    {
        const auto now = espchrono::millis_clock::now();

        if (recovery && now.time_since_epoch() > 5s)
        {
            ESP_LOGI(TAG, "Booting successful, disabling recovery...");
            recovery = false;
        }

//       if (!heap_caps_check_integrity_all(true))
//            ESP_LOGW(TAG, "OIS IM OARSCH!!!!!");

        for (auto &schedulerTask : schedulerTasks)
        {
            if (schedulerTask.isInitialized())
                schedulerTask.loop();
        }

        if (!lastStatsUpdate || now - *lastStatsUpdate >= 1000ms/configs.boardcomputerHardware.timersSettings.statsUpdateRate.value())
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
                if (const auto avgVoltage = controllers.getAvgVoltage(); avgVoltage)
                {
                    if (avgVoltage > 30)
                    {
                        battery::bootBatPercentage = getBatteryPercentage(*avgVoltage, BatteryCellType(configs.battery.cellType.value()));
                        battery::bootBatWh = getRemainingWattHours();
                    }
                }
            }
        }

        espcpputils::delay(1ms);
    }
}
