#include "ledstripblinkactions.h"

// local includes
#include "utils.h"
#include "globals.h"
#include "ledstrip.h"
#include "ledstripdefines.h"

void LedstripAnimationBlinkNoneAction::triggered()
{
    blinkAnimation = LEDSTRIP_OVERWRITE_NONE;
}

void LedstripAnimationBlinkLeftAction::triggered()
{
#ifndef LEDSTRIP_WRONG_DIRECTION
    blinkAnimation = LEDSTRIP_OVERWRITE_BLINKLEFT;
#else
    blinkAnimation = LEDSTRIP_OVERWRITE_BLINKRIGHT;
#endif
}

void LedstripAnimationBlinkRightAction::triggered()
{
#ifndef LEDSTRIP_WRONG_DIRECTION
    blinkAnimation = LEDSTRIP_OVERWRITE_BLINKRIGHT;
#else
    blinkAnimation = LEDSTRIP_OVERWRITE_BLINKLEFT;
#endif
}

void LedstripAnimationBlinkBothAction::triggered()
{
    blinkAnimation = LEDSTRIP_OVERWRITE_BLINKBOTH;
}
