#include "ledstripselectanimationmenu.h"

// Local includes
#include "actions/dummyaction.h"
#include "actions/ledstripanimationactions.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "helpers/ledstripdefines.h"
#include "ledstripmenu.h"

using namespace espgui;


#ifdef FEATURE_LEDSTRIP
std::string currentSelectedAnimationText::text() const
{
    switch (animation_type) {
    case LedstripAnimation::DefaultRainbow:
        return TEXT_ANIMATION_DEFAULTRAINBOW;
    case LedstripAnimation::BetterRainbow:
        return TEXT_ANIMATION_BETTERRAINBOW;
    case LedstripAnimation::SpeedSync:
        return TEXT_ANIMATION_SPEEDSYNCANIMATION;
    case LedstripAnimation::CustomColor:
        return TEXT_ANIMATION_CUSTOMCOLOR;
    default:
        return "Animation Unkown";
    }
}

LedstripSelectAnimationMenu::LedstripSelectAnimationMenu()
{
    constructMenuItem<makeComponent<MenuItem, currentSelectedAnimationText,                  DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                     DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_DEFAULTRAINBOW>,     LedStripSetAnimationAction<LedstripAnimation::DefaultRainbow>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_BETTERRAINBOW>,      LedStripSetAnimationAction<LedstripAnimation::BetterRainbow>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_SPEEDSYNCANIMATION>, LedStripSetAnimationAction<LedstripAnimation::SpeedSync>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_CUSTOMCOLOR>,        LedStripSetAnimationAction<LedstripAnimation::CustomColor>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                         SwitchScreenAction<LedstripMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void LedstripSelectAnimationMenu::back()
{
    switchScreen<LedstripMenu>();
}
#endif
