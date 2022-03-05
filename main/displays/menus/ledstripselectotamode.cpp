#include "ledstripselectotamode.h"

// 3rdparty lib includes
#include "actions/pushscreenaction.h"
#include "actions/popscreenaction.h"
#include "icons/back.h"

// local includes
#include "ledstrip.h"
#include "newsettings.h"
#include "utils.h"

#if defined(FEATURE_LEDSTRIP) && defined(FEATURE_OTA)
namespace {
constexpr char TEXT_BLINKANIMATION[] = "Blink animation";
constexpr char TEXT_OTAANIM_NONE[] = "None";
constexpr char TEXT_OTAANIM_PROGRESS[] = "Progress Bar";
constexpr char TEXT_OTAANIM_COLOR[] = "Color change";
constexpr char TEXT_BACK[] = "Back";

template <OtaAnimationModes mode>
class LedstripChangeOtaAnimModeAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        configs.write_config(configs.ledstrip.otaMode, mode);
    }
};
} // namespace

LedstripOtaAnimationChangeMenu::LedstripOtaAnimationChangeMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_OTAANIM_NONE>,          LedstripChangeOtaAnimModeAction<OtaAnimationModes::None>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_OTAANIM_PROGRESS>,      LedstripChangeOtaAnimModeAction<OtaAnimationModes::GreenProgressBar>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_OTAANIM_COLOR>,         LedstripChangeOtaAnimModeAction<OtaAnimationModes::ColorChangeAll>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                  PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string LedstripOtaAnimationChangeMenu::text() const
{
    return TEXT_BLINKANIMATION;
}

void LedstripOtaAnimationChangeMenu::back()
{
    espgui::popScreen();
}
#endif
