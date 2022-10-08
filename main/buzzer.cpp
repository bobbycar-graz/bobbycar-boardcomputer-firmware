/* LEDC (LED Controller) basic example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

// esp-idf includes
#include <esp_log.h>
#include "driver/ledc.h"
#include "esp_err.h"

#include "buzzer.h"

namespace buzzer
{
constexpr const char TAG[] = "BUZZER";

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (0) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (4095) // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
#define LEDC_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz

void init()
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    if (auto result = ledc_timer_config(&ledc_timer) != ESP_OK) {
        ESP_LOGW(TAG, "ledc_timer_config() failed with %s", esp_err_to_name(result));
        return;
    }

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = LEDC_OUTPUT_IO,
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    if (auto result = ledc_channel_config(&ledc_channel) != ESP_OK) {
        ESP_LOGW(TAG, "ledc_channel_config() failed with %s", esp_err_to_name(result));
        return;
    }
}

void set_frequency(int frequency)
{
    if (frequency != 0)
    {
        ledc_set_freq(LEDC_MODE, LEDC_TIMER, frequency);
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    }
    else
    {
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    }
}
}
