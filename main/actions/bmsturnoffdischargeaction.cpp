#include "bmsturnoffdischargeaction.h"

// local includes
#include "bmsutils.h"

#ifdef FEATURE_BMS
void BmsTurnOffDischargeAction::triggered()
{
    if (!bms::send6Bit(42405, 249, 0))
    {
        //Serial.println("Could not turn off discharge mosfet");
        // TODO: better error handling
    }
}
#endif
