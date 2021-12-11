#pragma once

// esp-idf includes
#ifdef FEATURE_WEBSERVER
#include <esp_http_server.h>
#endif
#include <esp_log.h>

// 3rdparty lib includes
#include <ArduinoJson.h>
#include <espchrono.h>
#include <espcppmacros.h>
#include <esphttpdutils.h>
#include <fmt/core.h>
#include <htmlbuilder.h>
#include <lockhelper.h>
#include <tickchrono.h>

// local includes
#include "globals.h"
#include "settingsutils.h"
#include "webserver_lock.h"

#ifdef FEATURE_WEBSERVER
esp_err_t webserver_dump_nvs_handler(httpd_req_t *req);
#endif
