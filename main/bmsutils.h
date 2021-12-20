#pragma once

#include <espchrono.h>

#include "globals.h"

namespace {
#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
namespace bms {
constexpr auto autoReconnect = false; // causes hangs when not available

bool lastConnected;
espchrono::millis_clock::time_point lastSend;
espchrono::millis_clock::time_point lastReceive;

float voltage;
float current;
float capacity;
int8_t soc;
float cycle;
int16_t power;
float batt[12];

bool send6Bit(int zhen_tou, int address, int data)
{
    uint8_t buffer[6];
    buffer[0] = (char)((zhen_tou>>8)&255);
    buffer[1] = (char)(zhen_tou&255);
    buffer[2] = (char)(address&255);
    buffer[3] = (char)((data>>8)&255);
    buffer[4] = (char)(data&255);
    buffer[5] = (char)(buffer[2]+buffer[3]+buffer[4]);

    const auto sent = bluetoothSerial.write(buffer, 6);
    if (sent != 6)
    {
        //Serial.printf("send: %u\r\n", sent);
    }
    return sent == 6;
}

void init()
{

}

void update()
{
    const auto now = espchrono::millis_clock::now();

    if (bluetoothSerial.hasClient() != lastConnected)
    {
        lastSend = 0;
        lastConnected = bluetoothSerial.hasClient();
    }

    if (bluetoothSerial.hasClient())
    {
        if (bluetoothSerial.available() >= 140)
        {
            uint8_t buffer[140];
            const auto read = bluetoothSerial.readBytes(buffer, 140);
            if (read != 140)
            {
                //Serial.printf("bms read buffer too short %u\r\n", read);

                for (int i = 0; i < read; i++)
                {
                    //Serial.printf("%i  ", buffer[i]);
                }
                //Serial.println();

                goto after_read;
            }

            if (buffer[0] != 170 ||
                buffer[1] != 85 ||
                buffer[2] != 170 ||
                buffer[3] != 255)
            {
                //Serial.println("bms read buffer wrong sequence");

                for (const auto &x : buffer)
                {
                    //Serial.printf("%i  ", x);
                }
                //Serial.println();

                goto after_read;
            }

            lastReceive = now;

            voltage = static_cast<int16_t>((buffer[4] << 8) | buffer[5]) / 10.f;
            current = static_cast<int16_t>((buffer[72] << 8) | buffer[73]) / 10.f;
            capacity = static_cast<int32_t>((((((buffer[79] << 8) | buffer[80]) << 8) | buffer[81]) << 8) | buffer[82]) / 1000.f;
            soc = buffer[74];
            cycle = static_cast<int32_t>((((((buffer[83] << 8) | buffer[84]) << 8) | buffer[85]) << 8) | buffer[86]) / 1000.f;
            power = (buffer[113] << 8) | buffer[114];

            for (int i = 0; i < 12; i++)
                batt[i] = ((buffer[(i*2)+6] * 256) + buffer[(i*2)+7]) / 1000.f;
        }
//        else if (bluetoothSerial.available())
//        {
//            //Serial.printf("available() = %i\r\n", bluetoothSerial.available());
//            while (bluetoothSerial.available())
//            {
//                //Serial.printf("%i  ", bluetoothSerial.read());
//            }
//            //Serial.println();
//        }

        after_read:

        if (!lastSend || now-lastSend >= 1000)
        {
            //Serial.println("requresting bms live values");
            if (!send6Bit(56283, 0, 0))
            {
                //Serial.println("could not request bms live values");
            }
            lastSend = now;
        }
    }
    else if (autoReconnect)
    {
        if (!lastSend || now-lastSend >= 500)
        {
            uint8_t remoteAddress[ESP_BD_ADDR_LEN] = {0xAA, 0xBB, 0xCC, 0xA1, 0x23, 0x45};
            //Serial.println("connect()");
            const auto result = bluetoothSerial.connect(remoteAddress);
            //Serial.printf("connect() returned %s\r\n", result?"true":"false");

            lastSend = now;
        }
    }
}
}
#endif
}
