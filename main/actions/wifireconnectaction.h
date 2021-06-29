#pragma once

#include "actioninterface.h"

namespace {
class WifiReconnectAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        //if (!WiFi.reconnect())
        //{
            //Serial.println("Could not reconnect WiFi");
            // TODO: better error handling
        //}
    }
};
}
