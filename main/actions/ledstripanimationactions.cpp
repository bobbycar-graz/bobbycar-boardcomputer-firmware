#include "ledstripanimationactions.h"

// local includes
#include "utils.h"
#include "globals.h"
#include "ledstrip.h"
// #include "ledstripdefines.h"

template<int16_t type>
void LedStripSetAnimationAction<type>::triggered()
{
    animation_type = type;
}

template void LedStripSetAnimationAction<LEDSTRIP_ANIMATION_TYPE_DEFAULTRAINBOW>::triggered();
template void LedStripSetAnimationAction<LEDSTRIP_ANIMATION_TYPE_BETTERRAINBOW>::triggered();
template void LedStripSetAnimationAction<LEDSTRIP_ANIMATION_TYPE_SPEEDSYNCANIMATION>::triggered();
