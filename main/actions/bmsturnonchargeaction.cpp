#include "bmsturnonchargeaction.h"

// local includes
#include "bmsutils.h"

#ifdef FEATURE_BMS
void BmsTurnOnChargeAction::triggered()
{
    if (!bms::send6Bit(42405, 250, 1))
    {
        //Serial.println("Could not turn on charge mosfet");
        // TODO: better error handling
    }
}
#endif
