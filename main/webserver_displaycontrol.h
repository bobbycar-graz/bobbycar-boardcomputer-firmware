#pragma once

// esp-idf includes
#ifdef FEATURE_WEBSERVER
#include <esp_http_server.h>
#endif
#include <esp_log.h>

// 3rdparty lib includes
#include <changevaluedisplay.h>
#include <espcppmacros.h>
#include <esphttpdutils.h>
#include <fmt/core.h>
#include <htmlbuilder.h>
#include <lockhelper.h>
#include <menudisplay.h>
#include <numberparsing.h>
#include <screenmanager.h>
#include <textinterface.h>
#include <tickchrono.h>

// local includes
#include "buttons.h"
#include "globals.h"
#include "webserver_lock.h"

#ifdef FEATURE_WEBSERVER
esp_err_t webserver_root_handler(httpd_req_t *req);
esp_err_t webserver_triggerButton_handler(httpd_req_t *req);
esp_err_t webserver_triggerItem_handler(httpd_req_t *req);
esp_err_t webserver_setValue_handler(httpd_req_t *req);
#endif
