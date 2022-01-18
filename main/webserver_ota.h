#pragma once

// esp-idf includes
#ifdef FEATURE_WEBSERVER
#include <esp_http_server.h>
#endif
#include <esp_err.h>

#ifdef FEATURE_WEBSERVER
esp_err_t webserver_ota_handler(httpd_req_t *req);
esp_err_t webserver_ota_percentage_handler(httpd_req_t *req);
esp_err_t webserver_trigger_ota_handler(httpd_req_t *req);
#endif
