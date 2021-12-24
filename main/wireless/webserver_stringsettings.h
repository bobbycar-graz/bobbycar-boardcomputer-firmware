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
#include <lockhelper.h>
#include <tickchrono.h>

// local includes
#include "globals.h"
#include "wireless/webserver_lock.h"

#ifdef FEATURE_WEBSERVER
esp_err_t webserver_stringSettings_handler(httpd_req_t *req);
esp_err_t webserver_saveStringSettings_handler(httpd_req_t *req);
#endif

