#pragma once

// esp-idf includes
#include <esp_http_server.h>
#include <esp_err.h>

esp_err_t webserver_root_handler(httpd_req_t *req);
esp_err_t webserver_triggerRawButton_handler(httpd_req_t *req);
esp_err_t webserver_triggerButton_handler(httpd_req_t *req);
esp_err_t webserver_triggerItem_handler(httpd_req_t *req);
esp_err_t webserver_setValue_handler(httpd_req_t *req);
