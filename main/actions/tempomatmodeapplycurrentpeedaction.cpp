#include "tempomatmodeapplycurrentpeedaction.h"

// local includes
#include "globals.h"
#include "modes/tempomatmode.h"

void TempomatModeApplyCurrentSpeedAction::triggered()
{
    modes::tempomatMode.nCruiseMotTgt = avgSpeed;
}
