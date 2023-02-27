#include "antbmsmanager.h"

// local includes
#include "antbms.h"
#include "bmsutils.h"
#include "newsettings.h"

using namespace std::chrono_literals;

void ANTBmsManager::init()
{
    // init code
    m_initialized = true;

    // scan
    startScan();
}

void ANTBmsManager::update()
{
    if (!m_initialized)
        return;

    handleConnect();

    if (m_client && (*m_client)->isConnected())
    {
        requestData();
    }
}

void ANTBmsManager::deinit()
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

bool ANTBmsManager::isInitialized() const
{
    return m_initialized;
}

void ANTBmsManager::startScan()
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

void ANTBmsManager::clearScanResults()
{
    m_scanResults.reset();
}

bool ANTBmsManager::getScanStatus() const
{
    return m_scanStarted;
}

const std::optional<scanResults_t> &ANTBmsManager::getScanResults()
{
    return m_scanResults;
}

void ANTBmsManager::handleConnect()
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

        m_client = NimBLEDevice::createClient(); // this sometimes crashes with StoreProhibited

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

void ANTBmsManager::notifyCB(NimBLERemoteCharacteristic *pRemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
{
    ESP_LOGI(TAG, "Received %s: %s (%.*s)", isNotify ? "notification" : "indication", bmsutils::bytesToHex(pData, length).c_str(), length, pData);
}

void ANTBmsManager::requestData()
{
    if (!m_initialized)
        return;

    if (!m_connected)
        return;

    if (espchrono::ago(m_lastRequestTime) > 2000ms || m_newPacketReceived)
    {
        m_lastRequestTime = espchrono::millis_clock::now();

        readBmsState();
    }
}

void ANTBmsManager::sendCommand(uint8_t *pData, size_t length)
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

void ANTBmsManager::readBmsState()
{
    BmsInstruction bmsInstruction = BmsInstruction(1, -66);
    ESP_LOGI(TAG, "read real status, inst:  %s", bmsInstruction.toString().c_str());
    const auto inst = bmsInstruction.getInstruction();
    sendCommand(inst, sizeof(inst));
}

