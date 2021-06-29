#pragma once

#include "actioninterface.h"

namespace {
class WifiSoftApEnableIpV6Action : public virtual ActionInterface
{
public:
    void triggered() override
    {
        //if (!WiFi.softAPenableIpV6())
        //{
            //Serial.println("Could not softAPenableIpV6 WiFi");
            // TODO: better error handling
        //}
    }
};
}
