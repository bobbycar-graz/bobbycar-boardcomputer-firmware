#pragma once

// 3rdparty lib includes
#include <ArduinoJson.h>
#ifdef FEATURE_BLE
#include <NimBLEDevice.h>
#endif
#ifdef FEATURE_LEDSTRIP
#include "hardware/ledstrip.h"
#endif

// local includes
#include "globals.h"
#include "futurecpp.h"
#include "modes/remotecontrolmode.h"
#include "utils.h"

//wifistack
#include "wifi_bobbycar.h"

#ifdef FEATURE_BLE
extern BLEServer *pServer;
extern BLEService *pService;
extern BLECharacteristic *livestatsCharacteristic;
extern BLECharacteristic *remotecontrolCharacteristic;
#ifdef FEATURE_WIRELESS_CONFIG
extern BLECharacteristic *wirelessConfig;
extern BLECharacteristic *getwifilist;
#endif

class RemoteControlCallbacks : public NimBLECharacteristicCallbacks
{
public:
    void onWrite(NimBLECharacteristic* pCharacteristic) override;
};

#ifdef FEATURE_WIRELESS_CONFIG
class WirelessSettingsCallbacks : public NimBLECharacteristicCallbacks
{
public:
    void onWrite(NimBLECharacteristic* pCharacteristic) override;
};

class WiFiListCallbacks : public NimBLECharacteristicCallbacks
{
public:
    void onRead(NimBLECharacteristic* pCharacteristic) override;
};
#endif

extern RemoteControlCallbacks bleRemoteCallbacks;

#ifdef FEATURE_WIRELESS_CONFIG
extern WirelessSettingsCallbacks bleWirelessSettingsCallbacks;
extern WiFiListCallbacks bleWiFiListCallbacks;
#endif

void initBle();

void handleBle();

#endif
