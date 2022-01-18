#pragma once

// esp-idf includes
#include <NimBLEDevice.h>

extern BLEServer *pServer;
extern BLEService *pService;
extern BLECharacteristic *livestatsCharacteristic;
extern BLECharacteristic *remotecontrolCharacteristic;
extern BLECharacteristic *wirelessConfig;
extern BLECharacteristic *getwifilist;

void initBle();

void handleBle();
