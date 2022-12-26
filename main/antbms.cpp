#include "antbms.h"

// local includes
#include "bmsutils.h"
#include "newsettings.h"

using namespace std::chrono_literals;

void ANTBms::init()
{
    // init code
    m_initialized = true;

    // scan
    startScan();
}

void ANTBms::update()
{
    if (!m_initialized)
        return;

    handleConnect();

    if (m_client && (*m_client)->isConnected())
    {
        requestData();
    }
}

void ANTBms::deinit()
{
    // deinit code
    m_initialized = false;

    if (m_client)
    {
        (*m_client)->disconnect();
        m_client.reset();
    }

    if (m_scanResults)
    {
        m_scanResults.reset();
    }

    if (m_service)
    {
        m_service.reset();
    }

    if (m_rxCharacteristic)
    {
        m_rxCharacteristic.reset();
    }

    if (m_txCharacteristic)
    {
        m_txCharacteristic.reset();
    }

    m_scanStarted = false;
    m_initialized = false;
}

bool ANTBms::isInitialized() const
{
    return m_initialized;
}

void ANTBms::startScan()
{
    if (!m_initialized)
        return;

    ESP_LOGI(TAG, "starting scan");

    NimBLEScan* pScan = NimBLEDevice::getScan();
    pScan->setActiveScan(true);
    pScan->setInterval(100);
    pScan->setWindow(99);
    pScan->setScanCallbacks(new ScanResultsCallback(this), false);
    pScan->start(5000);

    ESP_LOGI(TAG, "scan started");

    m_scanStarted = true;
}

void ANTBms::clearScanResults()
{
    m_scanResults.reset();
}

bool ANTBms::getScanStatus() const
{
    return m_scanStarted;
}

const std::optional<scanResults_t> &ANTBms::getScanResults()
{
    return m_scanResults;
}

void ANTBms::handleConnect()
{
    if (!m_initialized)
        return;

    ESP_LOGD(TAG, "!m_initialized passed");

    if (m_connected)
        return;

    ESP_LOGD(TAG, "m_connected passed");

    if (!m_scanResults)
        return;

    ESP_LOGD(TAG, "!m_scanResults passed");

    if (m_scanResults && m_scanResults->entries.empty())
        return;

    ESP_LOGD(TAG, "m_scanResults->entries.empty() passed");

    if (m_client && (*m_client)->isConnected())
        return;

    ESP_LOGD(TAG, "!m_client.has_value() passed");

    if (configs.bmsAddress.value().empty())
        return;

    ESP_LOGD(TAG, "configs.bmsAddress.value().empty() passed");

    ESP_LOGI(TAG, "connecting to bms");

    if (NimBLEDevice::getClientListSize())
    {
        m_client = NimBLEDevice::getClientByPeerAddress(configs.bmsAddress.value());

        if (m_client)
        {
            if (!(*m_client)->connect(configs.bmsAddress.value()))
            {
                ESP_LOGE(TAG, "Reconnect failed");
                m_client.reset();
                m_service.reset();
                m_rxCharacteristic.reset();
                m_txCharacteristic.reset();

                m_connected = false;
                return;
            }
            ESP_LOGI(TAG, "Reconnected to client");
        }
        else
        {
            m_client = NimBLEDevice::getDisconnectedClient();
        }
    }

    if (!m_client)
    {
        if (NimBLEDevice::getClientListSize() >= NIMBLE_MAX_CONNECTIONS)
        {
            ESP_LOGE(TAG, "Max clients reached - no more connections available!");
            return;
        }

        m_client = NimBLEDevice::createClient();

        const auto pClient = *m_client;

        pClient->setClientCallbacks(new ClientCallbacks(this), false);
        pClient->setConnectTimeout(10);
        pClient->setConnectionParams(12, 12, 0, 51);

        if (!pClient->connect(configs.bmsAddress.value()))
        {
            NimBLEDevice::deleteClient(pClient);
            m_client.reset();
            m_service.reset();
            m_rxCharacteristic.reset();
            m_txCharacteristic.reset();

            m_connected = false;
            return;
        }
    }

    if (!(*m_client)->isConnected())
    {
        if (!(*m_client)->connect(configs.bmsAddress.value()))
        {
            ESP_LOGE(TAG, "Failed to connect");
            m_client.reset();
            m_service.reset();
            m_rxCharacteristic.reset();
            m_txCharacteristic.reset();

            m_connected = false;
            return;
        }
    }

    ESP_LOGI(TAG, "Connected!");
    m_connected = true;

    m_service = (*m_client)->getService(serviceUUID);

    if (!m_service)
    {
        ESP_LOGE(TAG, "Failed to find our service UUID: %s", serviceUUID.toString().c_str());
        m_client.reset();
        m_service.reset();
        m_rxCharacteristic.reset();
        m_txCharacteristic.reset();

        m_connected = false;
        return;
    }

    if (m_service && (*m_service))
    {
        ESP_LOGI(TAG, "Getting characteristic");
        m_rxCharacteristic = (*m_service)->getCharacteristic(charRXUUID);
        m_txCharacteristic = (*m_service)->getCharacteristic(charTXUUID);

        if ((m_rxCharacteristic && (*m_rxCharacteristic)) && (m_txCharacteristic && (*m_txCharacteristic)))
        {
            const auto pChr = *m_rxCharacteristic;

            if (pChr->canNotify())
            {
                ESP_LOGI(TAG, "Subscribing to notifications");
                if (!pChr->subscribe(true, bmsutils::_notifyCB))
                {
                    (*m_client)->disconnect();
                    ESP_LOGE(TAG, "Failed to subscribe for notifications");
                    m_client.reset();
                    m_service.reset();
                    m_rxCharacteristic.reset();

                    m_connected = false;
                    return;
                }
                ESP_LOGI(TAG, "Subscribed for notifications");

                m_connected = true;

                // 7ea1010000be1855aa55
                uint8_t data[10] = {0x7e, 0xa1, 0x01, 0x00, 0x00, 0xbe, 0x18, 0x55, 0xaa, 0x55};
                sendCommand(data, sizeof(data));
                return;
            }
            else
            {
                ESP_LOGE(TAG, "Characteristic can't notify, disconnecting");
                (*m_client)->disconnect();
                m_client.reset();
                m_service.reset();
                m_rxCharacteristic.reset();

                m_connected = false;
                return;
            }
        }
        else
        {
            ESP_LOGE(TAG, "Failed to find our characteristic UUID: %s", charRXUUID.toString().c_str());
            m_client.reset();
            m_service.reset();
            m_rxCharacteristic.reset();

            m_connected = false;
            return;
        }
    }
}

void ANTBms::notifyCB(NimBLERemoteCharacteristic *pRemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
{
    ESP_LOGI(TAG, "Received %s: %s (%.*s)", isNotify ? "notification" : "indication", bmsutils::bytesToHex(pData, length).c_str(), length, pData);
}

void ANTBms::requestData()
{
    if (!m_initialized)
        return;

    if (!m_connected)
        return;

    if (espchrono::ago(m_lastRequestTime) > 2000ms || m_newPacketReceived)
    {
        m_lastRequestTime = espchrono::millis_clock::now();

        if (m_toggle)
        {
            bmsGetInfo3();
            m_newPacketReceived = false;
        }
        else
        {
            bmsGetInfo4();
            m_newPacketReceived = false;
        }
        m_toggle = !m_toggle;
    }
}

void ANTBms::sendCommand(uint8_t *pData, size_t length)
{
    if (!m_initialized)
        return;

    if (!m_connected)
        return;

    if (!m_txCharacteristic)
        return;

    if (!(*m_txCharacteristic))
        return;

    if (!(*m_txCharacteristic)->canWrite())
        return;

    ESP_LOGI(TAG, "Sending command: %s", bmsutils::bytesToHex(pData, length).c_str());

    (*m_txCharacteristic)->writeValue(pData, length, true);
}

void ANTBms::bmsGetInfo3()
{
    // DD A5 03 00 FF FD 77
    uint8_t data[7] = {0xdd, 0xa5, 0x3, 0x0, 0xff, 0xfd, 0x77};

    sendCommand(data, sizeof(data));
}

void ANTBms::bmsGetInfo4()
{
    // DD A5 04 00 FF FC 77
    uint8_t data[7] = {0xdd, 0xa5, 0x4, 0x0, 0xff, 0xfc, 0x77};

    sendCommand(data, sizeof(data));
}
