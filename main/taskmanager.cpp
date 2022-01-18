#include "taskmanager.h"

#include "sdkconfig.h"

// system includes
#include <iterator>
#include <chrono>

// esp-idf includes
#include <esp_log.h>

// local includes
#include "wifi_bobbycar.h"
#include "dpad.h"
#ifdef FEATURE_DPAD_3WIRESW
#include "dpad3wire.h"
#endif
#ifdef FEATURE_DPAD_5WIRESW
#include "dpad5wire.h"
#endif
#ifdef FEATURE_DPAD_5WIRESW_2OUT
#include "dpad5wire_2out.h"
#endif
#ifdef FEATURE_DPAD_6WIRESW
#include "dpad6wire.h"
#endif
#ifdef FEATURE_ROTARY
#include "rotary.h"
#endif
#ifdef FEATURE_MOSFETS
#include "mosfets.h"
#endif
#include "time_bobbycar.h"
#include "potis.h"
#ifdef FEATURE_BLUETOOTH
#include "bluetooth_bobby.h"
#ifdef FEATURE_BMS
#include "bmsutils.h"
#endif
#endif
#ifdef FEATURE_CAN
#include "can.h"
#endif
#include "debuginputhandler.h"
#ifdef FEATURE_SERIAL
#include "serial_bobby.h"
#endif
#ifdef FEATURE_OTA
#include "ota.h"
#endif
#ifdef FEATURE_BLE
#include "ble_bobby.h"
#endif
#ifdef FEATURE_WEBSERVER
#include "webserver.h"
#endif
#include "ledstrip.h"
#ifdef FEATURE_ESPNOW
#include "espnowfunctions.h"
#endif
#include "cloud.h"
#include "udpcloud.h"
#include "modes.h"
#include "drivingstatistics.h"
#include "dnsannounce.h"
#include "screens.h"

using namespace std::chrono_literals;

namespace {
constexpr const char * const TAG = "TASKS";

void not_needed() {}

BobbySchedulerTask schedulerTasksArr[] {
    BobbySchedulerTask { "wifi",           wifi_begin,            wifi_update,             100ms },
#ifdef FEATURE_DPAD
    BobbySchedulerTask { "dpad",           dpad::init,            dpad::update,            20ms  },
#endif
#ifdef FEATURE_DPAD_3WIRESW
    BobbySchedulerTask { "dpad3wire",      dpad3wire::init,       dpad3wire::update,       20ms  },
#endif
#ifdef FEATURE_DPAD_5WIRESW
    BobbySchedulerTask { "dpad5wire",      dpad5wire::init,       dpad5wire::update,       20ms  },
#endif
#ifdef FEATURE_DPAD_5WIRESW_2OUT
    BobbySchedulerTask { "dpad5wire_2out", dpad5wire_2out::init,  dpad5wire_2out::update,  20ms  },
#endif
#ifdef FEATURE_DPAD_6WIRESW
    BobbySchedulerTask { "dpad6wire",      dpad6wire::init,       dpad6wire::update,       20ms  },
#endif
#ifdef FEATURE_ROTARY
    BobbySchedulerTask { "rotary",         initRotary,            updateRotary,            20ms  },
#endif
#ifdef FEATURE_MOSFETS
    BobbySchedulerTask { "mosfets",        init_mosfets,          update_mosfets,          100ms },
#endif
    BobbySchedulerTask { "time",           initTime,              updateTime,              100ms },
    BobbySchedulerTask { "potis",          initPotis,             readPotis,               20ms  },
#ifdef FEATURE_BLUETOOTH
    BobbySchedulerTask { "bluetooth",      bluetooth_init,        bluetooth_update,        100ms },
#ifdef FEATURE_BMS
    BobbySchedulerTask { "bms",            bms::init,             bms::update,             100ms },
#endif
#endif
#ifdef FEATURE_CAN
    BobbySchedulerTask { "can",            can::initCan,          can::updateCan,          10ms  },
#endif
    BobbySchedulerTask { "debuginput",     initDebugInput,        handleDebugInput,        50ms  },
#ifdef FEATURE_SERIAL
    BobbySchedulerTask { "serial",         initSerial,            updateSerial,            50ms  },
#endif
#ifdef FEATURE_OTA
    BobbySchedulerTask { "ota",            initOta,               handleOta,               50ms  },
#endif
#ifdef FEATURE_BLE
    BobbySchedulerTask { "ble",            initBle,               handleBle,               100ms },
#endif
#ifdef FEATURE_WEBSERVER
    BobbySchedulerTask { "webserver",      initWebserver,         handleWebserver,         100ms },
#endif
    BobbySchedulerTask { "ledstrip",       initLedStrip,          updateLedStrip,          30ms },
#ifdef FEATURE_ESPNOW
    BobbySchedulerTask { "espnow",         espnow::initESPNow,    espnow::handle,          100ms },
#endif
    BobbySchedulerTask { "cloud",          initCloud,             updateCloud,             50ms },
    BobbySchedulerTask { "udpcloud",       udpCloudInit,          udpCloudUpdate,          50ms },
    BobbySchedulerTask { "drivingmode",    initDrivingMode,       updateDrivingMode,       20ms },
    BobbySchedulerTask { "drivingstatistics", initStatistics,     calculateStatistics,     100ms },
    BobbySchedulerTask { "dnsannounce",    init_dns_announce,     handle_dns_announce,     100ms },
    BobbySchedulerTask { "updateDisp",     not_needed,            updateDisplay,           20ms },
    BobbySchedulerTask { "redrawDisp",     not_needed,            redrawDisplay,           20ms },
};
} // namespace

cpputils::ArrayView<BobbySchedulerTask> schedulerTasks{std::begin(schedulerTasksArr), std::end(schedulerTasksArr)};

const BobbySchedulerTask &drivingModeTask = []() -> const BobbySchedulerTask & {
    auto iter = std::find_if(std::begin(schedulerTasksArr), std::end(schedulerTasksArr), [](const BobbySchedulerTask &task){
        return std::string_view{task.name()} == "drivingmode";
    });
    return *iter;
}();

void sched_pushStats(bool printTasks)
{
    if (printTasks)
        ESP_LOGI(TAG, "begin listing tasks...");

    for (auto &schedulerTask : schedulerTasks)
        schedulerTask.pushStats(printTasks);

    if (printTasks)
        ESP_LOGI(TAG, "end listing tasks");
}
