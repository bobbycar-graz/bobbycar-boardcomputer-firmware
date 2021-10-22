constexpr const char * const TAG = "BOBBY";

// system includes
#include <cstdio>

// esp-idf includes
#include <esp_wifi_types.h>
#include <esp_log.h>

// Arduino includes
#include <Arduino.h>

// 3rdparty lib includes
#include <espchrono.h>
using namespace std::chrono_literals;
#include <espwifistack.h>

// local includes
#include "bobbycar-common.h"
#include "bobbycar-serial.h"
#include "macros_bobbycar.h"
#include "globals.h"
#include "modes/defaultmode.h"
#include "modes/tempomatmode.h"
#include "displays/menus/aboutmenu.h"
#include "displays/menus/accesspointwifisettingsmenu.h"
#ifdef FEATURE_BLUETOOTH
#include "displays/menus/bluetoothsettingsmenu.h"
#endif
#ifdef FEATURE_BLE
#include "displays/menus/blesettingsmenu.h"
#endif
#ifdef FEATURE_CLOUD
#include "displays/menus/cloudsettingsmenu.h"
#endif
#ifdef FEATURE_BMS
#include "displays/menus/bmsmenu.h"
#endif
#include "displays/menus/buzzermenu.h"
#include "displays/menus/commanddebugmenu.h"
#include "displays/menus/crashmenu.h"
#include "displays/menus/debugmenu.h"
#include "displays/menus/defaultmodesettingsmenu.h"
#include "displays/menus/demosmenu.h"
#include "displays/menus/dynamicdebugmenu.h"
#include "displays/menus/enablemenu.h"
#include "displays/menus/feedbackdebugmenu.h"
#ifdef FEATURE_GAMETRAK
#include "displays/menus/gametrakmodesettingsmenu.h"
#endif
#include "displays/menus/genericwifisettingsmenu.h"
#include "displays/menus/graphsmenu.h"
#include "displays/menus/controllerhardwaresettingsmenu.h"
#include "displays/menus/invertmenu.h"
#include "displays/menus/larsmmodesettingsmenu.h"
#ifdef FEATURE_LEDSTRIP
#include "displays/menus/ledstripmenu.h"
#endif
#include "displays/menus/limitssettingsmenu.h"
#include "displays/menus/lockscreensettingsmenu.h"
#include "displays/menus/mainmenu.h"
#include "displays/menus/tempomatmodesettingsmenu.h"
#include "displays/menus/modessettingsmenu.h"
#ifdef FEATURE_LEDSTRIP
#include "displays/menus/ledstripcolorsmenu.h"
#endif
#ifdef FEATURE_MOSFETS
#include "displays/menus/mosfetsmenu.h"
#endif
#include "displays/menus/motorfeedbackdebugmenu.h"
#include "displays/menus/motorstatedebugmenu.h"
#include "displays/menus/profilesmenu.h"
#include "displays/menus/presetsmenu.h"
#include "displays/menus/boardcomputerhardwaresettingsmenu.h"
#include "displays/menus/selectmodemenu.h"
#include "displays/menus/settingsmenu.h"
#include "displays/menus/stationwifisettingsmenu.h"
#include "displays/menus/timersmenu.h"
#include "displays/menus/timesettingsmenu.h"
#include "displays/menus/wifiscanmenu.h"
#include "displays/menus/wifisettingsmenu.h"
#ifdef FEATURE_BMS
#include "displays/bmsdisplay.h"
#endif
#include "displays/calibratedisplay.h"
#ifdef FEATURE_DPAD_5WIRESW
#include "displays/dpad5wiredebugdisplay.h"
#endif
#ifdef FEATURE_DPAD_5WIRESW_2OUT
#include "displays/dpad5wire2outdebugdisplay.h"
#endif
#ifdef FEATURE_DPAD_6WIRESW
#include "displays/dpad6wiredebugdisplay.h"
#endif
#include "displays/gameoflifedisplay.h"
#ifdef FEATURE_GARAGE
#include "displays/garagedisplay.h"
#endif
#ifdef FEATURE_LOCKSCREEN
#include "displays/gametrakcalibratedisplay.h"
#endif
#include "displays/lockscreen.h"
#include "displays/metersdisplay.h"
#include "displays/pingpongdisplay.h"
#include "displays/poweroffdisplay.h"
#include "displays/powersupplydisplay.h"
#include "displays/spirodisplay.h"
#include "displays/starfielddisplay.h"
#include "displays/statusdisplay.h"
#ifdef FEATURE_OTA
#include "displays/updatedisplay.h"
#include "displays/menus/otamenu.h"
#include "displays/menus/selectotabuildmenu.h"
#endif
#include "screens.h"
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
#include "serialhandler.h"
#ifdef FEATURE_OTA
#include "ota.h"
#include "displays/menus/selectbuildservermenu.h"
#endif
#include "presets.h"
#include "statistics.h"
#ifdef FEATURE_BLUETOOTH
#include "actions/bluetoothbeginaction.h"
#include "actions/bluetoothbeginmasteraction.h"
#ifdef FEATURE_BMS
#include "actions/bluetoothconnectbmsaction.h"
#endif
#endif
#ifdef FEATURE_BLE
#include "ble_bobby.h"
#endif
#ifdef FEATURE_WEBSERVER
#include "webserver.h"
#endif
#ifdef FEATURE_CAN
#include "can.h"
#endif
#ifdef FEATURE_CLOUD
#include "cloud.h"
#endif
#include "wifi_bobbycar.h"
#include "time_bobbycar.h"
#ifdef FEATURE_LEDSTRIP
#include "ledstrip.h"
#endif

namespace {
std::optional<espchrono::millis_clock::time_point> lastWifiUpdate;
std::optional<espchrono::millis_clock::time_point> lastPotiRead;
std::optional<espchrono::millis_clock::time_point> lastModeUpdate;
std::optional<espchrono::millis_clock::time_point> lastStatsUpdate;
std::optional<espchrono::millis_clock::time_point> lastDisplayUpdate;
std::optional<espchrono::millis_clock::time_point> lastDisplayRedraw;
#ifdef FEATURE_CAN
std::optional<espchrono::millis_clock::time_point> lastCanParse;
#endif
#ifdef FEATURE_BLE
std::optional<espchrono::millis_clock::time_point> lastBleUpdate;
#endif
#ifdef FEATURE_CLOUD
std::optional<espchrono::millis_clock::time_point> lastCloudCollect;
std::optional<espchrono::millis_clock::time_point> lastCloudSend;
#endif
#ifdef FEATURE_NTP
std::optional<espchrono::millis_clock::time_point> lastNtpUpdate;
#endif
#ifdef FEATURE_LEDSTRIP
std::optional<espchrono::millis_clock::time_point> lastLedstripUpdate;
#endif
}

extern "C" void app_main()
{
    Serial.begin(115200);
    //Serial.setDebugOutput(true);
    //Serial.println("setup()");

#ifdef FEATURE_LEDBACKLIGHT
    pinMode(PINS_LEDBACKLIGHT, OUTPUT);
    digitalWrite(PINS_LEDBACKLIGHT, ledBacklightInverted ? LOW : HIGH);
#endif

    pinMode(3, INPUT_PULLUP);

    currentlyReverseBeeping = false;

    initScreen();

    bootLabel.redraw("settings");
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

    bootLabel.redraw("wifi");
    wifi_begin();

#ifdef FEATURE_DPAD
    bootLabel.redraw("dpad");
    dpad::init();
#endif

#ifdef FEATURE_DPAD_3WIRESW
    bootLabel.redraw("dpad3wire");
    dpad3wire::init();
#endif

#ifdef FEATURE_DPAD_5WIRESW
    bootLabel.redraw("dpad5wire");
    dpad5wire::init();
#endif

#ifdef FEATURE_DPAD_5WIRESW_2OUT
    bootLabel.redraw("dpad5wire_2out");
    dpad5wire_2out::init();
#endif

#ifdef FEATURE_DPAD_6WIRESW
    bootLabel.redraw("dpad6wire");
    dpad6wire::init();
#endif

#ifdef FEATURE_ROTARY
    bootLabel.redraw("rotary");
    initRotary();
#endif

#ifdef FEATURE_MOSFETS
    bootLabel.redraw("mosfets");
    pinMode(PINS_MOSFET0, OUTPUT);
    pinMode(PINS_MOSFET1, OUTPUT);
    pinMode(PINS_MOSFET2, OUTPUT);

    digitalWrite(PINS_MOSFET0, LOW);
    digitalWrite(PINS_MOSFET1, LOW);
    digitalWrite(PINS_MOSFET2, LOW);
#endif

#ifdef FEATURE_SERIAL
    bootLabel.redraw("swap front back");
    updateSwapFrontBack();
#endif

#ifdef FEATURE_BLUETOOTH
    if (settings.bluetoothSettings.autoBluetoothMode == BluetoothMode::Master)
    {
        bootLabel.redraw("bluetooth begin master");
        BluetoothBeginMasterAction{}.triggered();
#ifdef FEATURE_BMS
        if (settings.autoConnectBms)
        {
            bootLabel.redraw("connect BMS");
            BluetoothConnectBmsAction{}.triggered();
        }
#endif
    }
    else if (settings.bluetoothSettings.autoBluetoothMode == BluetoothMode::Slave)
    {
        bootLabel.redraw("bluetooth begin");
        BluetoothBeginAction{}.triggered();
    }
#endif

#ifdef FEATURE_CAN
    bootLabel.redraw("can");
    can::initCan();
#endif

#ifdef FEATURE_SERIAL
    bootLabel.redraw("front Serial begin");
    controllers.front.serial.get().begin(38400, SERIAL_8N1, PINS_RX1, PINS_TX1);

    bootLabel.redraw("back Serial begin");
    controllers.back.serial.get().begin(38400, SERIAL_8N1, PINS_RX2, PINS_TX2);
#endif

#ifdef FEATURE_LEDSTRIP
    bootLabel.redraw("LED strip");
    initLedStrip();
#endif

    raw_gas = std::nullopt;
    raw_brems = std::nullopt;
    gas = std::nullopt;
    brems = std::nullopt;

    for (Controller &controller : controllers)
        controller.command.buzzer = {};

    currentMode = &modes::defaultMode;

#ifdef FEATURE_OTA
    bootLabel.redraw("ota");
    initOta();
#endif

#ifdef FEATURE_BLE
    bootLabel.redraw("ble");
    initBle();
#endif

#ifdef FEATURE_WEBSERVER
    bootLabel.redraw("webserver");
    initWebserver();
#endif

    bootLabel.redraw("potis");
    readPotis();

#ifdef FEATURE_CLOUD
    bootLabel.redraw("cloud");
    initCloud();
#endif

#ifdef FEATURE_NTP
    bootLabel.redraw("time");
    initTime();
#endif

    bootLabel.redraw("switchScreen");

#if defined(FEATURE_DPAD_5WIRESW) && defined(DPAD_5WIRESW_DEBUG)
    switchScreen<DPad5WireDebugDisplay>();
#elif defined(FEATURE_DPAD_5WIRESW_2OUT) && defined(DPAD_5WIRESW_DEBUG)
    switchScreen<DPad5Wire2OutDebugDisplay>();
#elif defined(FEATURE_DPAD_6WIRESW) && defined(DPAD_6WIRESW_DEBUG)
    switchScreen<DPad6WireDebugDisplay>();
#else

    if (!gas || !brems || *gas > 200.f || *brems > 200.f)
        switchScreen<CalibrateDisplay>(true);
    else
        switchScreen<StatusDisplay>();
#endif

    while (true)
    {
        const auto now = espchrono::millis_clock::now();

        if (!lastWifiUpdate || now - *lastWifiUpdate >= 100ms)
        {
            wifi_update();

            lastWifiUpdate = now;
        }

        InputDispatcher::update();

#ifdef FEATURE_DPAD
        dpad::update();
#endif

#ifdef FEATURE_DPAD_3WIRESW
        dpad3wire::update();
#endif

#ifdef FEATURE_DPAD_5WIRESW
        dpad5wire::update();
#endif
#ifdef FEATURE_DPAD_5WIRESW_2OUT
        dpad5wire_2out::update();
#endif
#ifdef FEATURE_DPAD_6WIRESW
        dpad6wire::update();
#endif

        if (!lastPotiRead || now - *lastPotiRead >= 1000ms/settings.boardcomputerHardware.timersSettings.potiReadRate)
        {
            readPotis();

            lastPotiRead = now;
        }

        if (!lastModeUpdate || now - *lastModeUpdate >= 1000ms/settings.boardcomputerHardware.timersSettings.modeUpdateRate)
        {
            if (lastMode != currentMode)
            {
                if (lastMode)
                    lastMode->stop();
                lastMode = currentMode;
                if (currentMode)
                    currentMode->start();
            }

            if (currentMode)
                currentMode->update();

            lastModeUpdate = now;

            performance.current++;
        }

        if (!lastStatsUpdate || now - *lastStatsUpdate >= 1000ms/settings.boardcomputerHardware.timersSettings.statsUpdateRate)
        {
            updateAccumulators();
            pushStats();
            lastStatsUpdate = now;
        }

        if (!lastDisplayUpdate || now - *lastDisplayUpdate >= 1000ms/settings.boardcomputerHardware.timersSettings.displayUpdateRate)
        {
            updateDisplay();

            lastDisplayUpdate = now;
        }

        if (!lastDisplayRedraw || now - *lastDisplayRedraw >= 1000ms/settings.boardcomputerHardware.timersSettings.displayRedrawRate)
        {
            redrawDisplay();

            lastDisplayRedraw = now;
        }

        if (now - performance.lastTime >= 1000ms)
        {
            performance.last = performance.current;
            performance.current = 0;
            performance.lastTime = now;
        }

#ifdef FEATURE_CAN
        if (!lastCanParse || now - *lastCanParse >= 1000ms/settings.boardcomputerHardware.timersSettings.canReceiveRate)
        {
            //can::tryParseCanInput();
            can::parseCanInput();

            lastCanParse = now;
        }
#endif

#ifdef FEATURE_SERIAL
        for (Controller &controller : controllers)
            controller.parser.update();
#endif

        handleSerial();

#ifdef FEATURE_OTA
        handleOta();
#endif

#ifdef FEATURE_BLE
        if (!lastBleUpdate || now - *lastBleUpdate >= 250ms)
        {
            handleBle();

            lastBleUpdate = now;
        }
#endif

#ifdef FEATURE_CLOUD
        if (!lastCloudCollect || now - *lastCloudCollect >= std::chrono::milliseconds{settings.boardcomputerHardware.timersSettings.cloudCollectRate})
        {
            cloudCollect();

            lastCloudCollect = now;
        }

        if (!lastCloudSend || now - *lastCloudSend >= 1000ms/settings.boardcomputerHardware.timersSettings.cloudSendRate)
        {
            cloudSend();

            lastCloudSend = now;
        }
#endif

#ifdef FEATURE_NTP
        if (!lastNtpUpdate || now - *lastNtpUpdate >= 100ms)
        {
            updateTime();

            lastNtpUpdate = now;
        }
#endif

#ifdef FEATURE_WEBSERVER
        handleWebserver();
#endif

#ifdef FEATURE_BMS
        bms::update();
#endif

#ifdef FEATURE_LEDSTRIP
        if (!lastLedstripUpdate || now - *lastLedstripUpdate >= 1000ms / 60)
        {
            updateLedStrip();

            lastLedstripUpdate = now;
        }
#endif
    }
}
