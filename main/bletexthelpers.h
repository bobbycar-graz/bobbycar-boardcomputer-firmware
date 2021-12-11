#pragma once

// local includes
#include "ble_bobby.h"
#include "textinterface.h"

#ifdef FEATURE_BLE
struct BleServerPeerDevicesText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override
    {
        std::string text = "peerDevices: ";
        if (pServer)
            text += std::to_string(pServer->getPeerDevices().size());
        return text;
    }
};

struct BleCharacSubscribedText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override
    {
        std::string text = "subscribed: ";
        if (livestatsCharacteristic)
            text += std::to_string(livestatsCharacteristic->getSubscribedCount());
        return text;
    }
};
#endif
