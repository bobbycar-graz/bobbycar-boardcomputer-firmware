#pragma once

// system includes
#include <atomic>
#include <string_view>

// esp-idf includes
#ifdef FEATURE_WEBSERVER
#include <esp_http_server.h>
#endif
#include <esp_log.h>

// 3rdparty lib includes
#include <espcppmacros.h>
#include <esphttpdutils.h>

// local includes
#include "wireless/webserver_lock.h"
#include "wireless/webserver_displaycontrol.h"
#ifdef FEATURE_OTA
#include "wireless/webserver_ota.h"
#endif
#include "wireless/webserver_settings.h"
#include "wireless/webserver_stringsettings.h"
#ifdef OLD_NVS
#include "wireless/webserver_dumpnvs.h"
#endif

#ifdef FEATURE_WEBSERVER
extern httpd_handle_t httpdHandle;

void initWebserver();
void handleWebserver();
bool MenuDisplayChanged();
esp_err_t webserver_reboot_handler(httpd_req_t *req);
esp_err_t webserver_status_handler(httpd_req_t *req);

namespace bobbywebserver {
extern bool forceRefresh;
extern bool lastScreenWasMenu;
extern int8_t lastSelectIndex;
extern std::vector<std::pair<std::string, const espgui::MenuItemIcon*>> menuBuf;
}
#endif
