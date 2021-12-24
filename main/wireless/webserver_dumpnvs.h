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
#include <espchrono.h>
#include <lockhelper.h>
#include <tickchrono.h>
#include <ArduinoJson.h>

// local includes
#include "globals.h"
#include "wireless/webserver_lock.h"
#include "settingsutils.h"

#ifdef FEATURE_WEBSERVER
esp_err_t webserver_dump_nvs_handler(httpd_req_t *req);
#endif

