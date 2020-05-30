#pragma once

#include <HardwareSerial.h>
#include <WiFi.h>

#include "globals.h"
#include "utils.h"
#include "screens.h"

namespace {
wl_status_t last_status;
IPAddress last_ip;

void handleSerial()
{
    const auto status = WiFi.status();
    if (last_status != status)
    {
        Serial.print("Status changed to: ");
        Serial.println(toString(status));
        last_status = status;
    }

    const auto ip = WiFi.localIP();
    if (last_ip != ip)
    {
        Serial.print("IP changed to: ");
        Serial.println(ip.toString());
        last_ip = ip;
    }

    while(Serial.available())
    {
        const auto c = Serial.read();

        switch (c)
        {
        case 'i':
        case 'I':
            tft.init();
            break;
#ifdef GLUMP_CONTROLLER
        case 'p':
        case 'P':
        {
            const auto firstPower = front.command.poweroff;
            for (Controller &controller : controllers())
                controller.command.poweroff = !firstPower;
            break;
        }
        case 'l':
        case 'L':
        {
            const auto firstLed = front.command.led;
            for (Controller &controller : controllers())
                controller.command.led = !firstLed;
            break;
        }
#endif
        case 'r':
        case 'R':
            loadSettings();
            break;
        case 's':
        case 'S':
            saveSettings();
            break;
#ifdef GLUMP_CONTROLLER
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            for (Controller &controller : controllers())
                controller.command.buzzer.freq = c-'0';
            break;
#endif
        case 'A':
            InputDispatcher::rotate(-1);
            break;
        case 'B':
            InputDispatcher::rotate(1);
            break;
        case 'C':
            InputDispatcher::confirmButton(true);
            InputDispatcher::confirmButton(false);
            break;
        case 'D':
            InputDispatcher::backButton(true);
            InputDispatcher::backButton(false);
            break;
        }
    }
}
}
