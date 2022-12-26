#include "bmsclearscanaction.h"

// local includes
#include "bmsutils.h"

void BMSClearScanAction::triggered()
{
    bmsutils::antBms.clearScanResults();
}
