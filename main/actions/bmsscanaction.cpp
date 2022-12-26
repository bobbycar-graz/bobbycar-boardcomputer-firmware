#include "bmsscanaction.h"

// local includes
#include "bmsutils.h"

void BMSScanAction::triggered()
{
    bmsutils::antBms.startScan();
}
