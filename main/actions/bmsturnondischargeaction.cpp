#include "bmsturnondischargeaction.h"

// local includes
#include "bmsutils.h"

#ifdef FEATURE_BMS
void BmsTurnOnDischargeAction::triggered()
{
    if (!bms::send6Bit(42405, 249, 1))
    {
        //Serial.println("Could not turn on discharge mosfet");
        // TODO: better error handling
    }
}
#endif
