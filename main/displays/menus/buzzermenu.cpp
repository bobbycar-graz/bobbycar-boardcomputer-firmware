#include "buzzermenu.h"

// 3rdparty lib includes
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "actions/toggleboolaction.h"
#include "checkboxicon.h"
#include "icons/back.h"

// local includes
#include "utilities/utils.h"
#include "globals.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/settingsmenu.h"

namespace {
struct FrontFreqAccessor : public espgui::RefAccessor<uint8_t> { uint8_t &getRef() const override { return controllers.front.command.buzzer.freq; } };
using FrontFreqChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_FRONTFREQ>,
    FrontFreqAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BuzzerMenu>>,
    espgui::SwitchScreenAction<BuzzerMenu>
>;

struct FrontPatternAccessor : public espgui::RefAccessor<uint8_t> { uint8_t &getRef() const override { return controllers.front.command.buzzer.pattern; } };
using FrontPatternChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_FRONTPATTERN>,
    FrontPatternAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BuzzerMenu>>,
    espgui::SwitchScreenAction<BuzzerMenu>
>;

struct BackFreqAccessor : public espgui::RefAccessor<uint8_t> { uint8_t &getRef() const override { return controllers.back.command.buzzer.freq; } };
using BackFreqChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_BACKFREQ>,
    BackFreqAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BuzzerMenu>>,
    espgui::SwitchScreenAction<BuzzerMenu>
>;

struct BackPatternAccessor : public espgui::RefAccessor<uint8_t> { uint8_t &getRef() const override { return controllers.back.command.buzzer.pattern; } };
using BackPatternChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_BACKPATTERN>,
    BackPatternAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BuzzerMenu>>,
    espgui::SwitchScreenAction<BuzzerMenu>
>;

using ReverseBeepFreq0ChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_REVERSEBEEPFREQ0>,
    ReverseBeepFreq0Accessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BuzzerMenu>>,
    espgui::SwitchScreenAction<BuzzerMenu>
>;
using ReverseBeepFreq1ChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_REVERSEBEEPFREQ1>,
    ReverseBeepFreq1Accessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BuzzerMenu>>,
    espgui::SwitchScreenAction<BuzzerMenu>
>;
using ReverseBeepDuration0ChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_REVERSEBEEPDURATION0>,
    ReverseBeepDuration0Accessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BuzzerMenu>>,
    espgui::SwitchScreenAction<BuzzerMenu>
>;
using ReverseBeepDuration1ChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_REVERSEBEEPDURATION1>,
    ReverseBeepDuration1Accessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<BuzzerMenu>>,
    espgui::SwitchScreenAction<BuzzerMenu>
>;
} // namespace

using namespace espgui;

BuzzerMenu::BuzzerMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTFREQ>,            SwitchScreenAction<FrontFreqChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTPATTERN>,         SwitchScreenAction<FrontPatternChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKFREQ>,             SwitchScreenAction<BackFreqChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKPATTERN>,          SwitchScreenAction<BackPatternChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REVERSEBEEP>,          ToggleBoolAction, CheckboxIcon, ReverseBeepAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REVERSEBEEPFREQ0>,     SwitchScreenAction<ReverseBeepFreq0ChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REVERSEBEEPFREQ1>,     SwitchScreenAction<ReverseBeepFreq1ChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REVERSEBEEPDURATION0>, SwitchScreenAction<ReverseBeepDuration0ChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REVERSEBEEPDURATION1>, SwitchScreenAction<ReverseBeepDuration1ChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                 SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void BuzzerMenu::back()
{
    switchScreen<SettingsMenu>();
}
