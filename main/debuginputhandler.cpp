#include "debuginputhandler.h"

// system includes
#include <string_view>

// esp-idf includes
#include <driver/uart.h>
#include <esp_log.h>

// 3rdparty lib includes
#include <tftinstance.h>
#include <esp32-hal-gpio.h>
#include <screenmanager.h>

// local includes
#include "globals.h"
#include "utils.h"
#include "bobbybuttons.h"

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
            if (consoleControlCharsReceived < 2)
            {
                switch (c)
                {
                case '\x1b':
                    if (consoleControlCharsReceived == 0)
                        consoleControlCharsReceived = 1;
                    else
                        consoleControlCharsReceived = 0;
                    break;
                case '\x5b':
                    if (consoleControlCharsReceived == 1)
                        consoleControlCharsReceived = 2;
                    else
                        consoleControlCharsReceived = 0;
                    break;
                case 'i':
                case 'I':
                    consoleControlCharsReceived = 0;
                    espgui::tft.init();
                    break;
                case 'p':
                case 'P':
                {
                    consoleControlCharsReceived = 0;
                    const auto firstPower = controllers.front.command.poweroff;
                    for (Controller &controller : controllers)
                        controller.command.poweroff = !firstPower;
                    break;
                }
                case 'l':
                case 'L':
                {
                    consoleControlCharsReceived = 0;
                    const auto firstLed = controllers.front.command.led;
                    for (Controller &controller : controllers)
                        controller.command.led = !firstLed;
                    break;
                }
                case 'r':
                case 'R':
                    consoleControlCharsReceived = 0;
                    loadSettings();
                    break;
                case 's':
                case 'S':
                    consoleControlCharsReceived = 0;
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
                    consoleControlCharsReceived = 0;
                    for (Controller &controller : controllers)
                        controller.command.buzzer.freq = c-'0';
                    break;
                case 'z':
                case 'Z':
                    consoleControlCharsReceived = 0;
                    if (espgui::currentDisplay)
                    {
                        espgui::currentDisplay->buttonPressed(espgui::Button(BobbyButton::Left2));
                        espgui::currentDisplay->buttonReleased(espgui::Button(BobbyButton::Left2));
                    }
                    break;
                case 'u':
                case 'U':
                    consoleControlCharsReceived = 0;
                    if (espgui::currentDisplay)
                    {
                        espgui::currentDisplay->buttonPressed(espgui::Button(BobbyButton::Right2));
                        espgui::currentDisplay->buttonReleased(espgui::Button(BobbyButton::Right2));
                    }
                    break;
                default:
                    consoleControlCharsReceived = 0;
                }
            }
            else
            {
                consoleControlCharsReceived = 0;
                switch (c)
                {
                case 'A': // Up arrow pressed
                    if (espgui::currentDisplay)
                    {
                        espgui::currentDisplay->buttonPressed(espgui::Button::Up);
                        espgui::currentDisplay->buttonReleased(espgui::Button::Up);
                    }
                    break;
                case 'B': // Down arrow pressed
                    if (espgui::currentDisplay)
                    {
                        espgui::currentDisplay->buttonPressed(espgui::Button::Down);
                        espgui::currentDisplay->buttonReleased(espgui::Button::Down);
                    }
                    break;
                case 'C': // Right arrow pressed
                    if (espgui::currentDisplay)
                    {
                        espgui::currentDisplay->buttonPressed(espgui::Button::Right);
                        espgui::currentDisplay->buttonReleased(espgui::Button::Right);
                    }
                    break;
                case 'D': // Left arrow pressed
                    if (espgui::currentDisplay)
                    {
                        espgui::currentDisplay->buttonPressed(espgui::Button::Left);
                        espgui::currentDisplay->buttonReleased(espgui::Button::Left);
                    }
                    break;
                default:
                    ESP_LOGI(TAG, "unknown control char received: %hhx", c);
                }
            }
        }
    }
}
