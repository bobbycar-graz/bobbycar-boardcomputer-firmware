#ifdef FEATURE_ESPNOW
#include "espnowfunctions.h"

#include <string>

#include <espchrono.h>
#include <esp_log.h>
#include <numberparsing.h>
#include <espwifistack.h>
#include <asynchttprequest.h>
#include <delayedconstruction.h>
#include <cleanuphelper.h>

#include "globals.h"
#include "utils.h"
#include "time_bobbycar.h"
#define PIN_RELAY 17

using namespace std::chrono_literals;

namespace espnow {
namespace {
constexpr const char * const TAG = "BOBBY_ESP_NOW";
} // namespace

std::optional<espchrono::millis_clock::time_point> ledtimer;
std::optional<espchrono::millis_clock::time_point> last_send_ms;

uint16_t lastYear; // Used for esp-now timesync

std::deque<esp_now_message_t> message_queue{};
std::vector<esp_now_peer_info_t> peers{};
uint8_t initialized{0};

bool receiveTimeStamp{true};
bool receiveTsFromOtherBobbycars{true};

namespace ifttt {
namespace {
cpputils::DelayedConstruction<AsyncHttpRequest> http_request;
}
void setup_request()
{
    if (!http_request.constructed())
    {
        http_request.construct("qr_request", espcpputils::CoreAffinity::Core0);
    }
}

tl::expected<void, std::string> start_qr_request(std::string event)
{
    if (!http_request.constructed())
    {
        return tl::make_unexpected("request im oarsch");
    }

    if (const auto res = http_request->start(fmt::format("http://maker.ifttt.com/trigger/{}/with/key/{}", event, stringSettings.ifttt_key)); !res)
    {
        return res;
    }
    return{};
}

tl::expected<std::string, std::string> check_request()
{
    if (!http_request.constructed())
    {
        return tl::make_unexpected("request im oarsch");
    }

    if (!http_request->finished())
    {
        return tl::make_unexpected("request has not finished");
    }

    const auto helper = cpputils::makeCleanupHelper([](){ http_request->clearFinished(); });

    if (const auto result = http_request->result(); !result)
    {
        return tl::make_unexpected(result.error());
    }
    else
    {
        ESP_LOGI(TAG, "%.*s", http_request->buffer().size(), http_request->buffer().data());
        return http_request->takeBuffer();
    }
}

bool get_request_running()
{
    if (!http_request.constructed())
    {
        return false;
    }

    return http_request->finished();
}
} // namespace

namespace {
extern "C" void onReceive(const uint8_t *mac_addr, const uint8_t *data, int len)
{
    const std::string message(data, data + len);
    ESP_LOGI(TAG, "Received data (%s)", message.c_str());

    ESP_LOGI(TAG, "ID: %s - Token: %s", stringSettings.esp_now_door_id.c_str(), stringSettings.esp_now_door_token.c_str());

    if (settings.repeatESPNow)
    {
        ESP_LOGI(TAG, "Repeat ESP-NOW message \"%s\"", message.c_str());
        send_espnow_message(message);
    }

    // Parse the message (msg format: "msg_type:msg_value:msg_token") via find and npos
    const size_t pos = message.find(":");
    if (pos == std::string::npos) {
        ESP_LOGW(TAG, "Invalid message format");
        return;
    }
    const size_t pos2 = message.find(":", pos + 1);
    if (pos2 == std::string::npos) {
        ESP_LOGW(TAG, "Invalid message format");
        return;
    }
    const std::string msg_type = message.substr(0, pos);
    const std::string msg_value = message.substr(pos + 1, pos2 - pos - 1);
    std::string msg_token{};
    if (pos2 + 1 < message.size()) {
        msg_token = message.substr(pos2 + 1);
    }

    if (msg_token.empty()) {
        ESP_LOGW(TAG, "No token was send");
        return;
    }

    if (msg_token != stringSettings.esp_now_door_token) {
        ESP_LOGW(TAG, "Invalid token (%s)", msg_token.c_str());
        return;
    }

    if (msg_type == "BOBBYOPEN" && msg_value == stringSettings.esp_now_door_id)
    {
        //Serial.println("Bob is opening the door");
        digitalWrite(PIN_RELAY, HIGH);
        ledtimer = espchrono::millis_clock::now();
    }
    else if(msg_type == "BOBBYOPEN" && msg_value == stringSettings.esp_now_ifttt_door_id)
    {

    }
    /*const std::string_view data_str{(const char *)data, size_t(data_len)};

    size_t sep_pos = data_str.find(":");
    if (std::string_view::npos != sep_pos)
    {
        std::string_view msg_type = data_str.substr(0, sep_pos);
        std::string_view msg = data_str.substr(sep_pos+1, data_str.length()-3); // - 3 may needs to be converted to sep_pos+1
        ESP_LOGD(TAG, "Type: %.*s - Message: %.*s", msg_type.size(), msg_type.data(), msg.size(), msg.data());

        message_queue.push_back(esp_now_message_t {
            .content = std::string{msg},
            .type = std::string{msg_type}
        });
    }
    else
    {
        ESP_LOGW(TAG, "Invalid message: Could not find ':' (%.*s)", data_str.size(), data_str.data());
    }*/
}
} // namespace

void initESPNow()
{
    pinMode(PIN_RELAY, OUTPUT);
    digitalWrite(PIN_RELAY, LOW);

    ESP_LOGI(TAG, "Initializing esp-now...");

    if (initialized < 1)
    {
        if (!settings.wifiSettings.wifiApEnabled && (!settings.wifiSettings.wifiStaEnabled && wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::NO_SHIELD) || (wifi_stack::get_wifi_mode() != wifi_mode_t::WIFI_MODE_STA && wifi_stack::get_wifi_mode() != wifi_mode_t::WIFI_MODE_AP && wifi_stack::get_wifi_mode() != wifi_mode_t::WIFI_MODE_APSTA))
        {
            ESP_LOGW(TAG, "cannot execute esp_now_init(): tcp stack is down.");
            return;
        }
        else
            initialized = 1;
    }

    if (initialized < 2)
    {
        if (const auto error = esp_now_init(); error != ESP_OK)
        {
            ESP_LOGE(TAG, "esp_now_init() failed with %s", esp_err_to_name(error));
            return;
        }
        else
            initialized = 2;
    }

    if (initialized < 3)
    {
        if (const auto error = esp_now_register_recv_cb(onReceive); error != ESP_OK)
        {
            ESP_LOGE(TAG, "esp_now_register_recv_cb() failed with %s", esp_err_to_name(error));
            return;
        }
        else
            initialized = 3;
    }

    if (initialized < 4)
    {
        peers.push_back(esp_now_peer_info_t{});
        esp_now_peer_info_t &peer = peers.back();
        std::memcpy(peer.peer_addr, broadcast_address, sizeof(peer.peer_addr));
        peer.channel = 0;

        if (settings.wifiSettings.wifiApEnabled)
            peer.ifidx = WIFI_IF_AP;
        else if (settings.wifiSettings.wifiStaEnabled)
            peer.ifidx = WIFI_IF_STA;
        else
        {
            ESP_LOGE(TAG, "Interfaces not ready.");
            return;
        }

        if (const auto error = esp_now_add_peer(&peers.back()); error != ESP_OK)
        {
            ESP_LOGE(TAG, "esp_now_add_peer() failed with %s", esp_err_to_name(error));
            return;
        }
        else
            initialized = 4;
    }

    initialized = 255;
    ESP_LOGI(TAG, "Init done.");
}

void handle()
{
    if (!ledtimer || espchrono::ago(*ledtimer) > 3s) {
        digitalWrite(PIN_RELAY, LOW);
    }

    if (!last_send_ms || espchrono::ago(*last_send_ms) > 1s && !settings.repeatESPNow) {
        const auto message = fmt::format("T:{}", espchrono::utc_clock::now().time_since_epoch().count());
        send_espnow_message(message);
        last_send_ms = espchrono::millis_clock::now();
    }


    if (initialized < 255 && !(!settings.wifiSettings.wifiApEnabled && (!settings.wifiSettings.wifiStaEnabled && wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::NO_SHIELD) || (wifi_stack::get_wifi_mode() != wifi_mode_t::WIFI_MODE_STA && wifi_stack::get_wifi_mode() != wifi_mode_t::WIFI_MODE_AP && wifi_stack::get_wifi_mode() != wifi_mode_t::WIFI_MODE_APSTA)))
    {
        initESPNow();
        return;
    }
    else if (!settings.wifiSettings.wifiApEnabled && (!settings.wifiSettings.wifiStaEnabled && wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::NO_SHIELD) || (wifi_stack::get_wifi_mode() != wifi_mode_t::WIFI_MODE_STA && wifi_stack::get_wifi_mode() != wifi_mode_t::WIFI_MODE_AP && wifi_stack::get_wifi_mode() != wifi_mode_t::WIFI_MODE_APSTA))
    {
        if (initialized > 0)
        {
            if (initialized >= 4) // peer
            {
                for (const auto &peer : peers)
                {
                    if (const auto error = esp_now_del_peer(peer.peer_addr); error != ESP_OK)
                    {
                        if (error == ESP_ERR_ESPNOW_NOT_FOUND)
                        {
                            initialized = 0;
                            return;
                        }
                        ESP_LOGE(TAG, "esp_now_del_peer() failed with %s", esp_err_to_name(error));
                        return;
                    }
                }
                initialized--;
            }

            if (initialized >= 3) // callback
            {
                if (const auto error = esp_now_unregister_recv_cb(); error != ESP_OK)
                {
                    ESP_LOGE(TAG, "esp_now_unregister_recv_cb() failed with %s", esp_err_to_name(error));
                    return;
                }
                else if (error == ESP_ERR_ESPNOW_NOT_FOUND)
                {
                    initialized = 0;
                }
                else
                    initialized--;
            }

            if (initialized >= 2) // esp deinit
            {
                if (const auto error = esp_now_deinit(); error != ESP_OK)
                {
                    ESP_LOGE(TAG, "esp_now_deinit() failed with %s", esp_err_to_name(error));
                    return;
                }
                else if (error == ESP_ERR_ESPNOW_NOT_FOUND)
                {
                    initialized = 0;
                }
                else
                    initialized--;
            }

            initialized = 0;
            ESP_LOGI(TAG, "Deinit done.");
        }
        return;
    }

    if(message_queue.size())
    {
        for (const esp_now_message_t &msg : message_queue)
        {
            ESP_LOGD(TAG, "queue has processed message of type '%s' with content '%s'", msg.type.c_str(), msg.content.c_str());
            message_queue.pop_front();

            if (msg.type == "T")
            {
                if (!receiveTimeStamp || !settings.espnow.syncTime)
                    return;

                if (const auto result = cpputils::fromString<uint64_t>(msg.content); result)
                {
                    onRecvTs(*result);
                }
                else
                {
                    ESP_LOGW(TAG, "could not parse number: %.*s", result.error().size(), result.error().data());
                }
            }
            else if (msg.type == "BOBBYT")
            {
                if (!receiveTsFromOtherBobbycars || !settings.espnow.syncTimeWithOthers)
                    return;

                if (const auto result = cpputils::fromString<uint64_t>(msg.content); result)
                {
                    ESP_LOGI(TAG, "setting current time to %" PRIu64, *result);
                    onRecvTs(*result, true);
                }
                else
                {
                    ESP_LOGW(TAG, "could not parse number: %.*s", result.error().size(), result.error().data());
                }
            }
            else
            {
                ESP_LOGI(TAG, "Unkown Type: %s - Message: %s", msg.type.c_str(), msg.content.c_str());
            }
        }
    }
}

void onRecvTs(uint64_t millis, bool isFromBobbycar)
{
    const auto milliseconds = std::chrono::milliseconds(millis);
    const auto timepoint = espchrono::utc_clock::time_point(milliseconds);
    // ESP_LOGW(TAG, "setting current time to %s", espchrono::toString(timepoint.time_since_epoch()).c_str());
    time_set_now(timepoint);

    if (receiveTimeStamp)
    {
        if (const auto thisYear = int(espchrono::toDateTime(espchrono::utc_clock::now()).date.year()); abs(thisYear - espnow::lastYear) > 1)
        {
            espnow::lastYear = thisYear;
            receiveTimeStamp = false;
        }
    }

    receiveTsFromOtherBobbycars = false;
}

esp_err_t send_espnow_message(std::string_view message)
{
    if (initialized < 255)
        return ESP_ERR_ESPNOW_NOT_INIT;

    if (!settings.wifiSettings.wifiApEnabled && !settings.wifiSettings.wifiStaEnabled)
    {
        return ESP_ERR_ESPNOW_IF;
    }

    if (peers.size() < 1)
    {
        return ESP_FAIL;
    }

    for (auto &peer : peers)
    {

        if (settings.wifiSettings.wifiApEnabled)
            peer.ifidx = WIFI_IF_AP;
        else if (settings.wifiSettings.wifiStaEnabled)
            peer.ifidx = WIFI_IF_STA;
        else
            return ESP_ERR_ESPNOW_IF;

        const auto timeBefore = espchrono::millis_clock::now();

        if(const auto error = esp_now_send(broadcast_address, (const uint8_t*)message.data(), message.size()); error != ESP_OK)
        {
            return error;
        }
        else
        {
            const auto timeAfter = espchrono::millis_clock::now();
            ESP_LOGI(TAG, "Successfully executed esp_now_send() with message %.*s (Took %lldms)", message.size(), message.data(), std::chrono::milliseconds{timeAfter-timeBefore}.count());
        }
    }
    return ESP_OK;
}

} // namespace espnow
#endif
