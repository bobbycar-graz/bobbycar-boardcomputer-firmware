#include "webserver_settings.h"

// system includes
#include <limits>

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
#include <numberparsing.h>
#include <lockhelper.h>
#include <tickchrono.h>

// local includes
#include "globals.h"
#include "webserver_lock.h"

#ifdef FEATURE_WEBSERVER
using namespace std::chrono_literals;
using esphttpdutils::HtmlTag;

namespace {
constexpr const char * const TAG = "BOBBYWEB";

template<typename T>
typename std::enable_if<
    !std::is_same<T, bool>::value &&
        !std::is_integral<T>::value &&
        !std::is_same<T, std::array<int8_t, 4>>::value
    , bool>::type
showInputForSetting(std::string_view key, T value, std::string &body)
{
    HtmlTag spanTag{"span", "style=\"color: red;\"", body};
    body += "Unsupported config type";
    return false;
}

template<typename T>
typename std::enable_if<
    std::is_same<T, bool>::value
    , bool>::type
showInputForSetting(std::string_view key, T value, std::string &body)
{
    body += fmt::format("<input type=\"checkbox\" name=\"{}\" value=\"true\" {}/>"
                        "<input type=\"hidden\" name=\"{}\" value=\"false\" />",
                        esphttpdutils::htmlentities(key),
                        value ? "checked " : "",
                        esphttpdutils::htmlentities(key));
    return true;
}

template<typename T>
typename std::enable_if<
    !std::is_same<T, bool>::value &&
        std::is_integral<T>::value
    , bool>::type
showInputForSetting(std::string_view key, T value, std::string &body)
{
    body += fmt::format("<input type=\"number\" name=\"{}\" value=\"{}\" min=\"{}\" max=\"{}\" step=\"1\" />",
                        esphttpdutils::htmlentities(key),
                        value,
                        std::numeric_limits<T>::min(),
                        std::numeric_limits<T>::max());
    return true;
}

template<typename T>
typename std::enable_if<
    std::is_same<T, std::array<int8_t, 4>>::value
    , bool>::type
showInputForSetting(std::string_view key, T value, std::string &body)
{
    body += fmt::format("<input type=\"text\" name=\"{}\" value=\"{}{}{}{}\" pattern=\"[0-9]{{4}}\" />",
                        esphttpdutils::htmlentities(key),
                        value[0],
                        value[1],
                        value[2],
                        value[3]);
    return true;
}
} // namespace

esp_err_t webserver_settings_handler(httpd_req_t *req)
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
    std::string body;

    {
        HtmlTag htmlTag{"html", body};

        {
            HtmlTag headTag{"head", body};

            {
                HtmlTag titleTag{"title", body};
                body += "Settings";
            }

            body += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\" />";

            HtmlTag styleTag{"style", "type=\"text/css\"", body};
            body +=
                ".form-table {"
                    "display: table;"
                    "border-collapse: separate;"
                    "border-spacing: 10px 0;"
                "}"

                ".form-table .form-table-row {"
                    "display: table-row;"
                "}"

                ".form-table .form-table-row .form-table-cell {"
                    "display: table-cell;"
                "}";
        }

        {
            HtmlTag bodyTag{"body", body};

            {
                HtmlTag h1Tag{"h1", body};
                body += "Settings";
            }

            {
                HtmlTag pTag{"p", body};
                body += "<a href=\"/\">Display control</a> - "
                        "<a href=\"/ota\">Update</a> - "
                        "<b>Settings</b> - "
                        "<a href=\"/newSettings\">New Settings</a> - "
                        "<a href=\"/dumpnvs\">Dump NVS</a>";
            }

            HtmlTag divTag{"div", "class=\"form-table\"", body};
            profileSettings.executeForEveryProfileSetting([&](std::string_view key, const auto &value){
                HtmlTag formTag{"form", "class=\"form-table-row\" action=\"/saveSettings\" method=\"GET\"", body};

                {
                    HtmlTag divTag{"div", "class=\"form-table\"", body};
                    HtmlTag bTag{"b", body};
                    body += esphttpdutils::htmlentities(key);
                }

                {
                    HtmlTag divTag{"div", "class=\"form-table-cell\"", body};
                    showInputForSetting(key, value, body);
                }

                {
                    HtmlTag divTag{"div", "class=\"form-table-cell\"", body};
                    HtmlTag buttonTag{"button", "type=\"submit\"", body};
                    body += "Save";
                }
            });
        }
    }

    CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::Ok, "text/html", body)
}

namespace {
template<typename T>
typename std::enable_if<
    !std::is_same<T, bool>::value &&
        !std::is_integral<T>::value &&
        !std::is_same<T, std::array<int8_t, 4>>::value
    , bool>::type
saveSetting(T &value, std::string_view newValue, std::string &body)
{
    body += "Unsupported config type";
    return false;
}

template<typename T>
typename std::enable_if<
    std::is_same<T, bool>::value
    , bool>::type
saveSetting(T &value, std::string_view newValue, std::string &body)
{
    if (newValue == "true")
    {
        value = true;
        body += "applied";
        return true;
    }
    else if (newValue == "false")
    {
        value = false;
        body += "applied";
        return true;
    }
    else
    {
        body += fmt::format("only true and false allowed, not {}", newValue);
        return false;
    }
}

template<typename T>
typename std::enable_if<
    !std::is_same<T, bool>::value &&
        std::is_integral<T>::value
    , bool>::type
saveSetting(T &value, std::string_view newValue, std::string &body)
{
    if (auto parsed = cpputils::fromString<T>(newValue))
    {
        value = *parsed;
        body += "applied";
        return true;
    }
    else
    {
        body += fmt::format("could not parse {}", newValue);
        return false;
    }
}

template<typename T>
typename std::enable_if<
    std::is_same<T, std::array<int8_t, 4>>::value
    , bool>::type
saveSetting(T &value, std::string_view newValue, std::string &body)
{
    if (std::array<int8_t, 4> parsed; std::sscanf(newValue.data(), "%1hhi%1hhi%1hhi%1hhi", &parsed[0], &parsed[1], &parsed[2], &parsed[3]) == 4)
    {
        value = parsed;
        body += "applied";
        return true;
    }
    else
    {
        body += fmt::format("could not parse {}", newValue);
        return false;
    }
}
} // namespace

esp_err_t webserver_saveSettings_handler(httpd_req_t *req)
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

    std::string query;
    if (auto result = esphttpdutils::webserver_get_query(req))
        query = *result;
    else
    {
        ESP_LOGE(TAG, "%.*s", result.error().size(), result.error().data());
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", result.error());
    }

    std::string body;
    bool success{true};

    profileSettings.executeForEveryProfileSetting([&](std::string_view key, auto &value){
        char valueBufEncoded[256];
        if (const auto result = httpd_query_key_value(query.data(), key.data(), valueBufEncoded, 256); result != ESP_OK)
        {
            if (result != ESP_ERR_NOT_FOUND)
            {
                const auto msg = fmt::format("{}: httpd_query_key_value() failed with {}", key, esp_err_to_name(result));
                ESP_LOGE(TAG, "%.*s", msg.size(), msg.data());
                body += msg;
                body += '\n';
                success = false;
            }
            return;
        }

        char valueBuf[257];
        esphttpdutils::urldecode(valueBuf, valueBufEncoded);

        body += key;
        if (!saveSetting(value, valueBuf, body))
            success = false;
        body += '\n';
    });

    if (body.empty())
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::Ok, "text/plain", "nothing changed?!")

    if (settingsPersister.save(profileSettings))
        body += "settings persisted successfully";
    else
    {
        body += "error while persisting settings";
        success = false;
    }

    if (success)
    {
        CALL_AND_EXIT_ON_ERROR(httpd_resp_set_hdr, req, "Location", "/settings")
        body += "\nOk, continue at /settings";
    }

    CALL_AND_EXIT(esphttpdutils::webserver_resp_send,
                  req,
                  success ? esphttpdutils::ResponseStatus::TemporaryRedirect : esphttpdutils::ResponseStatus::BadRequest,
                  "text/plain",
                  body)
}
#endif
