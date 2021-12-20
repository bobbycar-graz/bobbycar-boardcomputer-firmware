#include "cloud.h"

// system includes
#include <optional>

// esp-idf includes
#include <esp_log.h>

using namespace std::chrono_literals;

namespace {
constexpr const char * const TAG = "BOBBYCLOUD";
} // namespace

#ifdef FEATURE_CLOUD

espcpputils::websocket_client cloudClient;
bool cloudStarted{};
espchrono::millis_clock::time_point lastCreateTry;
espchrono::millis_clock::time_point lastStartTry;
std::string cloudBuffer;

std::optional<espchrono::millis_clock::time_point> lastCloudCollect;
std::optional<espchrono::millis_clock::time_point> lastCloudSend;

void initCloud()
{
    if (settings.cloudSettings.cloudEnabled &&
        !stringSettings.cloudUrl.empty() &&
        esphttpdutils::urlverify(stringSettings.cloudUrl))
    {
        createCloud();
        if (!cloudClient)
            return;

        if (wifi_stack::get_sta_status() != wifi_stack::WiFiStaStatus::CONNECTED)
            return;

        startCloud();
    }
}

void updateCloud()
{
    const auto now = espchrono::millis_clock::now();

    if (!lastCloudCollect || now - *lastCloudCollect >= std::chrono::milliseconds{settings.boardcomputerHardware.timersSettings.cloudCollectRate})
    {
        cloudCollect();

        lastCloudCollect = now;
    }

    if (!lastCloudSend || now - *lastCloudSend >= 1000ms/settings.boardcomputerHardware.timersSettings.cloudSendRate)
    {
        cloudSend();

        lastCloudSend = now;
    }
}

void cloudCollect()
{
    if (!cloudClient)
    {
        cloudBuffer.clear();
        return;
    }

    if (!cloudStarted)
    {
        cloudBuffer.clear();
        return;
    }

    if (!cloudClient.is_connected())
    {
        cloudBuffer.clear();
        return;
    }

    if (cloudBuffer.empty())
        cloudBuffer = '[';
    else
        cloudBuffer += ',';

    cloudBuffer += fmt::format("[{},{},{}",
                      std::chrono::milliseconds{espchrono::millis_clock::now().time_since_epoch()}.count(),
                      std::chrono::milliseconds{espchrono::utc_clock::now().time_since_epoch()}.count(),
                      heap_caps_get_free_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT));
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
    {
        if (const auto &result = wifi_stack::get_sta_ap_info(); result)
            cloudBuffer += fmt::format(",{}", result->rssi);
        else
            cloudBuffer += ",null";
    }
    else
        cloudBuffer += ",null";

    if (raw_gas)
        cloudBuffer += fmt::format(",{}", *raw_gas);
    else
        cloudBuffer += ",null";

    if (raw_brems)
        cloudBuffer += fmt::format(",{}", *raw_brems);
    else
        cloudBuffer += ",null";

    if (gas)
        cloudBuffer += fmt::format(",{:.1f}", *gas);
    else
        cloudBuffer += ",null";

    if (brems)
        cloudBuffer += fmt::format(",{:.1f}", *brems);
    else
        cloudBuffer += ",null";

    constexpr const auto addController = [](const Controller &controller){
        if (!controller.feedbackValid)
        {
            cloudBuffer += ",null";
            return;
        }

        cloudBuffer += fmt::format(",[{:.02f},{:.02f}",
                           controller.getCalibratedVoltage(),
                           fixBoardTemp(controller.feedback.boardTemp));

        constexpr const auto addMotor = [](const bobbycar::protocol::serial::MotorState &command,
                                           const bobbycar::protocol::serial::MotorFeedback &feedback,
                                           bool invert){
            cloudBuffer += fmt::format(",[{},{:.2f},{:.2f},{}]",
                               command.pwm * (invert?-1:1),
                               convertToKmh(feedback.speed) * (invert?-1:1),
                               fixCurrent(feedback.dcLink),
                               feedback.error);
        };

        addMotor(controller.command.left, controller.feedback.left, controller.invertLeft);
        addMotor(controller.command.right, controller.feedback.right, controller.invertRight);

        cloudBuffer += ']';
    };

    addController(controllers.front);
    addController(controllers.back);

    //cloudBuffer += fmt::format("", );

    cloudBuffer += "]";
}

void cloudSend()
{
    if (settings.cloudSettings.cloudEnabled &&
        !stringSettings.cloudUrl.empty() &&
        esphttpdutils::urlverify(stringSettings.cloudUrl))
    {
        if (!cloudClient)
        {
            if (espchrono::ago(lastCreateTry) < 10s)
                return;
            createCloud();
        }
        if (!cloudClient)
            return;

        if (!cloudStarted)
        {
            if (espchrono::ago(lastStartTry) < 10s)
                return;

            if (wifi_stack::get_sta_status() != wifi_stack::WiFiStaStatus::CONNECTED)
                return;

            startCloud();
        }
        if (!cloudStarted)
            return;

        if (!cloudClient.is_connected())
            return;

        if (cloudBuffer.empty())
            return;

        cloudBuffer += ']';

        const auto timeout = std::chrono::ceil<espcpputils::ticks>(espchrono::milliseconds32{settings.cloudSettings.cloudTransmitTimeout}).count();
        const auto written = cloudClient.send_text(cloudBuffer, timeout);

        if (written < 0)
        {
            ESP_LOGE("BOBBY", "cloudClient.send_text() failed with %i", written);
        }
        else if (written != cloudBuffer.size())
        {
            ESP_LOGE("BOBBY", "websocket sent size mismatch, sent=%i, expected=%i", written, cloudBuffer.size());
        }

        cloudBuffer.clear();
    }
    else if (cloudClient)
    {
        destroyCloud();
    }
}

void createCloud()
{
    ESP_LOGI("BOBBY", "called");

    if (cloudClient)
    {
        ESP_LOGE(TAG, "cloud client already created");
        return;
    }

    lastCreateTry = espchrono::millis_clock::now();

    const esp_websocket_client_config_t config = {
        .uri = stringSettings.cloudUrl.c_str(),
    };

    cloudClient = espcpputils::websocket_client{&config};

    if (!cloudClient)
    {
        ESP_LOGE(TAG, "websocket could not be constructed");
        return;
    }

    ESP_LOGI("BOBBY", "cloud client created");
}

void startCloud()
{
    ESP_LOGI("BOBBY", "called");

    if (!cloudClient)
    {
        ESP_LOGE(TAG, "cloud client not created");
        return;
    }

    if (cloudStarted)
    {
        ESP_LOGE(TAG, "cloud client already started");
        return;
    }

    lastStartTry = espchrono::millis_clock::now();

    const auto result = cloudClient.start();
    ESP_LOG_LEVEL_LOCAL((result == ESP_OK ? ESP_LOG_INFO : ESP_LOG_ERROR), "BOBBY", "cloudClient.start() returned: %s", esp_err_to_name(result));

    if (result == ESP_OK)
        cloudStarted = true;
}

void destroyCloud()
{
    ESP_LOGI("BOBBY", "called");

    if (!cloudClient)
    {
        ESP_LOGE(TAG, "cloud client not created");
        return;
    }

    cloudClient = {};
    cloudStarted = false;
}

#endif
