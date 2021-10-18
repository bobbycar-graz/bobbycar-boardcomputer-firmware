#pragma once

// system includes
#include <cstdint>

// 3rdparty lib includes
#include "actioninterface.h"

template<int16_t type>
class LedStripSetAnimationAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};

/*
class LedstripAnimationDefaultRainbowAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override { animation_type = LEDSTRIP_ANIMATION_TYPE_DEFAULTRAINBOW; }
};

class LedstripAnimationBetterRainbowAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override { animation_type = LEDSTRIP_ANIMATION_TYPE_BETTERRAINBOW; }
};

class LedstripAnimationSyncToSpeedAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override { animation_type = LEDSTRIP_ANIMATION_TYPE_SPEEDSYNCANIMATION; }
};
*/
