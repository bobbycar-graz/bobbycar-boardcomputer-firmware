#include "ledstripselectblinkmenu.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "actions/ledstripblinkactions.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"
#include "ledstrip.h"
#include "ledstripdefines.h"

namespace bobby {

namespace {
constexpr char TEXT_BLINKANIMATION[] = "Blink animation";
constexpr char TEXT_ANIMATION_BLINKNONE[] = "Blink Off";
constexpr char TEXT_ANIMATION_BLINKLEFT[] = "Blink Left";
constexpr char TEXT_ANIMATION_BLINKRIGHT[] = "Blink Right";
constexpr char TEXT_ANIMATION_BLINKBOTH[] = "Blink Both";
constexpr char TEXT_LEDSTRIP_EN_BLINK_ANIM[] = "Animated Blink";
constexpr char TEXT_LEDSTRIP_VISUALIZE_BLINK[] = "Visualize Blink";
constexpr char TEXT_BACK[] = "Back";

class CurrentSelectedBlinkAnimationText : public virtual espgui::TextInterface
{
public:
    std::string text() const override;
};

} // namespace

LedstripSelectBlinkMenu::LedstripSelectBlinkMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, CurrentSelectedBlinkAnimationText,             DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                     DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_BLINKNONE>,          LedstripAnimationBlinkNoneAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_BLINKLEFT>,          LedstripAnimationBlinkLeftAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_BLINKRIGHT>,         LedstripAnimationBlinkRightAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_BLINKBOTH>,          LedstripAnimationBlinkBothAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LEDSTRIP_EN_BLINK_ANIM>,       BobbyCheckbox, LedstripEnableBlinkAnimationAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LEDSTRIP_VISUALIZE_BLINK>,     BobbyCheckbox, LedstripEnableVisualizeBlinkAnimationAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                         PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string LedstripSelectBlinkMenu::text() const
{
    return TEXT_BLINKANIMATION;
}

void LedstripSelectBlinkMenu::back()
{
    espgui::popScreen();
}

namespace {

std::string CurrentSelectedBlinkAnimationText::text() const
{
    switch (blinkAnimation)
    {
    case LEDSTRIP_OVERWRITE_BLINKLEFT:
#ifndef LEDSTRIP_WRONG_DIRECTION
        return TEXT_ANIMATION_BLINKLEFT;
#else
        return TEXT_ANIMATION_BLINKRIGHT;
#endif
    case LEDSTRIP_OVERWRITE_BLINKRIGHT:
#ifndef LEDSTRIP_WRONG_DIRECTION
        return TEXT_ANIMATION_BLINKRIGHT;
#else
        return TEXT_ANIMATION_BLINKLEFT;
#endif
    case LEDSTRIP_OVERWRITE_BLINKBOTH:
        return TEXT_ANIMATION_BLINKBOTH;
    default:
        return TEXT_ANIMATION_BLINKNONE;
    }
}

} // namespace

} // namespace bobby
