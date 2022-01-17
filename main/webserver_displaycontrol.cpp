#include "webserver_displaycontrol.h"

// esp-idf includes
#ifdef FEATURE_WEBSERVER
#include <esp_http_server.h>
#endif
#include <esp_log.h>

// 3rdparty lib includes
#include <htmlbuilder.h>
#include <fmt/core.h>
#include <espcppmacros.h>
#include <numberparsing.h>
#include <esphttpdutils.h>
#include <textinterface.h>
#include <menudisplay.h>
#include <changevaluedisplay.h>
#include <changevaluedisplay_string.h>
#include <lockhelper.h>
#include <tickchrono.h>
#include <screenmanager.h>
#include <futurecpp.h>

// local includes
#include "bobbybuttons.h"
#include "webserver_lock.h"
#include "newsettings.h"

#ifdef FEATURE_WEBSERVER
using esphttpdutils::HtmlTag;
using namespace std::chrono_literals;

namespace {
constexpr const char * const TAG = "BOBBYWEB";
} // namespace

esp_err_t webserver_root_handler(httpd_req_t *req)
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

        body += "{";
        if (auto currentDisplay = static_cast<const espgui::Display *>(espgui::currentDisplay.get()))
        {
            body.reserve(4096);
            if (const auto *textInterface = currentDisplay->asTextInterface())
            {
                body += fmt::format("\"name\":\"{}\",", textInterface->text());
            }

            if (const auto *menuDisplay = currentDisplay->asMenuDisplay())
            {
                body += fmt::format("\"index\":{},\"items\":[", menuDisplay->selectedIndex());
                menuDisplay->runForEveryMenuItem([&,selectedIndex=menuDisplay->selectedIndex()](const espgui::MenuItem &menuItem){
                    body += "{";
                    const auto itemName = menuItem.text();
                    std::string color{};
                    std::string font{};
                    switch (menuItem.color()) {
                    case TFT_RED:
                        color = "&1";
                        break;
                    case TFT_GREEN:
                        color = "&2";
                        break;
                    case TFT_BLUE:
                        color = "&3";
                        break;
                    case TFT_YELLOW:
                        color = "&4";
                        break;
                    case TFT_BLACK:
                        color = "&5";
                        break;
                    case TFT_WHITE:
                        color = "&6";
                        break;
                    case TFT_GREY:
                    case TFT_DARKGREY:
                        color = "&7";
                        break;
                    default:
                        color = "";
                        break;
                    }

                    switch (menuItem.font())
                    {
                    case 2:
                        font = "&s";
                        break;
                    case 4:
                        font = "&m";
                        break;
                    default:
                        font = "";
                        break;
                    }

                    std::string menuItemName = font + color + itemName;
                    body += fmt::format("\"name\":\"{}\",\"icon\":\"{}\",\"index\":{}", menuItemName, (menuItem.icon()) ? menuItem.icon()->name : "", selectedIndex);
                    body += "},";
                });
                body += "],";
            }
            else if (const auto *changeValueDisplay = currentDisplay->asChangeValueDisplayInterface())
            {
                body += fmt::format("\"value\":\"{}\",", changeValueDisplay->shownValue());
            }
            else
            {
                body += "\"err\":\"Screen not implemented yet.\"";
            }
        }
        else
        {
            body += "\"err\":\"Currently no screen instantiated.\"";
        }
        body += "}";

        size_t lastGesch = body.rfind("},");
        if (std::string::npos != lastGesch)
            body = body.erase(lastGesch+1, 1);

        size_t lastEckig = body.rfind("],");
        if (std::string::npos != lastEckig)
            body = body.erase(lastEckig+1, 1);

    }
    else if (key_result != ESP_ERR_NOT_FOUND && tmpBuf != configs.webserverPassword.value)
    {
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::Unauthorized, "text/plain", "");
    }
    else
    {
        HtmlTag htmlTag{"html", body};

        {
            HtmlTag headTag{"head", body};

            {
                HtmlTag titleTag{"title", body};
                body += "Display control";
            }

            body += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\" />";
        }

        {
            HtmlTag bodyTag{"body", body};

            {
                HtmlTag h1Tag{"h1", body};
                body += "Display control";
            }

            {
                HtmlTag pTag{"p", body};
                body += "<b>Display control</b> - "
#ifdef FEATURE_OTA
                        "<a href=\"/ota\">Update</a> - "
#endif

                        "<a href=\"/settings\">Settings</a> - "
                        "<a href=\"/newSettings\">New Settings</a> - "
                        "<a href=\"/dumpnvs\">Dump NVS</a>";
            }

            {
                HtmlTag pTag{"p", body};
                body += "Trigger raw button: "
                        "<a href=\"/triggerRawButton?button=0\">Button0</a> "
                        "<a href=\"/triggerRawButton?button=1\">Button1</a> "
                        "<a href=\"/triggerRawButton?button=2\">Button2</a> "
                        "<a href=\"/triggerRawButton?button=3\">Button3</a> "
                        "<a href=\"/triggerRawButton?button=4\">Button4</a> "
                        "<a href=\"/triggerRawButton?button=5\">Button5</a> "
                        "<a href=\"/triggerRawButton?button=6\">Button6</a> "
                        "<a href=\"/triggerRawButton?button=7\">Button7</a> "
                        "<a href=\"/triggerRawButton?button=8\">Button8</a> "
                        "<a href=\"/triggerRawButton?button=9\">Button9</a> "
                        "<a href=\"/triggerRawButton?button=10\">Button10</a> "
                        "<a href=\"/triggerRawButton?button=11\">Button11</a>";
            }

            {
                HtmlTag pTag{"p", body};
                body += fmt::format("Trigger button: "
                                    "<a href=\"/triggerButton?button={}\">Left</a> "
                                    "<a href=\"/triggerButton?button={}\">Right</a> "
                                    "<a href=\"/triggerButton?button={}\">Up</a> "
                                    "<a href=\"/triggerButton?button={}\">Down</a> "
                                    "<a href=\"/triggerButton?button={}\">Profile0</a> "
                                    "<a href=\"/triggerButton?button={}\">Profile1</a> "
                                    "<a href=\"/triggerButton?button={}\">Profile2</a> "
                                    "<a href=\"/triggerButton?button={}\">Profile3</a> "
                                    "<a href=\"/triggerButton?button={}\">Left2</a> "
                                    "<a href=\"/triggerButton?button={}\">Right2</a> "
                                    "<a href=\"/triggerButton?button={}\">Up2</a> "
                                    "<a href=\"/triggerButton?button={}\">Down2</a>",
                                    std::to_underlying(espgui::Button::Left),
                                    std::to_underlying(espgui::Button::Right),
                                    std::to_underlying(espgui::Button::Up),
                                    std::to_underlying(espgui::Button::Down),
                                    std::to_underlying(BobbyButton::Profile0),
                                    std::to_underlying(BobbyButton::Profile1),
                                    std::to_underlying(BobbyButton::Profile2),
                                    std::to_underlying(BobbyButton::Profile3),
                                    std::to_underlying(BobbyButton::Left2),
                                    std::to_underlying(BobbyButton::Right2),
                                    std::to_underlying(BobbyButton::Up2),
                                    std::to_underlying(BobbyButton::Down2));
            }

            if (auto currentDisplay = static_cast<const espgui::Display *>(espgui::currentDisplay.get()))
            {
                if (const auto *textInterface = currentDisplay->asTextInterface())
                {
                    HtmlTag h2Tag{"h2", body};
                    body += esphttpdutils::htmlentities(textInterface->text());
                }

                if (const auto *menuDisplay = currentDisplay->asMenuDisplay())
                {
                    HtmlTag ulTag{"ul", body};

                    int i{0};
                    menuDisplay->runForEveryMenuItem([&,selectedIndex=menuDisplay->selectedIndex()](const espgui::MenuItem &menuItem){
                        HtmlTag liTag = i == selectedIndex ?
                                            HtmlTag{"li", "style=\"border: 1px solid black;\"", body} :
                                            HtmlTag{"li", body};

                        body += fmt::format("<a href=\"/triggerItem?index={}\">{}</a>", i, esphttpdutils::htmlentities(menuItem.text()));
                        i++;
                    });
                }
                else if (const auto *changeValueDisplay = currentDisplay->asChangeValueDisplayInterface())
                {
                    HtmlTag formTag{"form", "action=\"/setValue\" method=\"GET\"", body};
                    body += fmt::format("<input type=\"number\" name=\"value\" value=\"{}\" />", changeValueDisplay->shownValue());
                    body += "<button type=\"submit\">Update</button>";
                }
                else if (const auto *changeValueDisplay = currentDisplay->asChangeValueDisplayString())
                {
                    HtmlTag formTag{"form", "action=\"/setValue\" method=\"GET\"", body};
                    body += fmt::format("<input type=\"text\" name=\"value\" value=\"{}\" />", changeValueDisplay->shownValue());
                    body += "<button type=\"submit\">Update</button>";
                }
                else
                {
                    body += "No web control implemented for current display.";
                }
            }
            else
            {
                body += "Currently no screen instantiated.";
            }
        }
    }

    CALL_AND_EXIT_ON_ERROR(httpd_resp_set_hdr, req, "Access-Control-Allow-Origin", "http://web.bobbycar.cloud");

    CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::Ok, (key_result == ESP_OK) ? "application/json":"text/html", body)
}

esp_err_t webserver_triggerRawButton_handler(httpd_req_t *req)
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

    uint8_t button;
    constexpr const std::string_view buttonParamName{"button"};

    {
        char valueBufEncoded[256];
        if (const auto result = httpd_query_key_value(query.data(), buttonParamName.data(), valueBufEncoded, 256); result != ESP_OK)
        {
            if (result == ESP_ERR_NOT_FOUND)
            {
                const auto msg = fmt::format("{} not set", buttonParamName);
                ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
                CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
            }
            else
            {
                const auto msg = fmt::format("httpd_query_key_value() {} failed with {}", buttonParamName, esp_err_to_name(result));
                ESP_LOGE(TAG, "%.*s", msg.size(), msg.data());
                CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
            }
        }

        char valueBuf[257];
        esphttpdutils::urldecode(valueBuf, valueBufEncoded);

        std::string_view value{valueBuf};

        if (auto parsed = cpputils::fromString<decltype(button)>(value))
        {
            button = *parsed;
        }
        else
        {
            const auto msg = fmt::format("could not parse {} {}", buttonParamName, value);
            ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
            CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
        }
    }

    if (!espgui::currentDisplay)
    {
        constexpr const std::string_view msg = "espgui::currentDisplay is null";
        ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
    }

    espgui::currentDisplay->rawButtonPressed(button);
    espgui::currentDisplay->rawButtonReleased(button);

    CALL_AND_EXIT_ON_ERROR(httpd_resp_set_hdr, req, "Location", "/")
    CALL_AND_EXIT_ON_ERROR(httpd_resp_set_hdr, req, "Access-Control-Allow-Origin", "http://web.bobbycar.cloud");
    CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::TemporaryRedirect, "text/html", "Ok, continue at <a href=\"/\">/</a>")
}

esp_err_t webserver_triggerButton_handler(httpd_req_t *req)
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

    espgui::Button button;
    constexpr const std::string_view buttonParamName{"button"};

    {
        char valueBufEncoded[256];
        if (const auto result = httpd_query_key_value(query.data(), buttonParamName.data(), valueBufEncoded, 256); result != ESP_OK)
        {
            if (result == ESP_ERR_NOT_FOUND)
            {
                const auto msg = fmt::format("{} not set", buttonParamName);
                ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
                CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
            }
            else
            {
                const auto msg = fmt::format("httpd_query_key_value() {} failed with {}", buttonParamName, esp_err_to_name(result));
                ESP_LOGE(TAG, "%.*s", msg.size(), msg.data());
                CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
            }
        }

        char valueBuf[257];
        esphttpdutils::urldecode(valueBuf, valueBufEncoded);

        std::string_view value{valueBuf};

        if (auto parsed = cpputils::fromString<std::underlying_type_t<espgui::Button>>(value))
        {
            button = espgui::Button(*parsed);
        }
        else
        {
            const auto msg = fmt::format("could not parse {} {}", buttonParamName, value);
            ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
            CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
        }
    }

    if (!espgui::currentDisplay)
    {
        constexpr const std::string_view msg = "espgui::currentDisplay is null";
        ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
    }

    espgui::currentDisplay->buttonPressed(button);
    espgui::currentDisplay->buttonReleased(button);

    CALL_AND_EXIT_ON_ERROR(httpd_resp_set_hdr, req, "Location", "/")
    CALL_AND_EXIT_ON_ERROR(httpd_resp_set_hdr, req, "Access-Control-Allow-Origin", "http://web.bobbycar.cloud");
    CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::TemporaryRedirect, "text/html", "Ok, continue at <a href=\"/\">/</a>")
}

esp_err_t webserver_triggerItem_handler(httpd_req_t *req)
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

    std::string query;
    if (auto result = esphttpdutils::webserver_get_query(req))
        query = *result;
    else
    {
        ESP_LOGE(TAG, "%.*s", result.error().size(), result.error().data());
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", result.error());
    }

    std::size_t index;

    constexpr const std::string_view indexParamName{"index"};

    {
        char valueBufEncoded[256];
        if (const auto result = httpd_query_key_value(query.data(), indexParamName.data(), valueBufEncoded, 256); result != ESP_OK)
        {
            if (result == ESP_ERR_NOT_FOUND)
            {
                const auto msg = fmt::format("{} not set", indexParamName);
                ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
                CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
            }
            else
            {
                const auto msg = fmt::format("httpd_query_key_value() {} failed with {}", indexParamName, esp_err_to_name(result));
                ESP_LOGE(TAG, "%.*s", msg.size(), msg.data());
                CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
            }
        }

        char valueBuf[257];
        esphttpdutils::urldecode(valueBuf, valueBufEncoded);

        std::string_view value{valueBuf};

        if (auto parsed = cpputils::fromString<typeof(index)>(value))
        {
            index = *parsed;
        }
        else
        {
            const auto msg = fmt::format("could not parse {} {}", indexParamName, value);
            ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
            CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
        }
    }

    if (!espgui::currentDisplay)
    {
        constexpr const std::string_view msg = "espgui::currentDisplay is null";
        ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
    }

    auto *menuDisplay = espgui::currentDisplay->asMenuDisplay();
    if (!menuDisplay)
    {
        constexpr const std::string_view msg = "espgui::currentDisplay is not a menu display";
        ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
    }

    if (/*index < 0 ||*/ index >= menuDisplay->menuItemCount())
    {
        const auto msg = fmt::format("{} {} out of range (must be smaller than {})", indexParamName, index, menuDisplay->menuItemCount());
        ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
    }

    menuDisplay->getMenuItem(index).triggered();

    CALL_AND_EXIT_ON_ERROR(httpd_resp_set_hdr, req, "Location", "/")
    CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::TemporaryRedirect, "text/html", "Ok, continue at <a href=\"/\">/</a>")
}

esp_err_t webserver_setValue_handler(httpd_req_t *req)
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

    std::string query;
    if (auto result = esphttpdutils::webserver_get_query(req))
        query = *result;
    else
    {
        ESP_LOGE(TAG, "%.*s", result.error().size(), result.error().data());
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", result.error());
    }

    char valueBuf[257];
    constexpr const std::string_view valueParamName{"value"};

    {
        char valueBufEncoded[256];
        if (const auto result = httpd_query_key_value(query.data(), valueParamName.data(), valueBufEncoded, 256); result != ESP_OK)
        {
            if (result == ESP_ERR_NOT_FOUND)
            {
                const auto msg = fmt::format("{} not set", valueParamName);
                ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
                CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
            }
            else
            {
                const auto msg = fmt::format("httpd_query_key_value() {} failed with {}", valueParamName, esp_err_to_name(result));
                ESP_LOGE(TAG, "%.*s", msg.size(), msg.data());
                CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
            }
        }

        esphttpdutils::urldecode(valueBuf, valueBufEncoded);
    }

    std::string_view value{valueBuf};

    if (!espgui::currentDisplay)
    {
        constexpr const std::string_view msg = "espgui::currentDisplay is null";
        ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
    }

    if (auto *changeValueDisplay = espgui::currentDisplay->asChangeValueDisplayInterface())
    {
        int newValue;
        if (auto parsed = cpputils::fromString<typeof(newValue)>(value))
        {
            newValue = *parsed;
        }
        else
        {
            const auto msg = fmt::format("could not parse {} {}", valueParamName, value);
            ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
            CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
        }

        changeValueDisplay->setShownValue(newValue);
    }
    else if (auto *changeValueDisplay = espgui::currentDisplay->asChangeValueDisplayString())
    {
        changeValueDisplay->setShownValue(std::string{value});
    }
    else
    {
        constexpr const std::string_view msg = "espgui::currentDisplay is not a change value display";
        ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
        CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::BadRequest, "text/plain", msg);
    }


    CALL_AND_EXIT_ON_ERROR(httpd_resp_set_hdr, req, "Location", "/")
    CALL_AND_EXIT(esphttpdutils::webserver_resp_send, req, esphttpdutils::ResponseStatus::TemporaryRedirect, "text/html", "Ok, continue at <a href=\"/\">/</a>")
}
#endif
