constexpr const char * const TAG = "BOBBY";

// esp-idf includes
#include <esp_pm.h>
#include <esp_chip_info.h>
#include <esp_log.h>

// 3rdparty lib includes
#include <espchrono.h>
using namespace std::chrono_literals;
#include <espwifistack.h>
#include <tickchrono.h>
#include <espstrutils.h>
#include <screenmanager.h>

// local includes
#include "globals.h"
#include "screens.h"
#include "presets.h"
#include "statistics.h"
#ifdef FEATURE_JOYSTICK
#include "modes/wheelchairmode.h"
#else
#include "modes/defaultmode.h"
#endif
#include "screens/lockscreen.h"
#include "screens/recoverymenu.h"
#include "screens/setup/information.h"
#include "screens/setup/basic_buttons.h"
#include "screens/setup/calibrate_potis.h"
#include "screens/statusdisplay.h"
#include "newsettings.h"
#include "taskmanager.h"

#define BOOT_PROGRESS(s) \
    bobby::set_boot_msg(s); \
    ESP_LOGI("BOOT", "%s", s);

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
        bobby::initScreen();

        ESP_LOGE(TAG, "Recovery mode (%s)", espcpputils::toString(esp_reset_reason()).c_str());
        BOOT_PROGRESS("Entering recovery mode");

        if (const auto result = configs.init("bobbycar"); result != ESP_OK)
            ESP_LOGE(TAG, "config_init_settings() failed with %s", esp_err_to_name(result));

        for (auto &task : schedulerTasks)
        {
            task.setup(recovery);
        }

        espgui::switchScreen<bobby::RecoveryMenu>();

        recovery = false;

        while (true)
        {
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

    BOOT_PROGRESS("settings");

    if (const auto result = configs.init("bobbycar"); result != ESP_OK)
        ESP_LOGE(TAG, "config_init_settings() failed with %s", esp_err_to_name(result));

    bobby::initScreen();

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
            BOOT_PROGRESS(task.name());
            task.setup(false);
        }
    }

#ifdef FEATURE_JOYSTICK
    currentMode = &modes::wheelchairMode;
#else
    currentMode = &modes::defaultMode;
#endif

    BOOT_PROGRESS("switchScreen");

    if (const auto result = checkIfInCalibration(); result)
    {
        switch(*result)
        {
        case SetupStep::INFORMATION:
            BOOT_PROGRESS("Calibtration");
            espgui::switchScreen<bobby::SetupInformationDisplay>();
            break;
        case SetupStep::BASIC_BUTTONS:
            BOOT_PROGRESS("Calibtration");
            espgui::switchScreen<bobby::SetupBasicButtonsDisplay>(true);
            break;
        case SetupStep::CALIBRATE_POTIS:
            espgui::switchScreen<bobby::SetupCalibratePotisDisplay>(true);
            break;
        default:;
        }
    }
    else if (configs.lockscreen.keepLockedAfterReboot.value() && configs.lockscreen.locked.value())
    {
        BOOT_PROGRESS("Locked");
        espgui::switchScreen<bobby::StatusDisplay>();
        espgui::pushScreen<bobby::Lockscreen>();
    }
    else
    {
        BOOT_PROGRESS("StatusDisplay")
        espgui::switchScreen<bobby::StatusDisplay>();
    }

    esp_chip_info(&chip_info);
    esp_pm_get_configuration(&pm_config);

    ESP_LOGI(TAG, "Entering main loop...");

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
