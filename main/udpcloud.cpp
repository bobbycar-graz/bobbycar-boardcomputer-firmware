#include "udpcloud.h"

// 3rd party includes
#include <ArduinoJson.h>
#include <FastLED.h>
#include <espcppmacros.h>
#include <espstrutils.h>
#include <esp_ota_ops.h>
#include <espwifistack.h>

// local includes
#include "udpsender.h"
#include "espwifistack.h"
#include "esp_log.h"
#include "fmt/format.h"
#include "globals.h"
#include "utils.h"
#include "lwip/dns.h"
#include "espchrono.h"
#include "battery.h"
#include "drivingstatistics.h"
#include "newsettings.h"

using namespace std::chrono_literals;

namespace {
constexpr const char * const TAG = "bobbycloud";

espchrono::millis_clock::time_point timestampLastFailed;
} // namespace

// Little "flash" on statusdisplay when udp stuff is happening
bool visualSendUdpPacket;

void udpCloudInit()
{

}

void udpCloudUpdate()
{
    if (!configs.feature.udpcloud.value)
        return;

    if (configs.udpCloudSettings.udpCloudEnabled.value && configs.udpCloudSettings.udpUid.touched())
        sendUdpCloudPacket();
}

void spamUdpBroadcast()
{
    wifi_stack::UdpSender sender;

    if (!sender.ready())
    {
        ESP_LOGE(TAG, "could not init udp sender!");
        return;
    }

    std::string buf;
    const auto uptime = espchrono::millis_clock::now().time_since_epoch() / 1ms;

    buf = fmt::format("uptime: {}", uptime);

    if (const auto result = sender.send(ESP_IF_WIFI_STA, 187, buf); !result)
    {
        ESP_LOGE(TAG, "broadcast failed");
    }
}

std::string buildUdpCloudJson()
{
    static std::string version_string;
    if (version_string.empty() || version_string == "-")
    {
        if (const esp_app_desc_t *app_desc = esp_ota_get_app_description())
        {
            version_string = app_desc->version;
        }
        else
        {
            version_string = "-";
        }
    }
    StaticJsonDocument<1024> doc;
    std::string buf;
    const auto uptime = espchrono::millis_clock::now().time_since_epoch() / 1ms;

    float watt{0};
    const auto avgVoltage = controllers.getAvgVoltage();
    if(avgVoltage)
        watt = sumCurrent * *avgVoltage;

    // const auto w_per_kmh = watt / avgSpeedKmh;

    // User ID
    doc["uid"] = configs.udpCloudSettings.udpUid.value;
    doc["upt"] = uptime;

    const auto addController = [&](const Controller &controller, const bool isBack) {
        if (controller.feedbackValid)
        {
            auto arr = doc.createNestedObject(!isBack ? "f":"b");
            // Voltage
            arr["V"] = controller.getCalibratedVoltage();

            // Amperes
            arr["lA"] = fixCurrent(controller.feedback.left.dcLink);
            arr["rA"] = fixCurrent(controller.feedback.right.dcLink);

            // Temperature
            arr[!isBack ? "fT":"bT"] = fixBoardTemp(controller.feedback.boardTemp);

            // Errors
            arr[!isBack ? "flE":"blE"] = controller.feedback.left.error;
            arr[!isBack ? "frE":"brE"] = controller.feedback.right.error;

            // Speed
            arr[!isBack ? "flS":"blS"] = convertToKmh(controller.feedback.left.speed) * (controller.invertLeft?-1:1);
            arr[!isBack ? "frS":"brS"] = convertToKmh(controller.feedback.right.speed) * (controller.invertRight?-1:1);
        }
        else
        {
            doc[!isBack ? "f":"b"] = nullptr;
        }
    };

    addController(controllers.front, false);
    addController(controllers.back, true);

    // Potis
    {
        auto arr = doc.createNestedObject("p");
        if (gas)
            arr["g"] = *gas;
        if (raw_gas)
            arr["rg"] = *raw_gas;
        if (brems)
            arr["b"] = *brems;
        if (raw_brems)
            arr["rb"] = *raw_brems;
    }

    // Statistics
    if(avgVoltage)
    {
        doc["bP"] = getBatteryPercentage(*avgVoltage, BatteryCellType(configs.battery.cellType.value));
        doc["bV"] = *avgVoltage;
    }
    doc["l"] = isLocked;
    doc["mN"] = drivingStatistics.meters_driven;
    doc["mT"] = drivingStatistics.totalMeters;
    doc["dT"] = drivingStatistics.currentDrivingTime / 1ms;
    doc["cW"] = watt;
    doc["wN"] = drivingStatistics.wh_used;
    doc["wL"] = getRemainingWattHours();
    doc["kmL"] = getRemainingWattHours() / configs.battery.watthoursPerKilometer.value;
    doc["ver"] = version_string.substr(0, 6);

    serializeJson(doc, buf);
    return buf;
}

// Same as buildUdpCloudJson, but doesnt use ArduinoJson (Probably will not expand; added for performance testing)
std::string buildUdpCloudString()
{
    static std::string version_string;
    if (version_string.empty() || version_string == "-")
    {
        if (const esp_app_desc_t *app_desc = esp_ota_get_app_description())
        {
            version_string = app_desc->version;
        }
        else
        {
            version_string = "-";
        }
    }
//    StaticJsonDocument<1024> doc;
    std::string buf;
    buf.reserve(1024);

    const auto uptime = espchrono::millis_clock::now().time_since_epoch() / 1ms;

    float watt{0};
    const auto avgVoltage = controllers.getAvgVoltage();
    if(avgVoltage)
        watt = sumCurrent * *avgVoltage;
    // const auto w_per_kmh = watt / avgSpeedKmh;

    buf += "{";

    // User ID
    if(configs.udpCloudSettings.udpUid.value)
        buf += fmt::format("\"uid\":{},", configs.udpCloudSettings.udpUid.value);
    else
        buf += "\"uid\":null,";

    if(uptime)
        buf += fmt::format("\"upt\":{},", uptime);
    else
        buf += "\"uid\":null,";

    const auto addController = [&](const Controller &controller, const bool isBack) {
        if (controller.feedbackValid)
        {
            buf += fmt::format("\"{}\":{", !isBack ? "f":"b");
            // Voltage
            if (controller.getCalibratedVoltage())
                buf += fmt::format("\"V\":{},", controller.getCalibratedVoltage());
            else
                buf += "\"V\":null,";

            // Amperes
            if (controller.feedback.left.dcLink)
                buf += fmt::format("\"lA\":{},", fixCurrent(controller.feedback.left.dcLink));
            else
                buf += "\"lA\":null,";

            if (controller.feedback.right.dcLink)
                buf += fmt::format("\"rA\":{},", fixCurrent(controller.feedback.right.dcLink));
            else
                buf += "\"rA\":null,";

            // Temperature
            if (controller.feedback.right.dcLink)
                buf += fmt::format("\"{}\":{},", !isBack ? "fT":"bT", fixBoardTemp(controller.feedback.boardTemp));
            else
                buf += fmt::format("\"{}\":null,", !isBack ? "fT":"bT");

            // Errors
            if (controller.feedback.left.error)
                buf += fmt::format("\"{}\":{},", !isBack ? "flE":"blE", controller.feedback.left.error);
            else
                buf += fmt::format("\"{}\":null,", !isBack ? "flE":"blE");

            if (controller.feedback.right.error)
                buf += fmt::format("\"{}\":{},", !isBack ? "frE":"brE", controller.feedback.right.error);
            else
                buf += fmt::format("\"{}\":null,", !isBack ? "frE":"brE");


            // Speed
            if (controller.feedback.left.speed)
                buf += fmt::format("\"{}\":{},", !isBack ? "flS":"blS", convertToKmh(controller.feedback.left.speed) * (controller.invertLeft?-1:1));
            else
                buf += fmt::format("\"{}\":null,", !isBack ? "flS":"blS");

            if (controller.feedback.right.speed)
                buf += fmt::format("\"{}\":{},", !isBack ? "frS":"brS", convertToKmh(controller.feedback.right.speed) * (controller.invertRight?-1:1));
            else
                buf += fmt::format("\"{}\":null,", !isBack ? "frS":"brS");

            buf += "},";
        }
        else
        {
            buf += fmt::format("\"{}\":null,", !isBack ? "f":"b");
        }
    };

    addController(controllers.front, false);
    addController(controllers.back, true);

    // Potis
    {
        buf += "\"p\":{";
        if (gas)
            buf += fmt::format("\"g\":{},",*gas);
        else
            buf += "\"g\":null,";

        if (raw_gas)
            buf += fmt::format("\"rg\":{},",*raw_gas);
        else
            buf += "\"rg\":null,";

        if (brems)
            buf += fmt::format("\"b\":{},",*brems);
        else
            buf += "\"b\":null,";

        if (raw_brems)
            buf += fmt::format("\"rb\":{},",*raw_brems);
        else
            buf += "\"rb\":null,";

        buf += "},";
    }

    // Statistics
    if(avgVoltage)
    {
        buf += fmt::format("\"bP\":{},", getBatteryPercentage(*avgVoltage, BatteryCellType(configs.battery.cellType.value)));
        buf += fmt::format("\"bV\":{},", *avgVoltage);
    }
    buf += fmt::format("\"l\":{},", isLocked);
    buf += fmt::format("\"mN\":{},", drivingStatistics.meters_driven);
    buf += fmt::format("\"mT\":{},", drivingStatistics.totalMeters);
    buf += fmt::format("\"dT\":{},", drivingStatistics.currentDrivingTime / 1ms);
    buf += fmt::format("\"cW\":{},", watt);
    buf += fmt::format("\"wN\":{},", drivingStatistics.wh_used);
    buf += fmt::format("\"wL\":{},", getRemainingWattHours());
    buf += fmt::format("\"kmL\":{},", getRemainingWattHours() / configs.battery.watthoursPerKilometer.value);
    buf += fmt::format("\"ver\":{}", version_string.substr(0, 6));

    buf += "}";

    return buf;
}

void sendUdpCloudPacket()
{
    EVERY_N_MILLIS(configs.boardcomputerHardware.timersSettings.udpSendRateMs.value) {
        if (espchrono::ago(timestampLastFailed) < 2s)
        {
            visualSendUdpPacket = false;
            return;
        }

        if (configs.udpCloudHost.value.empty())
        {
            visualSendUdpPacket = false;
            return;
        }

        if (wifi_stack::get_sta_status() != wifi_stack::WiFiStaStatus::CONNECTED)
        {
            visualSendUdpPacket = false;
            return;
        }

        ip_addr_t udpCloudIp;

        if (const auto res = dns_gethostbyname(configs.udpCloudHost.value.c_str(), &udpCloudIp, nullptr, nullptr); res != ERR_OK)
        {
            if (res == ERR_INPROGRESS)
            {
                ESP_LOGD(TAG, "dns_gethostbyname() failed because: %i", res);
            }
            else
            {
                ESP_LOGE(TAG, "dns_gethostbyname() failed because: %i", res);
            }
            timestampLastFailed = espchrono::millis_clock::now();
            visualSendUdpPacket = false;
            return;
        }

        if (udpCloudIp.type != IPADDR_TYPE_V4)
        {
            ESP_LOGE(TAG, "unsupported ip type: %hhu", udpCloudIp.type);
            timestampLastFailed = espchrono::millis_clock::now();
            visualSendUdpPacket = false;
            return;
        }

        sockaddr_in receipient;
        receipient.sin_port = htons(24242);
        receipient.sin_addr.s_addr = udpCloudIp.u_addr.ip4.addr;
        receipient.sin_family = AF_INET;

        wifi_stack::UdpSender udpCloudSender;
        std::string buf;
            buf = configs.udpCloudSettings.udpUseStdString.value ? buildUdpCloudString() : buildUdpCloudJson();


        if (const auto result = udpCloudSender.send(receipient, buf); !result)
        {
            timestampLastFailed = espchrono::millis_clock::now();
            ESP_LOGE(TAG, "send to cloud failed: %.*s (ip=%s)", result.error().size(), result.error().data(), wifi_stack::toString(udpCloudIp.u_addr.ip4).c_str());
        }

        ESP_LOGD(TAG, "now: %s", buf.c_str());
        visualSendUdpPacket = !visualSendUdpPacket;
        }
}
