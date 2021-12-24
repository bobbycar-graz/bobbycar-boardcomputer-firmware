#include "ledstripblinkactions.h"

#ifdef FEATURE_LEDSTRIP
#include "hardware/ledstrip.h"
#include "helpers/ledstripdefines.h"

using namespace espgui;

void LedstripAnimationBlinkNoneAction::triggered()
{
    blinkAnimation = LEDSTRIP_OVERWRITE_NONE;
}

#ifndef LEDSTRIP_WRONG_DIRECTION
void LedstripAnimationBlinkLeftAction::triggered()
{
    blinkAnimation = LEDSTRIP_OVERWRITE_BLINKLEFT;
}
#else
void LedstripAnimationBlinkLeftAction::triggered()
{
    blinkAnimation = LEDSTRIP_OVERWRITE_BLINKRIGHT;
}
#endif

#ifndef LEDSTRIP_WRONG_DIRECTION
void LedstripAnimationBlinkRightAction::triggered()
{
    blinkAnimation = LEDSTRIP_OVERWRITE_BLINKRIGHT;
}
#else
void LedstripAnimationBlinkRightAction::triggered()
{
    blinkAnimation = LEDSTRIP_OVERWRITE_BLINKLEFT;
}
#endif

void LedstripAnimationBlinkBothAction::triggered()
{
    blinkAnimation = LEDSTRIP_OVERWRITE_BLINKBOTH;
}
#endif
