#pragma once

// system includes
#include <atomic>
#include <string_view>
#include <string>
#include <iterator>

// esp-idf includes
#ifdef FEATURE_WEBSERVER
#ifdef FEATURE_SSL
#include <esp_https_server.h>
#else
#include <esp_http_server.h>
#endif
#endif
#include <esp_log.h>

// 3rdparty lib includes
#include <espcppmacros.h>
#include <esphttpdutils.h>

// local includes
#include "webserver_lock.h"
#include "webserver_displaycontrol.h"
#ifdef FEATURE_OTA
#include "webserver_ota.h"
#endif
#include "webserver_settings.h"
#include "webserver_stringsettings.h"
#ifdef OLD_NVS
#include "webserver_dumpnvs.h"
#endif

#ifdef FEATURE_WEBSERVER
extern httpd_handle_t httpdHandle;

void initWebserver();
void handleWebserver();
bool MenuDisplayChanged();
esp_err_t webserver_reboot_handler(httpd_req_t *req);
esp_err_t webserver_status_handler(httpd_req_t *req);

extern const char cert_pem_start[] asm("_binary_cert_pem_start");
extern const char cert_pem_end[] asm("_binary_cert_pem_end");
extern const char key_pem_start[] asm("_binary_key_pem_start");
extern const char key_pem_end[] asm("_binary_key_pem_end");

namespace bobbywebserver {
extern bool forceRefresh;
extern bool lastScreenWasMenu;
extern int8_t lastSelectIndex;
extern std::vector<std::pair<std::string, const espgui::MenuItemIcon*>> menuBuf;

// Certs
const std::string_view cert_pem{cert_pem_start, size_t(std::distance(cert_pem_start, cert_pem_end))};
const std::string_view key_pem{key_pem_start, size_t(std::distance(key_pem_start, key_pem_end))};

}
#endif
