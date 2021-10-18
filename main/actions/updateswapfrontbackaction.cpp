#include "updateswapfrontbackaction.h"

// local includes
#include "utils.h"

#ifdef FEATURE_SERIAL
void UpdateSwapFrontBackAction::triggered()
{
    updateSwapFrontBack();
}
#endif
