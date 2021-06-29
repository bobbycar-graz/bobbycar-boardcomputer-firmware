#pragma once

#include "actioninterface.h"
#include "bmsutils.h"

namespace {
#ifdef FEATURE_BMS
class BmsTurnOffChargeAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        if (!bms::send6Bit(42405, 250, 0))
        {
            //Serial.println("Could not turn off charge mosfet");
            // TODO: better error handling
        }
    }
};
#endif
}
