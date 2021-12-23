#include "ledstripmenu.h"

#ifdef FEATURE_LEDSTRIP
// 3rdparty lib includes
#include <FastLED.h>
#include <actioninterface.h>
#include <actions/switchscreenaction.h>
#include <actions/toggleboolaction.h>
#include <changevaluedisplay.h>
#include <checkboxicon.h>
#include <icons/back.h>
#include <menuitem.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "displays/ledstripcolorsdisplay.h"
#include "displays/menus/ledstripselectotamode.h"
#include "displays/menus/mainmenu.h"
#include "globals.h"
#include "hardware/ledstrip.h"
#include "ledstripselectanimationmenu.h"
#include "ledstripselectblinkmenu.h"

// clang-format off

using namespace espgui;

namespace {
using LedsCountChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_LEDSCOUNT>,
    LedsCountAccessor,
    BackActionInterface<SwitchScreenAction<LedstripMenu>>,
    SwitchScreenAction<LedstripMenu>
>;

using CenterOffsetChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_CENTEROFFSET>,
    CenterOffsetAccessor,
    BackActionInterface<SwitchScreenAction<LedstripMenu>>,
    SwitchScreenAction<LedstripMenu>
>;

using SmallOffsetChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SMALLOFFSET>,
    SmallOffsetAccessor,
    BackActionInterface<SwitchScreenAction<LedstripMenu>>,
    SwitchScreenAction<LedstripMenu>
>;

using BigOffsetChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_BIGOFFSET>,
    BigOffsetAccessor,
    BackActionInterface<SwitchScreenAction<LedstripMenu>>,
    SwitchScreenAction<LedstripMenu>
>;

using DeziampereChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_LEDSTRIP_MILLIAMP>,
    DeziampereAccessor,
    BackActionInterface<SwitchScreenAction<LedstripMenu>>,
    SwitchScreenAction<LedstripMenu>
>;

using StVOOffsetChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_STVO_FRONTOFFSET>,
    LedsStVOFrontOffsetAccessor,
    BackActionInterface<SwitchScreenAction<LedstripMenu>>,
    SwitchScreenAction<LedstripMenu>
>;

using StVOLengthChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_STVO_FRONTLENGTH>,
    LedsStVOFrontLengthAccessor,
    BackActionInterface<SwitchScreenAction<LedstripMenu>>,
    SwitchScreenAction<LedstripMenu>
>;

using animationMultiplierChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_ANIMATION_MULTIPLIER>,
    AnimationMultiplierAccessor,
    BackActionInterface<SwitchScreenAction<LedstripMenu>>,
    SwitchScreenAction<LedstripMenu>
>;

using ledstripBrightnessChangeScreen = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_LEDSTRIP_BRIGHTNESS>,
    LedstripBrightnessAccessor,
    BackActionInterface<SwitchScreenAction<LedstripMenu>>,
    SwitchScreenAction<LedstripMenu>
>;

class AllCustomLedsOffAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        for(int index = 0; index < 8; index++)
        {
            ledstrip_custom_colors[index] = CRGB{0,0,0};
        }
    }
};
} // namespace

LedstripMenu::LedstripMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LEDSTRIPCOLORMENU>,                               SwitchScreenAction<LedstripColorsDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LEDANIMATION>, ToggleBoolAction, CheckboxIcon,    EnableLedAnimationAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BRAKELIGHTS>,  ToggleBoolAction, CheckboxIcon,    EnableBrakeLightsAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLINKBEEP>, ToggleBoolAction, CheckboxIcon,       EnableBeepWhenBlinkAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FULLBLINK>, ToggleBoolAction, CheckboxIcon,       EnableFullBlinkAccessor>>();

    if (!simplified) { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LEDSTRIP_STVO>, ToggleBoolAction, CheckboxIcon,   EnableLedstripStVOAccessor>>(); }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STVO_ENABLEFRONTLIGHT>, ToggleBoolAction, CheckboxIcon,   EnableLedstripStVOFrontlight>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LEDSTRIP_ALLCUSTOMOFF>, AllCustomLedsOffAction>>();
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_STVO_FRONTOFFSET, LedsStVOFrontOffsetAccessor>,    SwitchScreenAction<StVOOffsetChangeScreen>>>(); }
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_STVO_FRONTLENGTH, LedsStVOFrontLengthAccessor>,    SwitchScreenAction<StVOLengthChangeScreen>>>(); }

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SELECTANIMATION>,                                 SwitchScreenAction<LedstripSelectAnimationMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLINKANIMATION>,                                  SwitchScreenAction<LedstripSelectBlinkMenu>>>();
#ifdef FEATURE_OTA
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LEDSTRIP_CHANGE_OTA_ANIM>,     SwitchScreenAction<ledstripOtaAnimationChangeMenu>>>(); }
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_MULTIPLIER>,                            SwitchScreenAction<animationMultiplierChangeScreen>>>();
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_LEDSCOUNT, LedsCountAccessor>,           SwitchScreenAction<LedsCountChangeScreen>>>(); }
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_CENTEROFFSET, CenterOffsetAccessor>,     SwitchScreenAction<CenterOffsetChangeScreen>>>(); }
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SMALLOFFSET, SmallOffsetAccessor>,       SwitchScreenAction<SmallOffsetChangeScreen>>>(); }
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BIGOFFSET, BigOffsetAccessor>,           SwitchScreenAction<BigOffsetChangeScreen>>>(); }
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LEDSTRIP_BRIGHTNESS>,                             SwitchScreenAction<ledstripBrightnessChangeScreen>>>(); }
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_LEDSTRIP_MILLIAMP, DeziampereAccessor>,  SwitchScreenAction<DeziampereChangeScreen>>>(); }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                            SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void LedstripMenu::back()
{
    switchScreen<MainMenu>();
}
#endif
