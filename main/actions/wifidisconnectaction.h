#pragma once

#include "actioninterface.h"

namespace {
class WifiDisconnectAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        //if (!WiFi.disconnect())
        //{
            //Serial.println("Could not disconnect WiFi");
            // TODO: better error handling
        //}
    }
};
}
