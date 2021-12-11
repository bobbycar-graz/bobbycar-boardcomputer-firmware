#include "webserver_stringsettings.h"

#ifdef FEATURE_WEBSERVER
using namespace std::chrono_literals;
using esphttpdutils::HtmlTag;

namespace
{
constexpr const char *const TAG = "BOBBYWEB";
} // namespace

esp_err_t webserver_stringSettings_handler(httpd_req_t *req)
{
  espcpputils::LockHelper helper { webserver_lock->handle, std::chrono::ceil<espcpputils::ticks>(5s).count() };
  if (!helper.locked())
  {
    constexpr const std::string_view msg = "could not lock webserver_lock";
    ESP_LOGE(TAG, "%.*s", msg.size(), msg.data());
    CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
  }

  std::string body;

  {
    HtmlTag htmlTag { "html", body };

    {
      HtmlTag headTag { "head", body };

      {
        HtmlTag titleTag { "title", body };
        body += "String Settings";
      }

      body += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\" />";

      HtmlTag styleTag { "style", "type=\"text/css\"", body };
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
      HtmlTag bodyTag { "body", body };

      {
        HtmlTag h1Tag { "h1", body };
        body += "String Settings";
      }

      {
        HtmlTag pTag { "p", body };
        body +=
            "<a href=\"/\">Display control</a> - "
#ifdef FEATURE_OTA
            "<a href=\"/ota\">Update</a> - "
#endif
            "<a href=\"/settings\">Settings</a> - "
            "<b>String Settings</b> - "
            "<a href=\"/dumpnvs\">Dump NVS</a>";
      }

      HtmlTag divTag { "div", "class=\"form-table\"", body };
      stringSettings.executeForEveryCommonSetting([&](std::string_view key, const auto &value)
                                                  {
                HtmlTag formTag{"form", "class=\"form-table-row\" action=\"/saveStringSettings\" method=\"GET\"", body};

                {
                    HtmlTag divTag{"div", "class=\"form-table\"", body};
                    HtmlTag bTag{"b", body};
                    body += esphttpdutils::htmlentities(key);
                }

                {
                    HtmlTag divTag{"div", "class=\"form-table-cell\"", body};
                    body += fmt::format("<input type=\"text\" name=\"{}\" value=\"{}\" />",
                                        esphttpdutils::htmlentities(key),
                                        esphttpdutils::htmlentities(value));
                }

                {
                    HtmlTag divTag{"div", "class=\"form-table-cell\"", body};
                    HtmlTag buttonTag{"button", "type=\"submit\"", body};
                    body += "Save";
                } });
    }
  }

  CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::Ok, "text/html", body)
}

esp_err_t webserver_saveStringSettings_handler(httpd_req_t *req)
{
  espcpputils::LockHelper helper { webserver_lock->handle, std::chrono::ceil<espcpputils::ticks>(5s).count() };
  if (!helper.locked())
  {
    constexpr const std::string_view msg = "could not lock webserver_lock";
    ESP_LOGE(TAG, "%.*s", msg.size(), msg.data());
    CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
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
  bool success { true };

  stringSettings.executeForEveryCommonSetting([&](std::string_view key, auto &value)
                                              {
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

        value = valueBuf;
        body += fmt::format("{}: applied", key);
        body += '\n'; });

  if (body.empty())
    CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::Ok, "text/plain", "nothing changed?!")

  if (settingsPersister.save(stringSettings))
    body += "string settings persisted successfully";
  else
  {
    body += "error while persisting string settings";
    success = false;
  }

  if (success)
  {
    CALL_AND_EXIT_ON_ERROR(httpd_resp_set_hdr, req, "Location", "/stringSettings")
    body += "\nOk, continue at /stringSettings";
  }

  CALL_AND_EXIT(esphttpdutils::webserver_resp_send,
                req,
                success ? esphttpdutils::ResponseStatus::TemporaryRedirect : esphttpdutils::ResponseStatus::BadRequest,
                "text/plain",
                body)
}
#endif
