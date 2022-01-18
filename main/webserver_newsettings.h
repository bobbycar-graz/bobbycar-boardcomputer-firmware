#pragma once

// esp-idf includes
#include <esp_http_server.h>
#include <esp_err.h>

esp_err_t webserver_newSettings_handler(httpd_req_t *req);
esp_err_t webserver_saveNewSettings_handler(httpd_req_t *req);
esp_err_t webserver_resetNewSettings_handler(httpd_req_t *req);
