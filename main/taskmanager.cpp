#include "taskmanager.h"

#include "sdkconfig.h"

// system includes
#include <iterator>
#include <chrono>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <schedulertask.h>

// local includes
#include "wifi_bobbycar.h"
#include "buttons.h"
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
#ifdef FEATURE_NTP
#include "time_bobbycar.h"
#endif
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
#ifdef FEATURE_LEDSTRIP
#include "ledstrip.h"
#endif
#ifdef FEATURE_ESPNOW
#include "espnowfunctions.h"
#endif
#ifdef FEATURE_CLOUD
#include "cloud.h"
#endif
#ifdef FEATURE_UDPCLOUD
#include "udpcloud.h"
#endif
#include "modes.h"
#include "drivingstatistics.h"
#ifdef FEATURE_DNS_NS
#include "dnsannounce.h"
#endif
#include "screens.h"

using namespace std::chrono_literals;

namespace {
constexpr const char * const TAG = "TASKS";

void not_needed() {}

espcpputils::SchedulerTask schedulerTasksArr[] {
    espcpputils::SchedulerTask { "wifi",           wifi_begin,            wifi_update,             100ms },
    espcpputils::SchedulerTask { "input",          InputDispatcher::init, InputDispatcher::update, 20ms  },
#ifdef FEATURE_DPAD
    espcpputils::SchedulerTask { "dpad",           dpad::init,            dpad::update,            20ms  },
#endif
#ifdef FEATURE_DPAD_3WIRESW
    espcpputils::SchedulerTask { "dpad3wire",      dpad3wire::init,       dpad3wire::update,       20ms  },
#endif
#ifdef FEATURE_DPAD_5WIRESW
    espcpputils::SchedulerTask { "dpad5wire",      dpad5wire::init,       dpad5wire::update,       20ms  },
#endif
#ifdef FEATURE_DPAD_5WIRESW_2OUT
    espcpputils::SchedulerTask { "dpad5wire_2out", dpad5wire_2out::init,  dpad5wire_2out::update,  20ms  },
#endif
#ifdef FEATURE_DPAD_6WIRESW
    espcpputils::SchedulerTask { "dpad6wire",      dpad6wire::init,       dpad6wire::update,       20ms  },
#endif
#ifdef FEATURE_ROTARY
    espcpputils::SchedulerTask { "rotary",         initRotary,            updateRotary,            20ms  },
#endif
#ifdef FEATURE_MOSFETS
    espcpputils::SchedulerTask { "mosfets",        init_mosfets,          update_mosfets,          100ms },
#endif
    espcpputils::SchedulerTask { "wifi",           wifi_begin,            wifi_update,             100ms },
#ifdef FEATURE_NTP
    espcpputils::SchedulerTask { "time",           initTime,              updateTime,              100ms },
#endif
    espcpputils::SchedulerTask { "potis",          initPotis,             readPotis,               20ms  },
#ifdef FEATURE_BLUETOOTH
    espcpputils::SchedulerTask { "bluetooth",      bluetooth_init,        bluetooth_update,        100ms },
#ifdef FEATURE_BMS
    espcpputils::SchedulerTask { "bms",            bms::init,             bms::update,             100ms },
#endif
#endif
#ifdef FEATURE_CAN
    espcpputils::SchedulerTask { "can",            can::initCan,          can::parseCanInput,      10ms  },
#endif
    espcpputils::SchedulerTask { "debuginput",     initDebugInput,        handleDebugInput,        50ms  },
#ifdef FEATURE_SERIAL
    espcpputils::SchedulerTask { "serial",         initSerial,            updateSerial,            50ms  },
#endif
#ifdef FEATURE_OTA
    espcpputils::SchedulerTask { "ota",            initOta,               handleOta,               50ms  },
#endif
#ifdef FEATURE_BLE
    espcpputils::SchedulerTask { "ble",            initBle,               handleBle,               100ms },
#endif
#ifdef FEATURE_WEBSERVER
    espcpputils::SchedulerTask { "webserver",      initWebserver,         handleWebserver,         100ms },
#endif
#ifdef FEATURE_LEDSTRIP
    espcpputils::SchedulerTask { "ledstrip",       initLedStrip,          updateLedStrip,          30ms },
#endif
#ifdef FEATURE_ESPNOW
    espcpputils::SchedulerTask { "espnow",         espnow::initESPNow,    espnow::handle,          100ms },
#endif
#ifdef FEATURE_CLOUD
    espcpputils::SchedulerTask { "cloud",          initCloud,             updateCloud,             50ms },
#endif
#ifdef FEATURE_UDPCLOUD
    espcpputils::SchedulerTask { "udpcloud",       udpCloudInit,          udpCloudUpdate,          50ms },
#endif
    espcpputils::SchedulerTask { "drivingmode",    initDrivingMode,       updateDrivingMode,       20ms },
    espcpputils::SchedulerTask { "drivingstatistics", initStatistics,     calculateStatistics,     100ms },
#ifdef FEATURE_DNS_NS
    espcpputils::SchedulerTask { "dnsannounce",    init_dns_announce,     handle_dns_announce,     100ms },
#endif
    espcpputils::SchedulerTask { "updateDisp",     not_needed,            updateDisplay,           20ms },
    espcpputils::SchedulerTask { "redrawDisp",     not_needed,            redrawDisplay,           20ms },
};
} // namespace

cpputils::ArrayView<espcpputils::SchedulerTask> schedulerTasks{std::begin(schedulerTasksArr), std::end(schedulerTasksArr)};

const espcpputils::SchedulerTask &drivingModeTask = []() -> const espcpputils::SchedulerTask & {
    auto iter = std::find_if(std::begin(schedulerTasksArr), std::end(schedulerTasksArr), [](const espcpputils::SchedulerTask &task){
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
