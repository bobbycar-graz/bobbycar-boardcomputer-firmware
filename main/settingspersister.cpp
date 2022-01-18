#include "settingspersister.h"

// system includes
#include <type_traits>

// esp-idf includes
#include <esp_log.h>
#include <nvs_flash.h>
#include <lwip/apps/snmp.h>
#include <esp_sntp.h>

// 3rdparty lib includes
#include <fmt/core.h>
#include <cpputils.h>
#include <espchrono.h>
#include <futurecpp.h>

// local includes
#include "profilesettings.h"
#ifdef FEATURE_BLUETOOTH
#include "bluetoothmode.h"
#endif
#include "unifiedmodelmode.h"
#include "globals.h"

bool SettingsPersister::init()
{
    if (esp_err_t result = nvs_flash_init();
        cpputils::is_in(result, ESP_ERR_NVS_NO_FREE_PAGES, ESP_ERR_NVS_NEW_VERSION_FOUND))
    {
        ESP_LOGE("BOBBY", "nvs_flash_init() failed with %s, trying to erase...", esp_err_to_name(result));
        return erase();
    }
    else if (result != ESP_OK)
    {
        ESP_LOGE("BOBBY", "nvs_flash_init() failed with %s", esp_err_to_name(result));
        return false;
    }

    return true;
}

bool SettingsPersister::erase()
{
    closeProfile();

    bool result{true};

    if (esp_err_t result = nvs_flash_erase(); result != ESP_OK)
    {
        ESP_LOGE("BOBBY", "nvs_flash_erase() failed with %s", esp_err_to_name(result));
        result = false;
    }

    if (esp_err_t result = nvs_flash_init(); result != ESP_OK)
    {
        ESP_LOGE("BOBBY", "nvs_flash_init() failed with %s", esp_err_to_name(result));
        result = false;
    }

    return result;
}

bool SettingsPersister::openProfile(uint8_t index)
{
    closeProfile();

    nvs_handle handle;
    const auto name = fmt::format("bobbycar{}", index);
    if (esp_err_t result = nvs_open(name.c_str(), NVS_READWRITE, &handle); result != ESP_OK)
    {
        ESP_LOGE("BOBBY", "nvs_open() PROFILE %s failed with %s", name.c_str(), esp_err_to_name(result));
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
template<> struct nvsGetterHelper<std::string> { static esp_err_t nvs_get(nvs_handle handle, const char* key, std::string* out_value)
{
    size_t length;
    if (const esp_err_t result = nvs_get_str(handle, key, nullptr, &length); result != ESP_OK)
        return result;

    char buf[length];
    if (const esp_err_t result = nvs_get_str(handle, key, buf, &length); result != ESP_OK)
        return result;

    *out_value = buf;

    return ESP_OK;
}};
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
template<> struct nvsGetterHelper<espchrono::DayLightSavingMode> { static esp_err_t nvs_get(nvs_handle handle, const char* key, espchrono::DayLightSavingMode* out_value)
{
    uint8_t tempValue;
    esp_err_t err = nvs_get_u8(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = espchrono::DayLightSavingMode(tempValue);
    return err;
}};
template<> struct nvsGetterHelper<espchrono::milliseconds32> { static esp_err_t nvs_get(nvs_handle handle, const char* key, espchrono::milliseconds32* out_value)
{
    int32_t tempValue;
    esp_err_t err = nvs_get_i32(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = espchrono::milliseconds32(tempValue);
    return err;
}};
template<> struct nvsGetterHelper<espchrono::seconds32> { static esp_err_t nvs_get(nvs_handle handle, const char* key, espchrono::seconds32* out_value)
{
    int32_t tempValue;
    esp_err_t err = nvs_get_i32(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = espchrono::seconds32(tempValue);
    return err;
}};
template<> struct nvsGetterHelper<espchrono::minutes32> { static esp_err_t nvs_get(nvs_handle handle, const char* key, espchrono::minutes32* out_value)
{
    int32_t tempValue;
    esp_err_t err = nvs_get_i32(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = espchrono::minutes32(tempValue);
    return err;
}};
template<> struct nvsGetterHelper<espchrono::hours32> { static esp_err_t nvs_get(nvs_handle handle, const char* key, espchrono::hours32* out_value)
{
    int32_t tempValue;
    esp_err_t err = nvs_get_i32(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = espchrono::hours32(tempValue);
    return err;
}};
template<> struct nvsGetterHelper<sntp_sync_mode_t> { static esp_err_t nvs_get(nvs_handle handle, const char* key, sntp_sync_mode_t* out_value)
{
    uint8_t tempValue;
    esp_err_t err = nvs_get_u8(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = sntp_sync_mode_t(tempValue);
    return err;
}};
template<> struct nvsGetterHelper<std::array<int8_t, 4>> { static esp_err_t nvs_get(nvs_handle handle, const char* key, std::array<int8_t, 4>* out_value)
{
    uint32_t tempValue;
    esp_err_t err = nvs_get_u32(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = std::bit_cast<std::array<int8_t, 4>>(tempValue);
    return err;
}};
template<> struct nvsGetterHelper<std::array<uint8_t, 4>> { static esp_err_t nvs_get(nvs_handle handle, const char* key, std::array<uint8_t, 4>* out_value)
{
    uint32_t tempValue;
    esp_err_t err = nvs_get_u32(handle, key, &tempValue);
    if (err == ESP_OK)
        *out_value = std::bit_cast<std::array<uint8_t, 4>>(tempValue);
    return err;
}};

template<typename T>
bool SettingsPersister::load(T &settings)
{
    bool result{true};

    if (m_profile)
    {
        profileSettings.executeForEveryProfileSetting([&](const char *key, auto &value)
        {
            if (esp_err_t result = nvsGetterHelper<std::decay_t<decltype(value)>>::nvs_get(m_profile->handle, key, &value); result != ESP_OK)
            {
                if (result != ESP_ERR_NVS_NOT_FOUND)
                    ESP_LOGE("BOBBY", "nvs_get() PROFILE %s failed with %s", key, esp_err_to_name(result));
                result = false;
            }
        });
    }
    else
    {
        ESP_LOGW("BOBBY", "no profile open currently!");
        result = false;
    }

    return result;
}

template bool SettingsPersister::load<ProfileSettings>(ProfileSettings &profileSettings);

template<typename T> struct nvsSetterHelper;
template<> struct nvsSetterHelper<int8_t> { static constexpr auto nvs_set = &nvs_set_i8; };
template<> struct nvsSetterHelper<uint8_t> { static constexpr auto nvs_set = &nvs_set_u8; };
template<> struct nvsSetterHelper<int16_t> { static constexpr auto nvs_set = &nvs_set_i16; };
template<> struct nvsSetterHelper<uint16_t> { static constexpr auto nvs_set = &nvs_set_u16; };
template<> struct nvsSetterHelper<int32_t> { static constexpr auto nvs_set = &nvs_set_i32; };
template<> struct nvsSetterHelper<uint32_t> { static constexpr auto nvs_set = &nvs_set_u32; };
template<> struct nvsSetterHelper<bool> { static constexpr auto nvs_set = &nvs_set_u8; };
template<> struct nvsSetterHelper<std::string> { static esp_err_t nvs_set(nvs_handle handle, const char* key, const std::string &value)
{
    return nvs_set_str(handle, key, value.c_str());
}};
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
template<> struct nvsSetterHelper<espchrono::DayLightSavingMode> { static esp_err_t nvs_set(nvs_handle handle, const char* key, espchrono::DayLightSavingMode value)
{
    return nvs_set_u8(handle, key, uint8_t(value));
}};
template<> struct nvsSetterHelper<espchrono::milliseconds32> { static esp_err_t nvs_set(nvs_handle handle, const char* key, espchrono::milliseconds32 value)
{
    return nvs_set_i32(handle, key, value.count());
}};
template<> struct nvsSetterHelper<espchrono::seconds32> { static esp_err_t nvs_set(nvs_handle handle, const char* key, espchrono::seconds32 value)
{
    return nvs_set_i32(handle, key, value.count());
}};
template<> struct nvsSetterHelper<espchrono::minutes32> { static esp_err_t nvs_set(nvs_handle handle, const char* key, espchrono::minutes32 value)
{
    return nvs_set_i32(handle, key, value.count());
}};
template<> struct nvsSetterHelper<espchrono::hours32> { static esp_err_t nvs_set(nvs_handle handle, const char* key, espchrono::hours32 value)
{
    return nvs_set_i32(handle, key, value.count());
}};
template<> struct nvsSetterHelper<sntp_sync_mode_t> { static esp_err_t nvs_set(nvs_handle handle, const char* key, sntp_sync_mode_t value)
{
    return nvs_set_u8(handle, key, uint8_t(value));
}};
template<> struct nvsSetterHelper<std::array<int8_t, 4>> { static esp_err_t nvs_set(nvs_handle handle, const char* key, std::array<int8_t, 4> value)
{
    return nvs_set_u32(handle, key, std::bit_cast<uint32_t>(value));
}};
template<> struct nvsSetterHelper<std::array<uint8_t, 4>> { static esp_err_t nvs_set(nvs_handle handle, const char* key, std::array<uint8_t, 4> value)
{
    return nvs_set_u32(handle, key, std::bit_cast<uint32_t>(value));
}};

template<typename T>
bool SettingsPersister::save(T &settings)
{
    bool result{true};

    if (m_profile)
    {
        profileSettings.executeForEveryProfileSetting([&](const char *key, const auto &value)
        {
            if (esp_err_t result = nvsSetterHelper<std::decay_t<decltype(value)>>::nvs_set(m_profile->handle, key, value); result != ESP_OK)
            {
                ESP_LOGE("BOBBY", "nvs_set() PROFILE %s failed with %s", key, esp_err_to_name(result));
                result = false;
            }
        });
    }
    else
    {
        ESP_LOGW("BOBBY", "no profile open currently!");
        result = false;
    }

    return result;
}

template bool SettingsPersister::save<ProfileSettings>(ProfileSettings &settings);

std::optional<uint8_t> SettingsPersister::currentlyOpenProfileIndex() const
{
    if (m_profile)
        return m_profile->profileIndex;

    return std::nullopt;
}
