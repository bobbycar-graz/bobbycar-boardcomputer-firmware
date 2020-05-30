#pragma once

#include <type_traits>

#include <HardwareSerial.h>
#include <nvs_flash.h>
#include <nvs.h>

#include "settings.h"
#include "bluetoothmode.h"

namespace {
class SettingsSaver
{
public:
    bool init();
    bool load(Settings &settings);
    bool save(Settings &settings);

private:
    nvs_handle my_handle;
};

bool SettingsSaver::init()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        Serial.printf("nvs_flash_init() returned: %s, trying to erase\r\n", esp_err_to_name(err));
        err = nvs_flash_erase();
        if (err != ESP_OK)
        {
            Serial.printf("nvs_flash_erase() returned: %s, aborting\r\n", esp_err_to_name(err));
            return false;
        }

        err = nvs_flash_init();
    }

    if (err != ESP_OK)
    {
        Serial.printf("nvs_flash_init() returned: %s\r\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_open("bobbycar", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        Serial.printf("nvs_open() returned: %s\r\n", esp_err_to_name(err));
        return false;
    }

    return true;
}

template<typename T> struct nvsGetterHelper;
template<> struct nvsGetterHelper<int8_t> { static constexpr auto nvs_get = &nvs_get_i8; };
template<> struct nvsGetterHelper<uint8_t> { static constexpr auto nvs_get = &nvs_get_u8; };
template<> struct nvsGetterHelper<int16_t> { static constexpr auto nvs_get = &nvs_get_i16; };
template<> struct nvsGetterHelper<uint16_t> { static constexpr auto nvs_get = &nvs_get_u16; };
template<> struct nvsGetterHelper<int32_t> { static constexpr auto nvs_get = &nvs_get_i32; };
template<> struct nvsGetterHelper<uint32_t> { static constexpr auto nvs_get = &nvs_get_u32; };
template<> struct nvsGetterHelper<bool> { static esp_err_t nvs_get(nvs_handle handle, const char* key, bool* out_value)
    {
        uint8_t tempValue;
        esp_err_t err = nvs_get_u8(handle, key, &tempValue);
        if (err == ESP_OK)
            *out_value = tempValue;
        return err;
    }};
#ifdef GLUMP_CONTROLLER
template<> struct nvsGetterHelper<ControlType> { static esp_err_t nvs_get(nvs_handle handle, const char* key, ControlType* out_value)
    {
        uint8_t tempValue;
        esp_err_t err = nvs_get_u8(handle, key, &tempValue);
        if (err == ESP_OK)
            *out_value = ControlType(tempValue);
        return err;
    }};
template<> struct nvsGetterHelper<ControlMode> { static esp_err_t nvs_get(nvs_handle handle, const char* key, ControlMode* out_value)
    {
        uint8_t tempValue;
        esp_err_t err = nvs_get_u8(handle, key, &tempValue);
        if (err == ESP_OK)
            *out_value = ControlMode(tempValue);
        return err;
    }};
template<> struct nvsGetterHelper<LarsmModeMode> { static esp_err_t nvs_get(nvs_handle handle, const char* key, LarsmModeMode* out_value)
    {
        uint8_t tempValue;
        esp_err_t err = nvs_get_u8(handle, key, &tempValue);
        if (err == ESP_OK)
            *out_value = LarsmModeMode(tempValue);
        return err;
    }};
#endif
template<> struct nvsGetterHelper<BluetoothMode> { static esp_err_t nvs_get(nvs_handle handle, const char* key, BluetoothMode* out_value)
    {
        uint8_t tempValue;
        esp_err_t err = nvs_get_u8(handle, key, &tempValue);
        if (err == ESP_OK)
            *out_value = BluetoothMode(tempValue);
        return err;
    }};

bool SettingsSaver::load(Settings &settings)
{
    bool result{true};

    settings.executeForEverySetting([&](const char *key, auto &value)
    {
        esp_err_t err = nvsGetterHelper<std::remove_reference_t<decltype(value)>>::nvs_get(my_handle, key, &value);
        if (err != ESP_OK)
        {
            Serial.printf("nvs_get_i32() for %s returned: %s\r\n", key, esp_err_to_name(err));
            result = false;
        }
    });

    return result;
}

template<typename T> struct nvsSetterHelper;
template<> struct nvsSetterHelper<int8_t> { static constexpr auto nvs_set = &nvs_set_i8; };
template<> struct nvsSetterHelper<uint8_t> { static constexpr auto nvs_set = &nvs_set_u8; };
template<> struct nvsSetterHelper<int16_t> { static constexpr auto nvs_set = &nvs_set_i16; };
template<> struct nvsSetterHelper<uint16_t> { static constexpr auto nvs_set = &nvs_set_u16; };
template<> struct nvsSetterHelper<int32_t> { static constexpr auto nvs_set = &nvs_set_i32; };
template<> struct nvsSetterHelper<uint32_t> { static constexpr auto nvs_set = &nvs_set_u32; };
template<> struct nvsSetterHelper<bool> { static constexpr auto nvs_set = &nvs_set_u8; };
#ifdef GLUMP_CONTROLLER
template<> struct nvsSetterHelper<ControlType> { static esp_err_t nvs_set(nvs_handle handle, const char* key, ControlType value)
    {
        return nvs_set_u8(handle, key, uint8_t(value));
    }};
template<> struct nvsSetterHelper<ControlMode> { static esp_err_t nvs_set(nvs_handle handle, const char* key, ControlMode value)
    {
        return nvs_set_u8(handle, key, uint8_t(value));
    }};
template<> struct nvsSetterHelper<LarsmModeMode> { static esp_err_t nvs_set(nvs_handle handle, const char* key, LarsmModeMode value)
    {
        return nvs_set_u8(handle, key, uint8_t(value));
    }};
#endif
template<> struct nvsSetterHelper<BluetoothMode> { static esp_err_t nvs_set(nvs_handle handle, const char* key, BluetoothMode value)
    {
        return nvs_set_u8(handle, key, uint8_t(value));
    }};

bool SettingsSaver::save(Settings &settings)
{
    bool result{true};

    settings.executeForEverySetting([&](const char *key, auto value)
    {
        esp_err_t err = nvsSetterHelper<decltype(value)>::nvs_set(my_handle, key, value);
        if (err != ESP_OK)
        {
            Serial.printf("nvs_get_i32() for %s returned: %s\r\n", key, esp_err_to_name(err));
            result = false;
        }
        return err == ESP_OK;
    });

    return result;
}
}
