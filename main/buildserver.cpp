#include "buildserver.h"

// esp-idf
#include "esp_http_client.h"
#include "esp_log.h"

// 3rdparty lib includes
#include <asynchttprequest.h>
#include <delayedconstruction.h>
#include <ArduinoJson.h>
#include <cpputils.h>
#include <cleanuphelper.h>
#include "fmt/core.h"

// local includes
#include "globals.h"
#include "newsettings.h"

namespace buildserver {

    uint16_t count_available_buildserver()
    {
        uint16_t count = 0;
        for (const auto &otaServer : configs.otaServers) {
            if (!otaServer.url.value.empty()) count++;
        }
        return count;
    }

    namespace SelectBranch {
    cpputils::DelayedConstruction<AsyncHttpRequest> request;
    bool request_running{false};
    bool constructedMenu{false};
    std::string request_failed{};
    std::vector<std::string> branches{};

    void setup_request()
    {
        if (!request.constructed())
        {
            request.construct("ota-descriptor-request", espcpputils::CoreAffinity::Core0);
        }
    }

    void start_descriptor_request(std::string server_base_url)
    {
        if (!request.constructed())
        {
            ESP_LOGW("BOBBY", "request is im oarsch");
            return;
        }

        const auto url = fmt::format("{}/otaDescriptor?username={}&branches", server_base_url, configs.otaUsername.value);
        ESP_LOGD("BOBBY", "requesting data...");
        if (const auto result = request->start(url); !result)
        {
            ESP_LOGW("BOBBY", "request start failed");
            return;
        }
        request_running = true;
        constructedMenu = false;
    }


    void check_descriptor_request()
    {
        if (!request.constructed())
        {
            ESP_LOGW("BOBBY", "request is im oarsch");
            request_running = false;
            request_failed = "request is im oarsch";
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
            request_failed = result.error();
            return;
        }

        const auto result = request->result();
        ESP_LOGW("BOBBY", "Request finished: %s", content.c_str());
        parse_response(content);
        request_running = false;
        request_failed = {};
    }

    void parse_response(std::string response)
    {
        StaticJsonDocument<1024> doc;

        if (const auto error = deserializeJson(doc, response))
        {
            ESP_LOGE("BOBBY", "Error parsing server-response => %s (%s)", error.c_str(), response.c_str());
            return;
        }

        JsonArray arr = doc.as<JsonArray>();
        branches.resize(arr.size());

        for(JsonVariant v : arr) {
            branches.push_back(v);
        }
    }

    bool get_request_running()
    {
        return request_running;
    }
    } // namespace SelectBranch

    namespace SelectBuild {
    void buildMenuFromJson(std::string json);
    void buildMenuRequestError(std::string error);

    std::string url_for_hashes{};
    std::string url_for_latest{};
    std::array<std::string, 10> availableVersions{};
    bool request_running{false};
    std::string request_failed{};
    bool parsing_finished{false};
    cpputils::DelayedConstruction<AsyncHttpRequest> request;

    std::string get_ota_url_from_index(uint16_t index)
    {
        if (index < configs.otaServers.size())
        {
            const auto &otaServer = configs.otaServers[index];
            if (!otaServer.url.value.empty())
            {
                return otaServer.url.value;
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

    std::string get_hash_url(std::string hash)
    {
        return fmt::format(url_for_hashes, hash);
    }

    std::string get_latest_url()
    {
        return url_for_latest;
    }

    std::string get_descriptor_url(std::string base_url)
    {
        if (configs.otaServerBranch.value.empty())
            return fmt::format("{}/otaDescriptor?username={}", base_url, configs.otaUsername.value);
        else
            return fmt::format("{}/otaDescriptor?username={}&branch={}", base_url, configs.otaUsername.value, configs.otaServerBranch.value);
    }

    void parse_response_into_variables(std::string response)
    {
        StaticJsonDocument<1024> doc;

        if (const auto error = deserializeJson(doc, response))
        {
            ESP_LOGE("BOBBY", "Error parsing server-response => %s (%s)", error.c_str(), response.c_str());
            return;
        }

        JsonObject availableVersionsObject = doc["availableVersions"];
        static auto index = 0;
        for (JsonPair kv : availableVersionsObject)
        {
            auto hash = kv.key().c_str();
            if (index > availableVersions.size())
            {
                break;
            }
            availableVersions.at(index) = hash;
            index++;
        }

        index = 0;

        url_for_latest = fmt::format("{}{}", configs.otaServerUrl.value, doc["latest"].as<std::string>());
        url_for_hashes = fmt::format("{}{}", configs.otaServerUrl.value, doc["url"].as<std::string>());
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
        request_failed = {};
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
            request_failed = "request is im oarsch";
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
            request_failed = result.error();
            return;
        }

        const auto result = request->result();
        ESP_LOGW("BOBBY", "Request finished: %s", content.c_str());
        parse_response_into_variables(content);
        request_running = false;
        request_failed = {};
    }

    bool get_request_running()
    {
        return request_running;
    }
    } // namespace SelectBuild
} // namespace buildserver
