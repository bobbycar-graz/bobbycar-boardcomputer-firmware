#pragma once

// 3rdparty lib includes
#include <fmt/core.h>

// local includes
#include "textinterface.h"
#include "globals.h"
#include "utils.h"

namespace bluetoothtexthelpers {
#ifdef FEATURE_BLUETOOTH
template<const char *Ttext, typename TreturnType, TreturnType (BluetoothSerial::*Tmethod)()>
using BluetoothStatusTextHelper = StatusTextHelper<Ttext, BluetoothSerial, &bluetoothSerial, TreturnType, Tmethod>;

constexpr char TEXT_BLUETOOTHAVAILABLE[] = "Available: ";
using BluetoothAvailableText = BluetoothStatusTextHelper<TEXT_BLUETOOTHAVAILABLE, int, &BluetoothSerial::available>;

constexpr char TEXT_BLUETOOTHHASCLIENT[] = "Has client: ";
using BluetoothHasClientText = BluetoothStatusTextHelper<TEXT_BLUETOOTHHASCLIENT, bool, &BluetoothSerial::hasClient>;

//constexpr char TEXT_BLUETOOTHCONNECTED[] = "Connected: ";
//using BluetoothConnectedText = BluetoothStatusTextHelper<TEXT_BLUETOOTHCONNECTED, bool, &BluetoothSerial::connected>;
struct BluetoothConnectedText : public virtual TextInterface {
public:
    std::string text() const override { return fmt::format("{}: {}", TEXT_BLUETOOTHCONNECTED, to_string(bluetoothSerial.connected())); }
};

//constexpr char TEXT_BLUETOOTHISREADY[] = "Is ready: ";
//using BluetoothIsReadyText = BluetoothStatusTextHelper<TEXT_BLUETOOTHISREADY, bool, &BluetoothSerial::isReady>;
struct BluetoothIsReadyText : public virtual TextInterface {
public:
    std::string text() const override { return fmt::format("{}: {}", TEXT_BLUETOOTHISREADY, to_string(bluetoothSerial.isReady())); }
};

//constexpr char TEXT_BLUETOOTHISREADYMASTER[] = "Is ready (M): ";
//using BluetoothIsReadyMasterText = BluetoothStatusTextHelper<TEXT_BLUETOOTHISREADYMASTER, bool, &BluetoothSerial::isReady>;
class BluetoothIsReadyMasterText : public virtual TextInterface {
public:
    std::string text() const override { return fmt::format("{}: {}", TEXT_BLUETOOTHISREADYMASTER, to_string(bluetoothSerial.isReady(true))); }
};
#endif
}
