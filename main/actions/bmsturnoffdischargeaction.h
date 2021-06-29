#pragma once

#include "actioninterface.h"
#include "bmsutils.h"

namespace {
#ifdef FEATURE_BMS
class BmsTurnOffDischargeAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        if (!bms::send6Bit(42405, 249, 0))
        {
            //Serial.println("Could not turn off discharge mosfet");
            // TODO: better error handling
        }
    }
};
#endif
}
