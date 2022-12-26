#pragma once

// system includes
#include <optional>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <NimBLEDevice.h>
#include <espchrono.h>

typedef struct {
    NimBLEAddress address;
    std::string name;
} scanResult_t;

typedef struct {
    std::vector<scanResult_t> entries;
} scanResults_t;

const NimBLEUUID serviceUUID{"0000ffe0-0000-1000-8000-00805f9b34fb"};
const NimBLEUUID charRXUUID {"0000ffe1-0000-1000-8000-00805f9b34fb"};
const NimBLEUUID charTXUUID {"0000ffe1-0000-1000-8000-00805f9b34fb"}; // same as RX
// const NimBLEUUID charTXUUID {"0000ffe2-0000-1000-8000-00805f9b34fb"}; // different

class ANTBmsManager
{
public:
    static constexpr const char * const TAG = "ANTBMS-MANAGER";

    // basic functions
    void init();
    void update();
    void deinit();

    [[nodiscard]] bool isInitialized() const;

    // scans
    void startScan();

    [[nodiscard]] bool getScanStatus() const;

    const std::optional<scanResults_t> &getScanResults();
    void clearScanResults();

    void handleConnect();

    void notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

    void requestData();
    void sendCommand(uint8_t *pData, size_t length);

    void readBmsState();
private:

    class ScanResultsCallback : public NimBLEScanCallbacks
    {
    public:
        explicit ScanResultsCallback(ANTBmsManager* antBms) : m_antBms{antBms} {}

        void onScanEnd(NimBLEScanResults scanResults) override
        {
            m_antBms->m_scanResults.reset();

            ESP_LOGI(TAG, "BLE Scan complete");

            scanResults_t results;

            for (auto &result : scanResults)
            {
                if (result->isAdvertisingService(serviceUUID))
                {
                    scanResult_t scanResult;
                    scanResult.address = result->getAddress();
                    scanResult.name = result->getName();
                    results.entries.push_back(scanResult);
                }
            }

            m_antBms->m_scanResults = results;

            m_antBms->m_scanStarted = false;
        }
    private:
        ANTBmsManager* m_antBms;
    };

    class ClientCallbacks : public NimBLEClientCallbacks
    {
    public:
        explicit ClientCallbacks(ANTBmsManager* antBms) : m_antBms{antBms} {}
        void onConnect(NimBLEClient* pClient) override
        {
            m_antBms->m_connected = true;
            ESP_LOGD(TAG, "Connected to server");
        }

        void onDisconnect(NimBLEClient* pClient, int reason) override
        {
            m_antBms->m_connected = false;
            ESP_LOGI(TAG, "Disconnected from server (%d)", reason);
        }
    private:
        ANTBmsManager* m_antBms;
    };

    bool m_initialized{false};
    bool m_scanStarted{false};
    bool m_connected{false};

    bool m_newPacketReceived{false};
    bool m_toggle{false};

    std::optional<NimBLEClient*> m_client;
    std::optional<NimBLERemoteService*> m_service;
    std::optional<NimBLERemoteCharacteristic*> m_rxCharacteristic;
    std::optional<NimBLERemoteCharacteristic*> m_txCharacteristic;

    std::optional<scanResults_t> m_scanResults;

    espchrono::millis_clock::time_point m_lastRequestTime;
};
