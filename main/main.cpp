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
#include "displays/menus/bmsmenu.h"
#include "displays/menus/buzzermenu.h"
#include "displays/menus/commanddebugmenu.h"
#include "displays/menus/debugmenu.h"
#include "displays/menus/defaultmodesettingsmenu.h"
#include "displays/menus/demosmenu.h"
#include "displays/menus/dynamicdebugmenu.h"
#include "displays/menus/enablemenu.h"
#include "displays/menus/feedbackdebugmenu.h"
#include "displays/menus/gametrakmodesettingsmenu.h"
#include "displays/menus/genericwifisettingsmenu.h"
#include "displays/menus/graphsmenu.h"
#include "displays/menus/controllerhardwaresettingsmenu.h"
#include "displays/menus/invertmenu.h"
#include "displays/menus/larsmmodesettingsmenu.h"
#include "displays/menus/limitssettingsmenu.h"
#include "displays/menus/mainmenu.h"
#include "displays/menus/tempomatmodesettingsmenu.h"
#include "displays/menus/modessettingsmenu.h"
#include "displays/menus/mosfetsmenu.h"
#include "displays/menus/motorfeedbackdebugmenu.h"
#include "displays/menus/motorstatedebugmenu.h"
#include "displays/menus/profilesmenu.h"
#include "displays/menus/presetsmenu.h"
#include "displays/menus/boardcomputerhardwaresettingsmenu.h"
#include "displays/menus/selectmodemenu.h"
#include "displays/menus/settingsmenu.h"
#include "displays/menus/stationwifisettingsmenu.h"
#include "displays/menus/timersmenu.h"
#include "displays/menus/wifiscanmenu.h"
#include "displays/menus/wifisettingsmenu.h"
#include "displays/bmsdisplay.h"
#include "displays/calibratedisplay.h"
#include "displays/dpad5wiredebugdisplay.h"
#include "displays/gameoflifedisplay.h"
#include "displays/gametrakcalibratedisplay.h"
#include "displays/lockscreen.h"
#include "displays/metersdisplay.h"
#include "displays/pingpongdisplay.h"
#include "displays/poweroffdisplay.h"
#include "displays/powersupplydisplay.h"
#include "displays/spirodisplay.h"
#include "displays/starfielddisplay.h"
#include "displays/statusdisplay.h"
#include "displays/updatedisplay.h"
#include "screens.h"
#include "dpad.h"
#include "dpad3wire.h"
#include "dpad5wire.h"
#include "rotary.h"
#include "serialhandler.h"
#include "ota.h"
#include "presets.h"
#include "statistics.h"
#include "actions/wifisoftapaction.h"
#ifdef FEATURE_BLUETOOTH
#include "actions/bluetoothbeginaction.h"
#include "actions/bluetoothbeginmasteraction.h"
#include "actions/bluetoothconnectbmsaction.h"
#endif
#include "bobby_webserver.h"
#ifdef FEATURE_CAN
#include "can.h"
#endif
#ifdef FEATURE_CLOUD
#include "cloud.h"
#endif
#include "wifi_bobbycar.h"

namespace {
ModeInterface *lastMode{};
std::optional<espchrono::millis_clock::time_point> lastPotiRead;
std::optional<espchrono::millis_clock::time_point> lastModeUpdate;
std::optional<espchrono::millis_clock::time_point> lastStatsUpdate;
std::optional<espchrono::millis_clock::time_point> lastDisplayUpdate;
std::optional<espchrono::millis_clock::time_point> lastDisplayRedraw;
}

void printMemoryStats(const char *s)
{
    //Serial.printf("MEMORY %s 8bit: %u 32bit: %u\r\n",
    //              s,
    //              heap_caps_get_free_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT),
    //              heap_caps_get_free_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_32BIT));
}

extern "C" void app_main()
{
    Serial.begin(115200);
    //Serial.setDebugOutput(true);
    //Serial.println("setup()");

#ifdef PINS_LED
    pinMode(PINS_LED, OUTPUT);
    digitalWrite(PINS_LED, LOW);
#endif

    printMemoryStats("setup()");

    pinMode(3, INPUT_PULLUP);

    currentlyReverseBeeping = false;

    initScreen();
    printMemoryStats("initScreen()");

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

    bootLabel.redraw("settings");
    settings = presets::defaultSettings;

    if (settingsPersister.init())
    {
        if (settingsPersister.openProfile(0))
        {
            loadSettings();
        }
    }
    printMemoryStats("loadSettings()");

    bootLabel.redraw("deviceName");
    if (const auto result = wifi_stack::get_default_mac_addr())
        std::sprintf(deviceName, STRING(DEVICE_PREFIX) "_%02hhx%02hhx%02hhx", result->at(3), result->at(4), result->at(5));
    else
        ESP_LOGE("MAIN", "get_default_mac_addr() failed: %.*s", result.error().size(), result.error().data());
    printMemoryStats("deviceName");

    bootLabel.redraw("wifi");
    wifi_begin();
    printMemoryStats("wifi_begin()");

#ifdef FEATURE_SERIAL
    bootLabel.redraw("swap front back");
    updateSwapFrontBack();
    printMemoryStats("swapFronBack()");
#endif

#ifdef FEATURE_BLUETOOTH
    if (settings.bluetoothSettings.autoBluetoothMode == BluetoothMode::Master)
    {
        bootLabel.redraw("bluetooth begin master");
        BluetoothBeginMasterAction{}.triggered();
        printMemoryStats("BluetoothBeginMasterAction()");
#ifdef FEATURE_BMS
        if (settings.autoConnectBms)
        {
            bootLabel.redraw("connect BMS");
            BluetoothConnectBmsAction{}.triggered();
            printMemoryStats("BluetoothConnectBmsAction()");
        }
#endif
    }
    else if (settings.bluetoothSettings.autoBluetoothMode == BluetoothMode::Slave)
    {
        bootLabel.redraw("bluetooth begin");
        BluetoothBeginAction{}.triggered();
        printMemoryStats("BluetoothBeginAction()");
    }
#endif

#ifdef FEATURE_CAN
    can::initCan();
#endif

#ifdef FEATURE_SERIAL
    bootLabel.redraw("front Serial begin");
    controllers.front.serial.get().begin(38400, SERIAL_8N1, PINS_RX1, PINS_TX1);

    bootLabel.redraw("back Serial begin");
    controllers.back.serial.get().begin(38400, SERIAL_8N1, PINS_RX2, PINS_TX2);
#endif

    raw_gas = std::nullopt;
    raw_brems = std::nullopt;
    gas = std::nullopt;
    brems = std::nullopt;

    for (Controller &controller : controllers)
        controller.command.buzzer = {};

    currentMode = &modes::defaultMode;

#ifdef FEATURE_ARDUINOOTA
    bootLabel.redraw("ota");
    initOta();
    printMemoryStats("initOta()");
#endif

#ifdef FEATURE_WEBSERVER
    bootLabel.redraw("webserver");
    initWebserver();
    printMemoryStats("initWebserver()");
#endif

    bootLabel.redraw("potis");
    readPotis();
    printMemoryStats("readPotis()");

#ifdef FEATURE_CLOUD
    startCloud();
#endif

#if defined(FEATURE_DPAD_5WIRESW) && defined(DPAD_5WIRESW_DEBUG)
    switchScreen<DPad5WireDebugDisplay>();
    return;
#endif

    if (!gas || !brems || *gas > 200.f || *brems > 200.f)
        switchScreen<CalibrateDisplay>(true);
    else
        switchScreen<StatusDisplay>();

    printMemoryStats("switchScreen()");

    while (true)
    {
        //Serial.println("loop()");
        //printMemoryStats("loop()");

        const auto now = espchrono::millis_clock::now();

        wifi_update();

#ifdef FEATURE_DPAD
        dpad::update();
#endif

#ifdef FEATURE_DPAD_3WIRESW
        dpad3wire::update();
#endif

#ifdef FEATURE_DPAD_5WIRESW
        dpad5wire::update();
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
        can::parseCanInput();
#endif

#ifdef FEATURE_SERIAL
        for (Controller &controller : controllers)
            controller.parser.update();
#endif

        handleSerial();

#ifdef FEATURE_ARDUINOOTA
        handleOta();
#endif

#ifdef FEATURE_WEBSERVER
        handleWebserver();
#endif

#ifdef FEATURE_BMS
        bms::update();
#endif
    }
}
