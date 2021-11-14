#pragma once

// esp-idf includes
#ifdef FEATURE_WEBSERVER
#include <esp_http_server.h>
#endif
#include <esp_log.h>

// 3rdparty lib includes
#include <htmlbuilder.h>
#include <fmt/core.h>
#include <espcppmacros.h>
#include <esphttpdutils.h>
#include <espchrono.h>
#include <lockhelper.h>
#include <tickchrono.h>
#include <ArduinoJson.h>

// local includes
#include "globals.h"
#include "webserver_lock.h"
#include "settingsutils.h"

#ifdef FEATURE_WEBSERVER
namespace dump_nvs_handler {
esp_err_t webserver_dump_nvs_handler(httpd_req_t *req);
} // namespace

using esphttpdutils::HtmlTag;
using namespace espchrono;

namespace dump_nvs_handler {

template<typename T>
typename std::enable_if<
    !std::is_same<T, bool>::value &&
    !std::is_integral<T>::value &&
    !std::is_same<T, std::array<int8_t, 4>>::value &&
    !std::is_same<T, std::string>::value &&
    !std::is_same<T, espchrono::minutes32>::value &&
    !std::is_same<T, espchrono::DayLightSavingMode>::value &&
    !std::is_same<T, UnifiedModelMode>::value
, bool>::type
showInputForSetting(std::string_view key, T value, JsonObject &body)
{
    body[key] = nullptr;
    return false;
}

template<typename T>
typename std::enable_if<
    std::is_same<T, bool>::value
, bool>::type
showInputForSetting(std::string_view key, T value, JsonObject &body)
{
    body[key] = value;
    return true;
}

template<typename T>
typename std::enable_if<
    !std::is_same<T, bool>::value &&
    std::is_integral<T>::value
, bool>::type
showInputForSetting(std::string_view key, T value, JsonObject &body)
{
    body[key] = value;
    return true;
}

template<typename T>
typename std::enable_if<
    std::is_same<T, std::array<int8_t, 4>>::value
, bool>::type
showInputForSetting(std::string_view key, T value, JsonObject &body)
{
    std::string array_str = fmt::format("{}{}{}{}", value[0], value[1], value[2], value[3]);
    body[key] = array_str;
    return true;
}

template<typename T>
typename std::enable_if<
    std::is_same<T, std::string>::value
, bool>::type
showInputForSetting(std::string_view key, T value, JsonObject &body)
{
    body[key] = value;
    return true;
}

template<typename T>
typename std::enable_if<
    std::is_same<T, espchrono::minutes32>::value
, bool>::type
showInputForSetting(std::string_view key, T value, JsonObject &body)
{
    body[key] = value.count();
    return true;
}

template<typename T>
typename std::enable_if<
    std::is_same<T, espchrono::DayLightSavingMode>::value
, bool>::type
showInputForSetting(std::string_view key, T value, JsonObject &body)
{
    body[key] = toString(espchrono::DayLightSavingMode(value));
    return true;
}

template<typename T>
typename std::enable_if<
    std::is_same<T, UnifiedModelMode>::value
, bool>::type
showInputForSetting(std::string_view key, T value, JsonObject &body)
{
    body[key] = int(value);
    return true;
}


esp_err_t webserver_dump_nvs_handler(httpd_req_t *req)
{
    espcpputils::LockHelper helper{webserver_lock->handle, std::chrono::ceil<espcpputils::ticks>(5s).count()};
    if (!helper.locked())
    {
        constexpr const std::string_view msg = "could not lock webserver_lock";
        ESP_LOGE(TAG, "%.*s", msg.size(), msg.data());
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
    }

    DynamicJsonDocument doc(6144);
    const auto profile = settingsPersister.currentlyOpenProfileIndex();
    const auto switchBackProfile = profile ? int(*profile) : 0;

    JsonObject json_settings = doc.createNestedObject("settings");
    settings.executeForEveryCommonSetting([&](std::string_view key, const auto &value){
        showInputForSetting(key, value, json_settings);
    });

    JsonObject json_stringSettings = doc.createNestedObject("stringSettings");
    stringSettings.executeForEveryCommonSetting([&](std::string_view key, const auto &value){
        showInputForSetting(key, value, json_stringSettings);
    });

    JsonObject profiles = doc.createNestedObject("profiles");

    // Profile settings
    for (uint8_t profile_num = 0; profile_num < 4; profile_num++) {

#ifdef SIMPLIFIED_TRIGGER_TRIGGERONPRESET
        if (profile_num == SIMPLIFIED_TRIGGER_TRIGGERONPRESET) {
            continue;
        }
#endif
        switchProfile(profile_num);

        const auto cur_profile = settingsPersister.currentlyOpenProfileIndex();
        const auto profile_str = cur_profile ? std::to_string(*cur_profile) : "-";

        JsonObject profile = profiles.createNestedObject(profile_str);
        JsonObject profile_stringSettings = profile.createNestedObject("stringSettings");
        JsonObject profile_settings = profile.createNestedObject("settings");

        stringSettings.executeForEveryProfileSetting([&](const char *key, auto &value){
            showInputForSetting(key, value, profile_stringSettings);
        });

        settings.executeForEveryProfileSetting([&](const char *key, auto &value){
            showInputForSetting(key, value, profile_settings);
        });
    }

    switchProfile(switchBackProfile);

    std::string body;
    serializeJson(doc, body);
    CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::Ok, "application/json", body)
}
} // namespace

#endif

