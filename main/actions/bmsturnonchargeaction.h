#pragma once

#include "actioninterface.h"
#include "bmsutils.h"

namespace {
#ifdef FEATURE_BMS
class BmsTurnOnChargeAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        if (!bms::send6Bit(42405, 250, 1))
        {
            //Serial.println("Could not turn on charge mosfet");
            // TODO: better error handling
        }
    }
};
#endif
}
