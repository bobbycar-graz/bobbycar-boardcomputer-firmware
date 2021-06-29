#pragma once

#include <type_traits>
#include <optional>

#include <nvs_flash.h>
#include <nvs.h>

#include "settings.h"
#ifdef FEATURE_BLUETOOTH
#include "bluetoothmode.h"
#endif
#include "unifiedmodelmode.h"

namespace {
class SettingsPersister
{
public:
    bool init();
    bool erase();
    bool openProfile(uint8_t index);
    void closeProfile();
    bool load(Settings &settings);
    bool save(Settings &settings);

    std::optional<uint8_t> currentlyOpenProfileIndex() const;

private:
    struct CurrentlyOpenProfile {
        nvs_handle handle;
        uint8_t profileIndex;
    };
    std::optional<CurrentlyOpenProfile> m_profile;
};

bool SettingsPersister::init()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        //Serial.printf("nvs_flash_init() returned: %s, trying to erase\r\n", esp_err_to_name(err));

        return erase();
    }

    if (err != ESP_OK)
    {
        //Serial.printf("nvs_flash_init() returned: %s\r\n", esp_err_to_name(err));
        return false;
    }

    return true;
}

bool SettingsPersister::erase()
{
    esp_err_t err = nvs_flash_erase();
    if (err != ESP_OK)
    {
        //Serial.printf("nvs_flash_erase() returned: %s, aborting\r\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_flash_init();
    if (err != ESP_OK)
    {
        //Serial.printf("nvs_flash_init() returned: %s\r\n", esp_err_to_name(err));
        return false;
    }

    return true;
}

bool SettingsPersister::openProfile(uint8_t index)
{
    closeProfile();

    nvs_handle handle;
    esp_err_t err = nvs_open(("bobbycar"+std::to_string(index)).c_str(), NVS_READWRITE, &handle);
    if (err != ESP_OK)
    {
        //Serial.printf("nvs_open() returned: %s\r\n", esp_err_to_name(err));
        return false;
    }

    m_profile = {handle, index};

    return true;
}

void SettingsPersister::closeProfile()
{
    if (!m_profile)
        return;

    nvs_close(m_profile->handle);

    m_profile = std::nullopt;
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
template<> struct nvsGetterHelper<bobbycar::protocol::ControlType> { static esp_err_t nvs_get(nvs_handle handle, const char* key, bobbycar::protocol::ControlType* out_value)
{
    uint8_t tempValue;
    esp_err_t err = nvs_get_u8(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = bobbycar::protocol::ControlType(tempValue);
    return err;
}};
template<> struct nvsGetterHelper<bobbycar::protocol::ControlMode> { static esp_err_t nvs_get(nvs_handle handle, const char* key, bobbycar::protocol::ControlMode* out_value)
{
    uint8_t tempValue;
    esp_err_t err = nvs_get_u8(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = bobbycar::protocol::ControlMode(tempValue);
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
#ifdef FEATURE_BLUETOOTH
template<> struct nvsGetterHelper<BluetoothMode> { static esp_err_t nvs_get(nvs_handle handle, const char* key, BluetoothMode* out_value)
{
    uint8_t tempValue;
    esp_err_t err = nvs_get_u8(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = BluetoothMode(tempValue);
    return err;
}};
#endif
template<> struct nvsGetterHelper<UnifiedModelMode> { static esp_err_t nvs_get(nvs_handle handle, const char* key, UnifiedModelMode* out_value)
{
    uint8_t tempValue;
    esp_err_t err = nvs_get_u8(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = UnifiedModelMode(tempValue);
    return err;
}};
template<> struct nvsGetterHelper<wifi_mode_t> { static esp_err_t nvs_get(nvs_handle handle, const char* key, wifi_mode_t* out_value)
{
    uint8_t tempValue;
    esp_err_t err = nvs_get_u8(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = wifi_mode_t(tempValue);
    return err;
}};

bool SettingsPersister::load(Settings &settings)
{
    if (!m_profile)
    {
        //Serial.println("SettingsPersister::load() no profile open currently!");
        return false;
    }

    bool result{true};

    settings.executeForEverySetting([&](const char *key, auto &value)
    {
        esp_err_t err = nvsGetterHelper<std::remove_reference_t<decltype(value)>>::nvs_get(m_profile->handle, key, &value);
        if (err != ESP_OK)
        {
            //Serial.printf("nvs_get_i32() for %s returned: %s\r\n", key, esp_err_to_name(err));
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
template<> struct nvsSetterHelper<bobbycar::protocol::ControlType> { static esp_err_t nvs_set(nvs_handle handle, const char* key, bobbycar::protocol::ControlType value)
{
    return nvs_set_u8(handle, key, uint8_t(value));
}};
template<> struct nvsSetterHelper<bobbycar::protocol::ControlMode> { static esp_err_t nvs_set(nvs_handle handle, const char* key, bobbycar::protocol::ControlMode value)
{
    return nvs_set_u8(handle, key, uint8_t(value));
}};
template<> struct nvsSetterHelper<LarsmModeMode> { static esp_err_t nvs_set(nvs_handle handle, const char* key, LarsmModeMode value)
{
    return nvs_set_u8(handle, key, uint8_t(value));
}};
#ifdef FEATURE_BLUETOOTH
template<> struct nvsSetterHelper<BluetoothMode> { static esp_err_t nvs_set(nvs_handle handle, const char* key, BluetoothMode value)
{
    return nvs_set_u8(handle, key, uint8_t(value));
}};
#endif
template<> struct nvsSetterHelper<UnifiedModelMode> { static esp_err_t nvs_set(nvs_handle handle, const char* key, UnifiedModelMode value)
{
    return nvs_set_u8(handle, key, uint8_t(value));
}};
template<> struct nvsSetterHelper<wifi_mode_t> { static esp_err_t nvs_set(nvs_handle handle, const char* key, wifi_mode_t value)
{
    return nvs_set_u8(handle, key, uint8_t(value));
}};

bool SettingsPersister::save(Settings &settings)
{
    if (!m_profile)
    {
        //Serial.println("SettingsPersister::save() no profile open currently!");
        return false;
    }

    bool result{true};

    settings.executeForEverySetting([&](const char *key, auto value)
    {
        esp_err_t err = nvsSetterHelper<decltype(value)>::nvs_set(m_profile->handle, key, value);
        if (err != ESP_OK)
        {
            //Serial.printf("nvs_get_i32() for %s returned: %s\r\n", key, esp_err_to_name(err));
            result = false;
        }
    });

    return result;
}

std::optional<uint8_t> SettingsPersister::currentlyOpenProfileIndex() const
{
    if (m_profile)
        return m_profile->profileIndex;

    return std::nullopt;
}
}
