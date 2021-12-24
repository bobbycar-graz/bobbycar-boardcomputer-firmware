#pragma once
/*
 * In this file there will be
 *  - a web-handler to get qr as plain text from http://qr.bobbycar.cloud/files/[OTA_NAME].qr that can be later triggered via in qrimportdisplay.h
 *  - getter and setter that use NVS
 */

// system includes
//#include <string>
#include <string>

// 3rd party includes
#include <esp_err.h>
#include <tl/expected.hpp>

// local includes

namespace qrimport {
    // nvs
    bool has_qr_code(std::string_view key);
    tl::expected<std::string, esp_err_t> get_qr_code(std::string_view key);
    tl::expected<void, esp_err_t> set_qr_code(std::string_view key, std::string_view qrcode);
    tl::expected<void, esp_err_t> delete_qr_code(std::string_view key);

    // web request
    void setup_request();
    tl::expected<void, std::string> start_qr_request();
    tl::expected<std::string, std::string> check_request();
    bool get_request_running();
} // namespace
