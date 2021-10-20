#pragma once

#include <ArduinoJson.h>
#include <cpputils.h>
#include <cleanuphelper.h>

// 3rdparty lib includes
#include <asynchttprequest.h>
#include <delayedconstruction.h>

// local includes
#include "globals.h"
#include "esp_log.h"
#include "fmt/core.h"

// esp-idf
#include "esp_http_client.h"

namespace {
    void buildMenuFromJson(std::string json);
    void buildMenuRequestError(std::string error);
    static std::string url_for_hashes = "";
    static std::string url_for_latest = "";
    static std::array<std::string, 10> availableVersions = {};
    bool request_running = false;
    uint16_t request_failed = false;
    bool parsing_finished = false;
    cpputils::DelayedConstruction<AsyncHttpRequest> request;

    std::string get_ota_url_from_index(uint16_t index)
    {
        if (index < stringSettings.otaServers.size())
        {
            auto otaServer = stringSettings.otaServers[index];
            if (!otaServer.url.empty())
            {
                return otaServer.url;
            }
            else
            {
                ESP_LOGE("BOBBY", "Cannot get OTA url: otaServer.url is empty");
                return "";
            }
        }
        else
        {
            ESP_LOGE("BOBBY", "Cannot get OTA url: Invalid Index");
            return "";
        }
    }

    uint16_t count_available_buildserver()
    {
        uint16_t count = 0;
        for (const auto &otaServer : stringSettings.otaServers) {
            if (!otaServer.url.empty()) count++;
        }
        return count;
    }

    std::string get_hash_url(std::string hash)
    {
        return fmt::format(url_for_hashes, hash);
    }

    std::string get_latest_url()
    {
        return url_for_latest;
    }

    std::string fix_url(std::string url)
    {
        std::string fixed_url = url;
        if (fixed_url.find("http") == std::string::npos)
        {
            fixed_url = fmt::format("http://{}", fixed_url);
        }
        return fixed_url;
    }

    std::string get_descriptor_url(std::string base_url)
    {
        std::string url = fix_url(base_url);
        return fmt::format("{}/otaDescriptor?username={}", url, OTA_USERNAME);
    }

    void parse_response_into_variables(std::string response)
    {
        StaticJsonDocument<512> doc;

        if (const auto error = deserializeJson(doc, response))
        {
            ESP_LOGE("BOBBY", "Error parsing server-response => %s", response.c_str());
            return;
        }

        JsonObject availableVersionsObject = doc["availableVersions"];
        for (JsonPair kv : availableVersionsObject)
        {
            static auto index = 0;
            auto hash = kv.key().c_str();
            if (index > availableVersions.size())
            {
                break;
            }
            availableVersions.at(index) = hash;
            index++;
        }

        url_for_latest = fix_url(fmt::format("{}{}", stringSettings.otaServerUrl, doc["latest"].as<std::string>()));
        url_for_hashes = fix_url(fmt::format("{}{}", stringSettings.otaServerUrl, doc["url"].as<std::string>()));
        parsing_finished = true;
    }

    void setup_request()
    {
        if (!request.constructed())
            request.construct("ota-descriptor-request", espcpputils::CoreAffinity::Core0);
    }

    void start_descriptor_request(std::string server_base_url)
    {
        if (!request.constructed())
        {
            ESP_LOGW("BOBBY", "request is im oarsch");
            return;
        }

        const auto url = get_descriptor_url(server_base_url);
        ESP_LOGD("BOBBY", "requesting data...");
        if (const auto result = request->start(url); !result)
        {
            ESP_LOGW("BOBBY", "request start failed");
            return;
        }
        request_running = true;
        request_failed = false;
        url_for_latest.clear();
        url_for_hashes.clear();
        availableVersions = {};
        parsing_finished = false;
    }

    void check_descriptor_request()
    {
        if (!request.constructed())
        {
            ESP_LOGW("BOBBY", "request is im oarsch");
            request_running = false;
            request_failed = true;
            return;
        }

        if (!request->finished())
        {
            // ESP_LOGW("BOBBY", "Request has not finished yet.");
            return;
        }

        const auto helper = cpputils::makeCleanupHelper([](){ request->clearFinished(); });
        const std::string content = std::move(request->takeBuffer());

        if (const auto result = request->result(); !result)
        {
            ESP_LOGW("BOBBY", "request failed: %.*s", result.error().size(), result.error().data());
            std::string failed_str = result.error().data();
            auto statuscode = failed_str.substr(failed_str.length() - 3);
            request_running = false;
            request_failed = std::stoi(statuscode);
            return;
        }

        const auto result = request->result();
        ESP_LOGW("BOBBY", "Request finished: %s", content.c_str());
        parse_response_into_variables(content);
        request_running = false;
        request_failed = false;
    }

    bool get_request_running()
    {
        return request_running;
    }
}
