#include "bmsturnoffchargeaction.h"

// local includes
#include "bmsutils.h"

#ifdef FEATURE_BMS
void BmsTurnOffChargeAction::triggered()
{
    if (!bms::send6Bit(42405, 250, 0))
    {
        //Serial.println("Could not turn off charge mosfet");
        // TODO: better error handling
    }
}
#endif
