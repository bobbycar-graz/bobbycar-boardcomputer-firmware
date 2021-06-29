#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "actions/toggleboolaction.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "texts.h"
#include "globals.h"
#include "settingsaccessors.h"

// forward declares
namespace {
class BuzzerMenu;
class SettingsMenu;
} // namespace

namespace {
struct FrontFreqAccessor : public RefAccessor<uint8_t> { uint8_t &getRef() const override { return controllers.front.command.buzzer.freq; } };
using FrontFreqChangeScreen = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_FRONTFREQ>,
    FrontFreqAccessor,
    BackActionInterface<SwitchScreenAction<BuzzerMenu>>,
    SwitchScreenAction<BuzzerMenu>
>;

struct FrontPatternAccessor : public RefAccessor<uint8_t> { uint8_t &getRef() const override { return controllers.front.command.buzzer.pattern; } };
using FrontPatternChangeScreen = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_FRONTPATTERN>,
    FrontPatternAccessor,
    BackActionInterface<SwitchScreenAction<BuzzerMenu>>,
    SwitchScreenAction<BuzzerMenu>
>;

struct BackFreqAccessor : public RefAccessor<uint8_t> { uint8_t &getRef() const override { return controllers.back.command.buzzer.freq; } };
using BackFreqChangeScreen = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_BACKFREQ>,
    BackFreqAccessor,
    BackActionInterface<SwitchScreenAction<BuzzerMenu>>,
    SwitchScreenAction<BuzzerMenu>
>;

struct BackPatternAccessor : public RefAccessor<uint8_t> { uint8_t &getRef() const override { return controllers.back.command.buzzer.pattern; } };
using BackPatternChangeScreen = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_BACKPATTERN>,
    BackPatternAccessor,
    BackActionInterface<SwitchScreenAction<BuzzerMenu>>,
    SwitchScreenAction<BuzzerMenu>
>;

using ReverseBeepFreq0ChangeScreen = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_REVERSEBEEPFREQ0>,
    ReverseBeepFreq0Accessor,
    BackActionInterface<SwitchScreenAction<BuzzerMenu>>,
    SwitchScreenAction<BuzzerMenu>
>;
using ReverseBeepFreq1ChangeScreen = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_REVERSEBEEPFREQ1>,
    ReverseBeepFreq1Accessor,
    BackActionInterface<SwitchScreenAction<BuzzerMenu>>,
    SwitchScreenAction<BuzzerMenu>
>;
using ReverseBeepDuration0ChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_REVERSEBEEPDURATION0>,
    ReverseBeepDuration0Accessor,
    BackActionInterface<SwitchScreenAction<BuzzerMenu>>,
    SwitchScreenAction<BuzzerMenu>
>;
using ReverseBeepDuration1ChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_REVERSEBEEPDURATION1>,
    ReverseBeepDuration1Accessor,
    BackActionInterface<SwitchScreenAction<BuzzerMenu>>,
    SwitchScreenAction<BuzzerMenu>
>;

class BuzzerMenu :
    public MenuDisplay,
    public StaticText<TEXT_BUZZER>,
    public BackActionInterface<SwitchScreenAction<SettingsMenu>>
{
public:
    BuzzerMenu()
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
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                 SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
} // namespace
