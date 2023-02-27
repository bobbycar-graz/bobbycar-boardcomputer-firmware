#include "potis.h"

// esp-idf includes
#include <esp_log.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>

// 3rdparty lib includes
#include <cpputils.h>

// local includes
#include "globals.h"
#ifdef FEATURE_CAN
#include "can.h"
#endif
#include "newsettings.h"
#include "utils.h"

using namespace std::chrono_literals;

namespace {
constexpr const char * const TAG = "POTIS";

adc_oneshot_unit_handle_t adc1_handle;

constexpr adc_channel_t ADC_CHANNEL_GAS = ADC_CHANNEL_6;
constexpr adc_channel_t ADC_CHANNEL_BREMS = ADC_CHANNEL_7;

//constexpr auto DEFAULT_VREF = 1100;
//constexpr adc_atten_t attenuation = ADC_ATTEN_DB_11;

//constexpr const double ADC_COUNTS = 3300.;

//esp_adc_cal_characteristics_t adc_chars;

} // namespace

void initPotis()
{
    raw_gas = std::nullopt;
    raw_brems = std::nullopt;
    gas = std::nullopt;
    brems = std::nullopt;

    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    if (const auto result = adc_oneshot_new_unit(&init_config1, &adc1_handle); result != ESP_OK)
        ESP_LOGE(TAG, "adc_oneshot_new_unit() failed with %s", esp_err_to_name(result));

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    if (const auto result = adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_GAS, &config); result != ESP_OK)
        ESP_LOGE(TAG, "adc_oneshot_config_channel() failed with %s", esp_err_to_name(result));
    if (const auto result = adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_BREMS, &config); result != ESP_OK)
        ESP_LOGE(TAG, "adc_oneshot_config_channel() failed with %s", esp_err_to_name(result));

    readPotis();
}

void readPotis()
{
//    [[maybe_unused]]
//    constexpr auto sampleMultipleTimes = [](uint8_t pin){
//          analogRead(pin);
//          float sum{};
//          const auto sampleCount = configs.sampleCount.value();
//          for (int i = 0; i < sampleCount; i++)
//              sum += analogRead(pin);
//          return sum / sampleCount;
//    };
#if defined(FEATURE_ADC_IN) || defined(FEATURE_GAMETRAK)
    constexpr auto sampleMultipleTimes = [](adc_channel_t channel){
        int sum{};
        const auto sampleCount = configs.sampleCount.value();
        for (int i = 0; i < sampleCount; i++)
        {
            int value;
            if (const auto result = adc_oneshot_read(adc1_handle, channel, &value); result != ESP_OK)
                ESP_LOGE(TAG, "adc_oneshot_read_channel() failed with %s", esp_err_to_name(result));
            sum += value;
        }
        return sum / sampleCount;
    };
#endif

    raw_gas = std::nullopt;
    raw_brems = std::nullopt;

#ifdef FEATURE_CAN
    const auto now = espchrono::millis_clock::now();

    if (can::can_gas)
    {
        if (now - can::last_can_gas < 100ms)
            raw_gas = *can::can_gas;
        else
            can::can_gas = std::nullopt;
    }

    if (can::can_brems)
    {
        if (now - can::last_can_brems < 100ms)
            raw_brems = *can::can_brems;
        else
            can::can_brems = std::nullopt;
    }
#endif

#ifdef FEATURE_ADC_IN
    if (!raw_gas)
    {
        raw_gas = sampleMultipleTimes(ADC_CHANNEL_GAS);
        ESP_LOGD(TAG, "raw_gas: %d", *raw_gas);
    }
    if (!raw_brems)
    {
        raw_brems = sampleMultipleTimes(ADC_CHANNEL_BREMS);
        ESP_LOGD(TAG, "raw_brems: %d", *raw_brems);
    }
#endif

#ifndef FEATURE_JOYSTICK
    if (raw_gas)
        gas = cpputils::mapValueClamped<float>(*raw_gas, configs.gasMin.value(), configs.gasMax.value(), 0.f, 1000.f);
    else
        gas = std::nullopt;
    if (raw_brems)
        brems = cpputils::mapValueClamped<float>(*raw_brems, configs.bremsMin.value(), configs.bremsMax.value(), 0.f, 1000.f);
    else
        brems = std::nullopt;
#else
    if (raw_gas)
    {
        gas = map_analog_stick(configs.gasMitte.value(), configs.gasMin.value(), configs.gasMax.value(), configs.deadband.value(), *raw_gas);
    }
    else
        gas = std::nullopt;
    if (raw_brems)
    {
        brems = map_analog_stick(configs.bremsMitte.value(), configs.bremsMin.value(), configs.bremsMax.value(), configs.deadband.value(), *raw_brems);
    }
    else
        brems = std::nullopt;
#endif

#ifdef FEATURE_GAMETRAK
    raw_gametrakX = sampleMultipleTimes(PINS_GAMETRAKX);
    gametrakX = cpputils::mapValueClamped<float>(raw_gametrakX, configs.boardcomputerHardware.gametrakXMin.value, configs.boardcomputerHardware.gametrakXMax.value, 0., 1000.);

    raw_gametrakY = sampleMultipleTimes(PINS_GAMETRAKY);
    gametrakY = cpputils::mapValueClamped<float>(raw_gametrakY, configs.boardcomputerHardware.gametrakYMin.value, configs.boardcomputerHardware.gametrakYMax.value, 0., 1000.);

    raw_gametrakDist = sampleMultipleTimes(PINS_GAMETRAKDIST);
    gametrakDist = cpputils::mapValueClamped<float>(raw_gametrakDist, configs.boardcomputerHardware.gametrakDistMin.value, configs.boardcomputerHardware.gametrakDistMax.value, 0., 1000.);
#endif
}
