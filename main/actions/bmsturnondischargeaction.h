#pragma once

#include "actioninterface.h"
#include "bmsutils.h"

namespace {
#ifdef FEATURE_BMS
class BmsTurnOnDischargeAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        if (!bms::send6Bit(42405, 249, 1))
        {
            //Serial.println("Could not turn on discharge mosfet");
            // TODO: better error handling
        }
    }
};
#endif
}
