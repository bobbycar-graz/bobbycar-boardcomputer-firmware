#include <cstdio>

#include <Arduino.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <WiFiMulti.h>

#include "bobbycar-protocol/protocol.h"

#include "globals.h"
#include "modes/defaultmode.h"
#include "modes/tempomatmode.h"
#include "screens.h"
#include "dpad.h"
#include "dpadHack.h"
#include "rotary.h"
#include "serialhandler.h"
#include "ota.h"
#include "presets.h"
#include "statistics.h"
#include "actions/bluetoothbeginaction.h"
#include "actions/bluetoothbeginmasteraction.h"
#include "actions/bluetoothconnectbmsaction.h"
#include "bobby_webserver.h"

namespace {
ModeInterface *lastMode{};
unsigned long lastModeUpdate{};
unsigned long lastStatsUpdate{};
unsigned long lastDisplayRedraw{};

constexpr auto modeUpdateRate = 50;
constexpr auto statsUpdateRate = 50;
constexpr auto displayRedrawRate = 50;
}

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println("setup()");
    pinMode(3, INPUT_PULLUP);

    currentlyReverseBeeping = false;

    initScreen();

#ifdef FEATURE_DPAD
  dpad.begin();
#endif

#ifdef FEATURE_3WIRESW
    dpadHack.begin();
#endif

#ifdef FEATURE_ROTARY
    initRotary();
#endif

#ifdef FEATURE_MOSFETS
    pinMode(PINS_MOSFET0, OUTPUT);
    pinMode(PINS_MOSFET1, OUTPUT);
    pinMode(PINS_MOSFET2, OUTPUT);

    digitalWrite(PINS_MOSFET0, LOW);
    digitalWrite(PINS_MOSFET1, LOW);
    digitalWrite(PINS_MOSFET2, LOW);
#endif

    settings = presets::defaultSettings;

    if (settingsSaver.init())
        loadSettings();

#ifdef GLUMP_CONTROLLER
    updateSwapFrontBack();
#endif

    {
        uint8_t macAddress[6];
        WiFi.macAddress(&macAddress[0]);
        #define STRING2(s) #s
        #define STRING(s) STRING2(s)
        std::sprintf(deviceName, STRING(DEVICE_PREFIX) "_%02hhx%02hhx%02hhx", macAddress[3], macAddress[4], macAddress[5]);
    }

    if (!WiFi.setHostname(deviceName))
        Serial.println("Could not setHostname");

    if (!WiFi.softAPsetHostname(deviceName))
        Serial.println("Could not softAPsetHostname");

    if (!WiFi.mode(WIFI_AP_STA))
        Serial.println("Could not set mode to WIFI_AP_STA");

    if (!WiFi.softAP(deviceName, "Passwort_123"))
        Serial.println("Could not softAP");

    if (!WiFi.begin("realraum", "r3alraum"))
        Serial.println("Could not begin WiFi");

    if (settings.bluetoothMode == BluetoothMode::Master)
    {
        BluetoothBeginMasterAction{}.triggered();
#ifdef FEATURE_BMS
        if (settings.autoConnectBms)
            BluetoothConnectBmsAction{}.triggered();
#endif
    } else if (settings.bluetoothMode == BluetoothMode::Slave)
        BluetoothBeginAction{}.triggered();

#ifdef GLUMP_CONTROLLER
    front.serial.get().begin(38400, SERIAL_8N1, PINS_RX1, PINS_TX1);
    back.serial.get().begin(38400, SERIAL_8N1, PINS_RX2, PINS_TX2);
#endif

#ifdef VESC_CONTROLLER
    one.serial.get().begin(57600, SERIAL_8N1, PINS_RX1, PINS_TX1);
    two.serial.get().begin(57600, SERIAL_8N1, PINS_RX2, PINS_TX2);
    Serial.println("vesc begin");
#endif

    raw_gas = 0;
    raw_brems = 0;
    gas = 0;
    brems = 0;

#ifdef GLUMP_CONTROLLER
    for (Controller &controller : controllers())
        controller.command.buzzer = {};
#endif

    currentMode = &modes::defaultMode;

#ifdef FEATURE_OTA
    initOta();
#endif

    initWebserver();

    readPotis();

    if (gas > 200.f || brems > 200.f)
        switchScreen<CalibrateDisplay>(true);
    else
        switchScreen<StatusDisplay>();
}

void loop()
{
    const auto now = millis();

#ifdef FEATURE_DPAD
  updateDpad();
#endif

#ifdef FEATURE_3WIRESW
  updateDpadHack();
#endif

    if (!lastModeUpdate)
        lastModeUpdate = now;
    else if (now - lastModeUpdate >= 1000/modeUpdateRate)
    {
        readPotis();

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

    if (!lastStatsUpdate)
        lastStatsUpdate = now;
    else if (now - lastStatsUpdate >= 1000/statsUpdateRate)
    {
        updateAccumulators();
        pushStats();
        lastStatsUpdate = now;
    }

    updateDisplay();

    if (!lastDisplayRedraw || now - lastDisplayRedraw >= 1000/displayRedrawRate)
    {
        redrawDisplay();

        lastDisplayRedraw = now;
    }

    if (now - performance.lastTime >= 1000)
    {
        performance.last = performance.current;
        performance.current = 0;
        performance.lastTime = now;
    }

#ifdef GLUMP_CONTROLLER
    for (Controller &controller : controllers())
        controller.parser.update();
#endif

#ifdef VESC_CONTROLLER
    for (VescController &controller : controllers())
        controller.update();
#endif

    handleSerial();

#ifdef FEATURE_OTA
    handleOta();
#endif

#ifdef FEATURE_WEBSERVER
    handleWebserver();
#endif

#ifdef FEATURE_BMS
    bms::update();
#endif
}
