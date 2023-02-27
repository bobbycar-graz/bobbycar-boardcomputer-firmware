#include "ble_bobby.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <ArduinoJson.h>
#include <wifi_bobbycar.h>

// local includes
#include "globals.h"
#include "ledstrip.h"
#include "modes/defaultmode.h"
#include "modes/remotecontrolmode.h"
#include "newsettings.h"
#include "utils.h"

namespace {
constexpr const char * const TAG = "BOBBYBLE";

class RemoteControlCallbacks : public NimBLECharacteristicCallbacks
{
public:
    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override;
};

class WirelessSettingsCallbacks : public NimBLECharacteristicCallbacks
{
public:
    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override;
};

class WiFiListCallbacks : public NimBLECharacteristicCallbacks
{
public:
    void onRead(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override;
};

} // namespace

BLEServer *pServer{};
BLEService *pService{};
BLECharacteristic *livestatsCharacteristic{};
BLECharacteristic *remotecontrolCharacteristic{};
BLECharacteristic *wirelessConfig{};
BLECharacteristic *getwifilist{};

namespace {
RemoteControlCallbacks bleRemoteCallbacks;

WirelessSettingsCallbacks bleWirelessSettingsCallbacks;
WiFiListCallbacks bleWiFiListCallbacks;

void createBle()
{
    ESP_LOGI("BOBBY", "called");

    if (!NimBLEDevice::getInitialized())
    {
        ESP_LOGI("BOBBY", "Initializing BLE");
        NimBLEDevice::init(configs.bluetoothName.value());
        NimBLEDevice::setPower(ESP_PWR_LVL_P9);
    }

    const auto serviceUuid{"0335e46c-f355-4ce6-8076-017de08cee98"};

    ESP_LOGI(TAG, "Creating BLE server");

    pServer = NimBLEDevice::createServer();

    ESP_LOGI(TAG, "Creating BLE service");

    pService = pServer->createService(serviceUuid);

    ESP_LOGI(TAG, "Creating BLE characteristics");

    livestatsCharacteristic = pService->createCharacteristic("a48321ea-329f-4eab-a401-30e247211524", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    remotecontrolCharacteristic = pService->createCharacteristic("4201def0-a264-43e6-946b-6b2d9612dfed", NIMBLE_PROPERTY::WRITE);
    remotecontrolCharacteristic->setCallbacks(&bleRemoteCallbacks);

    wirelessConfig = pService->createCharacteristic("4201def1-a264-43e6-946b-6b2d9612dfed", NIMBLE_PROPERTY::WRITE);
    wirelessConfig->setCallbacks(&bleWirelessSettingsCallbacks);
    getwifilist = pService->createCharacteristic("4201def2-a264-43e6-946b-6b2d9612dfed", NIMBLE_PROPERTY::READ);
    getwifilist->setCallbacks(&bleWiFiListCallbacks);

    ESP_LOGI(TAG, "Starting BLE service");

    pService->start();

    ESP_LOGI(TAG, "Starting BLE advertising");

    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(serviceUuid);
    pAdvertising->setScanResponse(true);
    NimBLEDevice::startAdvertising();
}

void destroyBle()
{
    ESP_LOGI("BOBBY", "called");

    if (NimBLEDevice::getInitialized())
    {
        NimBLEDevice::deinit(true);
    }

    pServer = {};
    pService = {};
    livestatsCharacteristic = {};
    remotecontrolCharacteristic = {};
    wirelessConfig = {};
    getwifilist = {};
}
} // namespace

void initBle()
{
    if (configs.bleSettings.bleEnabled.value())
        createBle();
}


void handleBle()
{
    if (!configs.feature.ble.isEnabled.value())
        return;

    if (configs.bleSettings.bleEnabled.value())
    {
        if (!pServer)
            createBle();

        if (livestatsCharacteristic->getSubscribedCount())
        {
            StaticJsonDocument<1024> doc;
            {
                auto arr = doc.createNestedArray("v");
                if (controllers.front.feedbackValid)
                    arr.add(controllers.front.getCalibratedVoltage());
                else
                    arr.add(nullptr);
                if (controllers.back.feedbackValid)
                    arr.add(controllers.back.getCalibratedVoltage());
                else
                    arr.add(nullptr);
            }

            {
                auto arr = doc.createNestedArray("t");
                if (controllers.front.feedbackValid)
                    arr.add(fixBoardTemp(controllers.front.feedback.boardTemp));
                else
                    arr.add(nullptr);
                if (controllers.back.feedbackValid)
                    arr.add(fixBoardTemp(controllers.back.feedback.boardTemp));
                else
                    arr.add(nullptr);
            }

            {
                auto arr = doc.createNestedArray("e");
                if (controllers.front.feedbackValid)
                {
                    arr.add(controllers.front.feedback.left.error);
                    arr.add(controllers.front.feedback.right.error);
                }
                else
                {
                    arr.add(nullptr);
                    arr.add(nullptr);
                }
                if (controllers.back.feedbackValid)
                {
                    arr.add(controllers.back.feedback.left.error);
                    arr.add(controllers.back.feedback.right.error);
                }
                else
                {
                    arr.add(nullptr);
                    arr.add(nullptr);
                }
            }

            {
                auto arr = doc.createNestedArray("s");
                if (controllers.front.feedbackValid)
                {
                    arr.add(convertToKmh(controllers.front.feedback.left.speed * (profileSettings.controllerHardware.invertFrontLeft ? -1 : 1)));
                    arr.add(convertToKmh(controllers.front.feedback.right.speed * (profileSettings.controllerHardware.invertFrontRight ? -1 : 1)));
                }
                else
                {
                    arr.add(nullptr);
                    arr.add(nullptr);
                }
                if (controllers.back.feedbackValid)
                {
                    arr.add(convertToKmh(controllers.back.feedback.left.speed * (profileSettings.controllerHardware.invertBackLeft ? -1 : 1)));
                    arr.add(convertToKmh(controllers.back.feedback.right.speed * (profileSettings.controllerHardware.invertBackRight ? -1 : 1)));
                }
                else
                {
                    arr.add(nullptr);
                    arr.add(nullptr);
                }
            }

            {
                auto arr = doc.createNestedArray("a");
                if (controllers.front.feedbackValid)
                {
                    arr.add(fixCurrent(controllers.front.feedback.left.dcLink) * 2);
                    arr.add(fixCurrent(controllers.front.feedback.right.dcLink) * 2);
                }
                else
                {
                    arr.add(nullptr);
                    arr.add(nullptr);
                }
                if (controllers.back.feedbackValid)
                {
                    arr.add(fixCurrent(controllers.back.feedback.left.dcLink) * 2);
                    arr.add(fixCurrent(controllers.back.feedback.right.dcLink) * 2);
                }
                else
                {
                    arr.add(nullptr);
                    arr.add(nullptr);
                }
            }

            std::string json;
            serializeJson(doc, json);

            livestatsCharacteristic->setValue(json);
            livestatsCharacteristic->notify();
        }
    }
    else if (pServer)
    {
        destroyBle();
    }
}

namespace {

void RemoteControlCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo)
{
    const std::string& val = pCharacteristic->getValue();

    StaticJsonDocument<256> doc;
    if (const auto error = deserializeJson(doc, val))
    {
        ESP_LOGW(TAG, "ignoring cmd with invalid json: %.*s %s", val.size(), val.data(), error.c_str());
        return;
    }

    if (configs.feature.ledstrip.isEnabled.value())
    {
        const auto newBlinkAnimation = doc["anim"].as<int16_t>();
        if (blinkAnimation != newBlinkAnimation) blinkAnimation = newBlinkAnimation;
    }

    const bool isInverted = (profileSettings.controllerHardware.invertFrontLeft && !profileSettings.controllerHardware.invertFrontRight);

    if (!simplified)
    {
        RemoteCommand cmd {
            .frontLeft = doc[isInverted ? "fr":"fl"].as<int16_t>(),
            .frontRight = doc[isInverted ? "fl":"fr"].as<int16_t>(),
            .backLeft = doc["bl"].as<int16_t>(),
            .backRight = doc["br"].as<int16_t>()
        };
        modes::defaultMode.setRemoteCommand(cmd);
        modes::remoteControlMode.setRemoteCommand(cmd);
    }
}

void WirelessSettingsCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo)
{
    const std::string& val = pCharacteristic->getValue();

    StaticJsonDocument<256> doc;
    if (const auto error = deserializeJson(doc, val))
    {
        ESP_LOGW(TAG, "ignoring cmd with invalid json: %.*s %s", val.size(), val.data(), error.c_str());
        return;
    }

    auto write_type = doc["type"].as<std::string>();

    if (write_type == "wifi") {
        const int index = doc["wifi_index"].as<int>();
        ESP_LOGI(TAG, "Set wifi%i: WiFi-SSID: %s, WiFi-Password: ***", doc["wifi_index"].as<int>(), doc["wifi_ssid"].as<const char*>());
        configs.write_config(configs.wifi_configs[index].ssid, doc["wifi_ssid"].as<std::string>());
        configs.write_config(configs.wifi_configs[index].key, doc["wifi_pass"].as<std::string>());
    } else {
        const auto deserialized = deserializeJson(doc, val);
        ESP_LOGW(TAG, "Unkown type %s -> json: %.*s %s", doc["type"].as<const char*>(), val.size(), val.data(), deserialized.c_str());
    }
}

void WiFiListCallbacks::onRead(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo)
{
    StaticJsonDocument<768> responseDoc;
    auto wifiArray = responseDoc.createNestedArray("wifis");
    ESP_LOGI(TAG, "Got request for listing wifi ssids.");
    for (const auto &wifi : configs.wifi_configs)
    {
        wifiArray.add(wifi.ssid.value());
    }
    responseDoc["wifi_count"] = configs.wifi_configs.size();
    std::string json;
    serializeJson(responseDoc, json);
    pCharacteristic->setValue(json);
}
} // namespace
