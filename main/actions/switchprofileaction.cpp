#include "switchprofileaction.h"

// local includes
#include "settingsutils.h"

template<uint8_t index>
void SwitchProfileAction<index>::triggered()
{
    switchProfile(index);
}

template void SwitchProfileAction<0>::triggered();
template void SwitchProfileAction<1>::triggered();
template void SwitchProfileAction<2>::triggered();
template void SwitchProfileAction<3>::triggered();
