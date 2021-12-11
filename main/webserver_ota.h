#pragma once

// esp-idf includes
#ifdef FEATURE_WEBSERVER
#include <esp_http_server.h>
#endif
#include <esp_log.h>
#include <esp_ota_ops.h>

// 3rdparty lib includes
#include <espcppmacros.h>
#include <esphttpdutils.h>
#include <espstrutils.h>
#include <fmt/core.h>
#include <htmlbuilder.h>
#include <lockhelper.h>
#include <tickchrono.h>

// local includes
#ifdef FEATURE_OTA
#include "ota.h"
#endif
#include "webserver_lock.h"

#if defined(FEATURE_WEBSERVER) && defined(FEATURE_OTA)
esp_err_t webserver_ota_handler(httpd_req_t *req);
esp_err_t webserver_ota_percentage_handler(httpd_req_t *req);
esp_err_t webserver_trigger_ota_handler(httpd_req_t *req);
#endif
