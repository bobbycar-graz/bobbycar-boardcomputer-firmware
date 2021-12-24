#pragma once

#include "actioninterface.h"
#include "hardware/ledstrip.h"
// #include "helpers/ledstripdefines.h"

#ifdef FEATURE_LEDSTRIP
template<uint16_t type>
class LedStripSetAnimationAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override { animation_type = type; }
};
#endif
