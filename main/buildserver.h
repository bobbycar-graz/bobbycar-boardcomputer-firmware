#pragma once

#include "globals.h"
#include "esp_log.h"
#include "fmt/core.h"

// esp-idf
#include "esp_http_client.h"

/*
 * ToDo:
 *  - get_ota_decriptor_json_string => returns std::string, takes std::string url
 */
namespace {

std::string ota_descriptor_json = "";

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

void get_ota_descriptor(std::string url)
{
    auto descriptorUrl = fmt::format("{}/otaDescriptor", url);
    // Make GET request to descriptorUrl and store json somewhere to be decoded when needed, for example in ota_descriptor_json (buildserver.h:16)
}
}
