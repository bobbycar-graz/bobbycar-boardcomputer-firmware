#include "debuginputhandler.h"

// Arduino includes
#include <HardwareSerial.h>

// 3rdparty lib includes
#include <tftinstance.h>

// local includes
#include "globals.h"
#include "utils.h"
#include "screens.h"
#include "buttons.h"

using namespace espgui;

//wl_status_t last_status;
//IPAddress last_ip;

void initDebugInput()
{
    Serial.begin(115200);
    //Serial.setDebugOutput(true);
}

void handleDebugInput()
{
    //const auto status = WiFi.status();
    //if (last_status != status)
    //{
        //Serial.print("Status changed to: ");
        //Serial.println(to_string(status).c_str());
        //last_status = status;
    //}

    //const auto ip = WiFi.localIP();
    //if (last_ip != ip)
    //{
        //Serial.print("IP changed to: ");
        //Serial.println(to_string(ip).c_str());
        //last_ip = ip;
    //}

    while(Serial.available())
    {
        const auto c = Serial.read();

        switch (c)
        {
        case 'i':
        case 'I':
            tft.init();
            break;
        case 'p':
        case 'P':
        {
            const auto firstPower = controllers.front.command.poweroff;
            for (Controller &controller : controllers)
                controller.command.poweroff = !firstPower;
            break;
        }
        case 'l':
        case 'L':
        {
            const auto firstLed = controllers.front.command.led;
            for (Controller &controller : controllers)
                controller.command.led = !firstLed;
            break;
        }
        case 'r':
        case 'R':
            loadSettings();
            break;
        case 's':
        case 'S':
            saveSettings();
            break;
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
            for (Controller &controller : controllers)
                controller.command.buzzer.freq = c-'0';
            break;
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
        case 'z':
        case 'Z':
#ifndef LEDSTRIP_WRONG_DIRECTION
            InputDispatcher::blinkLeftButton(true);
            InputDispatcher::blinkLeftButton(false);
#else
            InputDispatcher::blinkRightButton(true);
            InputDispatcher::blinkRightButton(false);
#endif
            break;
        case 'u':
        case 'U':
#ifndef LEDSTRIP_WRONG_DIRECTION
            InputDispatcher::blinkRightButton(true);
            InputDispatcher::blinkRightButton(false);
#else
            InputDispatcher::blinkLeftButton(true);
            InputDispatcher::blinkLeftButton(false);
#endif
            break;
        }
    }
}
