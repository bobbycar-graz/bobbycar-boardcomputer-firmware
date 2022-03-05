#include "ledstripselectanimationmenu.h"

// Local includes
#include "actions/dummyaction.h"
#include "actions/ledstripanimationactions.h"
#include "actions/popscreenaction.h"
#include "icons/back.h"
#include "ledstripdefines.h"
#include "ledstripmenu.h"
#include "newsettings.h"

#ifdef FEATURE_LEDSTRIP
namespace {
constexpr char TEXT_SELECTANIMATION[] = "Select Animation";
constexpr char TEXT_ANIMATION_DEFAULTRAINBOW[] = "Default Rainbow";
constexpr char TEXT_ANIMATION_BETTERRAINBOW[] = "Better Rainbow";
constexpr char TEXT_ANIMATION_SPEEDSYNCANIMATION[] = "Speed Sync";
constexpr char TEXT_ANIMATION_CUSTOMCOLOR[] = "Custom Color";
constexpr char TEXT_BACK[] = "Back";

class CurrentSelectedAnimationText : public virtual espgui::TextInterface
{
public:
    std::string text() const override;
};

} // namespace

LedstripSelectAnimationMenu::LedstripSelectAnimationMenu()
{
    using namespace espgui;

    constructMenuItem<makeComponent<MenuItem, CurrentSelectedAnimationText,                  DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                     DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_DEFAULTRAINBOW>,     LedStripSetAnimationAction<LedstripAnimation::DefaultRainbow>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_BETTERRAINBOW>,      LedStripSetAnimationAction<LedstripAnimation::BetterRainbow>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_SPEEDSYNCANIMATION>, LedStripSetAnimationAction<LedstripAnimation::SpeedSync>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_CUSTOMCOLOR>,        LedStripSetAnimationAction<LedstripAnimation::CustomColor>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                         PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string LedstripSelectAnimationMenu::text() const
{
    return TEXT_SELECTANIMATION;
}

void LedstripSelectAnimationMenu::back()
{
    espgui::popScreen();
}

namespace {

std::string CurrentSelectedAnimationText::text() const
{
    switch (configs.ledstrip.animationType.value)
    {
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

} // namespace
#endif
