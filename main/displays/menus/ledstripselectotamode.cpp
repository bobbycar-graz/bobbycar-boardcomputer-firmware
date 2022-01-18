#include "ledstripselectotamode.h"

// 3rdparty lib includes
#include "actions/switchscreenaction.h"
#include "icons/back.h"

// local includes
#include "ledstrip.h"
#include "newsettings.h"
#include "utils.h"
#include "ledstripmenu.h"

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
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                  SwitchScreenAction<LedstripMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string LedstripOtaAnimationChangeMenu::text() const
{
    return TEXT_BLINKANIMATION;
}

void LedstripOtaAnimationChangeMenu::back()
{
    espgui::switchScreen<LedstripMenu>();
}
