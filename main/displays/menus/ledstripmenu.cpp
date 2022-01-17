#include "ledstripmenu.h"

// 3rdparty lib includes
#include <FastLED.h>
#include <actioninterface.h>
#include <actions/switchscreenaction.h>
#include <changevaluedisplay.h>
#include <icons/back.h>
#include <menuitem.h>
#include <textwithvaluehelper.h>

// local includes
#include "displays/bobbychangevaluedisplay.h"
#include "accessors/settingsaccessors.h"
#include "displays/ledstripcolorsdisplay.h"
#include "displays/menus/ledstripselectotamode.h"
#include "displays/menus/mainmenu.h"
#include "globals.h"
#include "ledstrip.h"
#include "ledstripselectanimationmenu.h"
#include "ledstripselectblinkmenu.h"
#include "bobbycheckbox.h"

// clang-format off

namespace {
constexpr char TEXT_LEDSTRIP[] = "Ledstrip";
constexpr char TEXT_LEDANIMATION[] = "LED Animation";
constexpr char TEXT_SELECTANIMATION[] = "Select Animation";
constexpr char TEXT_BRAKELIGHTS[] = "Brake Lights";
constexpr char TEXT_LEDSTRIPCOLORMENU[] = "Customize Ledstrip";
constexpr char TEXT_BLINKANIMATION[] = "Blink animation";
constexpr char TEXT_ANIMATION_TYPE[] = "Blink animation";
constexpr char TEXT_LEDSCOUNT[] = "LEDs Count";
constexpr char TEXT_CENTEROFFSET[] = "Center Offset";
constexpr char TEXT_SMALLOFFSET[] = "Small Offset";
constexpr char TEXT_BIGOFFSET[] = "Big Offset";
constexpr char TEXT_LEDSTRIPCURRENTLIMIT[] = "Ledstrip Currentlimit";
constexpr char TEXT_BLINKBEEP[] = "Blink Beep";
constexpr char TEXT_FULLBLINK[] = "Full blink";
constexpr char TEXT_LEDSTRIP_STVO[] = "Enable StVO";
constexpr char TEXT_STVO_FRONTOFFSET[] = "StVO Front Offset";
constexpr char TEXT_STVO_FRONTLENGTH[] = "StVO Front Length";
constexpr char TEXT_STVO_ENABLEFRONTLIGHT[] = "StVO Front Enable";
constexpr char TEXT_ANIMATION_MULTIPLIER[] = "Animation Multiplier";
constexpr char TEXT_LEDSTRIP_BRIGHTNESS[] = "Ledstrip Brightness";
constexpr char TEXT_LEDSTRIP_ALLCUSTOMOFF[] = "All custom off";
constexpr char TEXT_LEDSTRIP_CHANGE_OTA_ANIM[] = "Change Ota animation";
constexpr char TEXT_BACK[] = "Back";

using LedsCountChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_LEDSCOUNT>,
    LedsCountAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<LedstripMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LedstripMenu>>
>;

using CenterOffsetChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_CENTEROFFSET>,
    CenterOffsetAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<LedstripMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LedstripMenu>>
>;

using SmallOffsetChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SMALLOFFSET>,
    SmallOffsetAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<LedstripMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LedstripMenu>>
>;

using BigOffsetChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_BIGOFFSET>,
    BigOffsetAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<LedstripMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LedstripMenu>>
>;

using LedStripMaxAmpereChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<float>,
    espgui::StaticText<TEXT_LEDSTRIPCURRENTLIMIT>,
    LedStripMaxAmpereAccessor,
    espgui::RatioNumberStep<float, std::ratio<1,10>>,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<LedstripMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LedstripMenu>>
>;

using StVOOffsetChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_STVO_FRONTOFFSET>,
    LedsStVOFrontOffsetAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<LedstripMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LedstripMenu>>
>;

using StVOLengthChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_STVO_FRONTLENGTH>,
    LedsStVOFrontLengthAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<LedstripMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LedstripMenu>>
>;

using AnimationMultiplierChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_ANIMATION_MULTIPLIER>,
    AnimationMultiplierAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<LedstripMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LedstripMenu>>
>;

using LedStripBrightnessChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_LEDSTRIP_BRIGHTNESS>,
    LedstripBrightnessAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<LedstripMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LedstripMenu>>
>;

class AllCustomLedsOffAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        // std::fill(std::begin(ledstrip_custom_colors), std::end(ledstrip_custom_colors), CRGB{0, 0, 0});
        for (uint8_t i = 0; i < configs.ledstrip.custom_color.size(); i++)
        {
            configs.write_config(configs.ledstrip.custom_color[i], 0);
        }
    }
};

class LedStripMaxCurrentText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("&sLedstrip max current: &f&2{:.02f}A", configs.ledstrip.maxMilliamps.value / 1000.f);
    }
};
} // namespace

LedstripMenu::LedstripMenu()
{
    using namespace espgui;

    if (!simplified) { constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_LEDSTRIP_STVO>, BobbyCheckbox, EnableLedstripStVOAccessor>>(); }
    constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_STVO_ENABLEFRONTLIGHT>, BobbyCheckbox, EnableLedstripStVOFrontlight>>();

    constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_LEDANIMATION>, BobbyCheckbox, EnableLedAnimationAccessor>>();
    constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_SELECTANIMATION>,                                 espgui::SwitchScreenAction<LedstripSelectAnimationMenu>>>();

    constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_BRAKELIGHTS>,  BobbyCheckbox,    EnableBrakeLightsAccessor>>();

    constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_LEDSTRIPCOLORMENU>, espgui::SwitchScreenAction<LedstripColorsDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_LEDSTRIP_ALLCUSTOMOFF>, AllCustomLedsOffAction>>();

    constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_BLINKBEEP>, BobbyCheckbox,       EnableBeepWhenBlinkAccessor>>();
    constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_FULLBLINK>, BobbyCheckbox,       EnableFullBlinkAccessor>>();

    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_STVO_FRONTOFFSET, LedsStVOFrontOffsetAccessor>,    espgui::SwitchScreenAction<StVOOffsetChangeScreen>>>(); }
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_STVO_FRONTLENGTH, LedsStVOFrontLengthAccessor>,    espgui::SwitchScreenAction<StVOLengthChangeScreen>>>(); }

    constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_BLINKANIMATION>,                                  espgui::SwitchScreenAction<LedstripSelectBlinkMenu>>>();
#ifdef FEATURE_OTA
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_LEDSTRIP_CHANGE_OTA_ANIM>,     espgui::SwitchScreenAction<LedstripOtaAnimationChangeMenu>>>(); }
#endif
    constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_ANIMATION_MULTIPLIER>,                            espgui::SwitchScreenAction<AnimationMultiplierChangeScreen>>>();
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_LEDSCOUNT, LedsCountAccessor>,           espgui::SwitchScreenAction<LedsCountChangeScreen>>>(); }
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_CENTEROFFSET, CenterOffsetAccessor>,     espgui::SwitchScreenAction<CenterOffsetChangeScreen>>>(); }
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SMALLOFFSET, SmallOffsetAccessor>,       espgui::SwitchScreenAction<SmallOffsetChangeScreen>>>(); }
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BIGOFFSET, BigOffsetAccessor>,           espgui::SwitchScreenAction<BigOffsetChangeScreen>>>(); }
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_LEDSTRIP_BRIGHTNESS>,                             espgui::SwitchScreenAction<LedStripBrightnessChangeScreen>>>(); }
    if (!simplified) { constructMenuItem<makeComponent<MenuItem, LedStripMaxCurrentText, espgui::SwitchScreenAction<LedStripMaxAmpereChangeScreen>>>(); }
    constructMenuItem<makeComponent<MenuItem, espgui::StaticText<TEXT_BACK>,                                            espgui::SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string LedstripMenu::text() const
{
    return TEXT_LEDSTRIP;
}

void LedstripMenu::back()
{
    espgui::switchScreen<MainMenu>();
}
