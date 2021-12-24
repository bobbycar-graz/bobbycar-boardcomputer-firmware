#include "debuginputhandler.h"

// system includes
#include <string_view>

// esp-idf includes
#include <driver/uart.h>
#include <esp_log.h>

// 3rdparty lib includes
#include <tftinstance.h>
#include <esp32-hal-gpio.h>

// local includes
#include "globals.h"
#include "utilities/utils.h"
#include "screens.h"
#include "hardware/buttons.h"

namespace {
constexpr const char * const TAG = "DEBUG";

uint8_t consoleControlCharsReceived{};
bool uart0Initialized{};
} // namespace

void initDebugInput()
{
    if (const auto result = uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); result != ESP_OK)
    {
        ESP_LOGE(TAG, "uart_set_pin() failed with %s", esp_err_to_name(result));
    }

    if (const auto result = uart_driver_install(UART_NUM_0, SOC_UART_FIFO_LEN + 1, 0, 10, nullptr, 0); result != ESP_OK)
        ESP_LOGE(TAG, "uart_driver_install() failed with %s", esp_err_to_name(result));
    else
        uart0Initialized = true;

    pinMode(3, INPUT_PULLUP);
}

void handleDebugInput()
{
    if (!uart0Initialized)
        return;

    size_t length{};
    if (const auto result = uart_get_buffered_data_len(UART_NUM_0, &length); result != ESP_OK)
    {
        ESP_LOGW(TAG, "uart_get_buffered_data_len() failed with %s", esp_err_to_name(result));
    }
    else if (length)
    {
        char data[length];
        length = uart_read_bytes(UART_NUM_0, data, length, 0);

        for (char c : std::string_view{data, length})
        {
            switch (c)
            {
            case 'i':
            case 'I':
                espgui::tft.init();
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
}
