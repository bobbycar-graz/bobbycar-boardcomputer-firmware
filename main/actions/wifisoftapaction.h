#pragma once

// local includes
#include "actioninterface.h"
#include "globals.h"
#include "utils.h"
#include "macros_bobbycar.h"

namespace {
class WifiSoftApAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        //if (!WiFi.softAP(deviceName, STRING(AP_PASSWORD)))
        //{
            //Serial.println("Could not softAP WiFi");
            // TODO: better error handling
        //}
    }
};
}
