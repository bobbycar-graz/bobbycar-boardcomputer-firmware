#include "webserver.h"
#include "sdkconfig.h"

// system includes
#include <chrono>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <menuitem.h>
#include <espcppmacros.h>
#include <esphttpdutils.h>
#include <display.h>
#include <screenmanager.h>
#include <menudisplay.h>
#include <lockhelper.h>
#include <tickchrono.h>

// local includes
#include "webserver_lock.h"
#include "webserver_displaycontrol.h"
#ifdef FEATURE_OTA
#include "webserver_ota.h"
#endif
#include "webserver_settings.h"
#include "webserver_newsettings.h"
#include "webserver_dumpnvs.h"
#include "globals.h"
#include "newsettings.h"

using namespace std::chrono_literals;

#ifdef FEATURE_WEBSERVER
namespace {
constexpr const char * const TAG = "BOBBYWEB";

//bool forceRefresh{false};
bool lastScreenWasMenu{};
int8_t lastSelectIndex{};
std::vector<std::pair<std::string, const espgui::MenuItemIcon*>> menuBuf{};

esp_err_t webserver_reboot_handler(httpd_req_t *req);
bool menuDisplayChanged();
esp_err_t webserver_status_handler(httpd_req_t *req);
} // namespace bobbywebserver

httpd_handle_t httpdHandle;

void initWebserver()
{
    if (!configs.feature.webserver_disable_lock.value)
    {
        webserver_lock.construct();
        webserver_lock->take(portMAX_DELAY);
    }

    {
        httpd_config_t httpConfig HTTPD_DEFAULT_CONFIG();
        httpConfig.core_id = 1;
        httpConfig.max_uri_handlers = 16;
        httpConfig.stack_size = 8192;

        const auto result = httpd_start(&httpdHandle, &httpConfig);
        ESP_LOG_LEVEL_LOCAL((result == ESP_OK ? ESP_LOG_INFO : ESP_LOG_ERROR), TAG, "httpd_start(): %s", esp_err_to_name(result));
        if (result != ESP_OK)
            return;
    }

    for (const httpd_uri_t &uri : {
             httpd_uri_t { .uri = "/",                   .method = HTTP_GET, .handler = webserver_root_handler,               .user_ctx = NULL },
             httpd_uri_t { .uri = "/triggerRawButton",   .method = HTTP_GET, .handler = webserver_triggerRawButton_handler,   .user_ctx = NULL },
             httpd_uri_t { .uri = "/triggerButton",      .method = HTTP_GET, .handler = webserver_triggerButton_handler,      .user_ctx = NULL },
             httpd_uri_t { .uri = "/triggerItem",        .method = HTTP_GET, .handler = webserver_triggerItem_handler,        .user_ctx = NULL },
             httpd_uri_t { .uri = "/setValue",           .method = HTTP_GET, .handler = webserver_setValue_handler,           .user_ctx = NULL },
             httpd_uri_t { .uri = "/reboot",             .method = HTTP_GET, .handler = webserver_reboot_handler,             .user_ctx = NULL },
#ifdef FEATURE_OTA
             httpd_uri_t { .uri = "/ota",                .method = HTTP_GET, .handler = webserver_ota_handler,                .user_ctx = NULL },
             httpd_uri_t { .uri = "/otaPercent",         .method = HTTP_GET, .handler = webserver_ota_percentage_handler,     .user_ctx = NULL },
             httpd_uri_t { .uri = "/triggerOta",         .method = HTTP_GET, .handler = webserver_trigger_ota_handler,        .user_ctx = NULL },
#endif
             httpd_uri_t { .uri = "/settings",           .method = HTTP_GET, .handler = webserver_settings_handler,           .user_ctx = NULL },
             httpd_uri_t { .uri = "/saveSettings",       .method = HTTP_GET, .handler = webserver_saveSettings_handler,       .user_ctx = NULL },
             httpd_uri_t { .uri = "/newSettings",        .method = HTTP_GET, .handler = webserver_newSettings_handler,        .user_ctx = NULL },
             httpd_uri_t { .uri = "/saveNewSettings",    .method = HTTP_GET, .handler = webserver_saveNewSettings_handler,    .user_ctx = NULL },
             httpd_uri_t { .uri = "/resetNewSettings",   .method = HTTP_GET, .handler = webserver_resetNewSettings_handler,   .user_ctx = NULL },
             httpd_uri_t { .uri = "/dumpnvs",            .method = HTTP_GET, .handler = webserver_dump_nvs_handler,           .user_ctx = NULL },
             httpd_uri_t { .uri = "/check",              .method = HTTP_GET, .handler = webserver_status_handler,             .user_ctx = NULL },
         })
    {
        const auto result = httpd_register_uri_handler(httpdHandle, &uri);
        ESP_LOG_LEVEL_LOCAL((result == ESP_OK ? ESP_LOG_INFO : ESP_LOG_ERROR), TAG, "httpd_register_uri_handler() for %s: %s", uri.uri, esp_err_to_name(result));
        //if (result != ESP_OK)
        //    return result;
    }
}

void handleWebserver()
{
    if (!configs.feature.webserver_disable_lock.value)
    {
        webserver_lock->give();
        vTaskDelay(1);
        webserver_lock->take(portMAX_DELAY);
    }
}

namespace {

esp_err_t webserver_reboot_handler(httpd_req_t *req)
{
    esp_restart();

    CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::Ok, "text/plain", "REBOOT called...")
}

bool menuDisplayChanged()
{
    if (auto currentDisplay = static_cast<const espgui::Display *>(espgui::currentDisplay.get()))
    {
        lastScreenWasMenu = true;
        if (const auto *menuDisplay = currentDisplay->asMenuDisplay())
        {
            if (menuBuf.size() != menuDisplay->menuItemCount())
            {
                menuBuf.resize(menuDisplay->menuItemCount());
                auto iterator = std::begin(menuBuf);
                menuDisplay->runForEveryMenuItem([&,selectedIndex=menuDisplay->selectedIndex()](const espgui::MenuItem &menuItem){
                    *(iterator++) = std::make_pair(menuItem.text(), menuItem.icon());
                });
                lastSelectIndex = menuDisplay->selectedIndex();
                return true;
            }
            bool _return{false};
            auto iterator = std::begin(menuBuf);
            menuDisplay->runForEveryMenuItem([&,selectedIndex=menuDisplay->selectedIndex()](const espgui::MenuItem &menuItem){
                if (menuItem.text() != iterator->first || menuItem.icon() != iterator->second)
                {
                    *iterator = std::make_pair(menuItem.text(), menuItem.icon());
                    _return = true;
                }
                iterator++;
            });

            if (menuDisplay->selectedIndex() != lastSelectIndex)
                _return = true;
            lastSelectIndex = menuDisplay->selectedIndex();
            return _return;
        }
        return false;
    }
    else
    {
        if (lastScreenWasMenu)
        {
            lastScreenWasMenu = false;
            menuBuf.clear();
            return true;
        }
        return false;
    }
}

esp_err_t webserver_status_handler(httpd_req_t *req)
{
    if (!configs.feature.webserver_disable_lock.value)
    {
        espcpputils::LockHelper helper{webserver_lock->handle, std::chrono::ceil<espcpputils::ticks>(5s).count()};
        if (!helper.locked())
        {
            constexpr const std::string_view msg = "could not lock webserver_lock";
            ESP_LOGE(TAG, "%.*s", msg.size(), msg.data());
            CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
        }
    }

    CALL_AND_EXIT_ON_ERROR(httpd_resp_set_hdr, req, "Access-Control-Allow-Origin", "http://web.bobbycar.cloud");

    std::string wants_json_query;
    if (auto result = esphttpdutils::webserver_get_query(req))
        wants_json_query = *result;
    else
    {
        ESP_LOGE(TAG, "%.*s", result.error().size(), result.error().data());

        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", result.error());
    }

    char tmpBuf[256];
    const auto key_result = httpd_query_key_value(wants_json_query.data(), "json", tmpBuf, 256);
    if (key_result == ESP_OK && (configs.webserverPassword.value.empty() || configs.webserverPassword.value == tmpBuf))
    {
        if (!menuDisplayChanged())
        {

            CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::Ok, "text/plain", "Ok.");
        }
        else
        {
            return webserver_root_handler(req);
        }
    }
    else
    {

        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::Unauthorized, "text/plain", "");
    }
}

} // namespace
#endif
