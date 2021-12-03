#pragma once
#include "globals.h"
#include "cpputils.h"

// 3rdparty lib includes
#include <asynchttprequest.h>
#include <delayedconstruction.h>

#ifdef FEATURE_OTA
namespace buildserver {
    void buildMenuFromJson(std::string json);
    void buildMenuRequestError(std::string error);

    extern std::string url_for_hashes;
    extern std::string url_for_latest;
    extern std::array<std::string, 10> availableVersions;
    extern bool request_running;
    extern std::string request_failed;
    extern bool parsing_finished;
    extern cpputils::DelayedConstruction<AsyncHttpRequest> request;

    std::string get_ota_url_from_index(uint16_t index);
    uint16_t count_available_buildserver();
    std::string get_hash_url(std::string hash);
    std::string get_latest_url();
    std::string get_descriptor_url(std::string base_url);
    void parse_response_into_variables(std::string response);
    void setup_request();
    void start_descriptor_request(std::string server_base_url);
    void check_descriptor_request();
    bool get_request_running();
}
#endif
