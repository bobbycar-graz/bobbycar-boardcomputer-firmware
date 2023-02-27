#include "buzzermenu.h"

// 3rdparty lib includes
#include <changevaluedisplay.h>
#include <menuitem.h>
#include <actions/pushscreenaction.h>
#include <actions/popscreenaction.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "globals.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"

namespace bobby {

namespace {
constexpr char TEXT_BUZZER[] = "Buzzer";
constexpr char TEXT_FRONTFREQ[] = "Front freq";
constexpr char TEXT_FRONTPATTERN[] = "Front pattern";
constexpr char TEXT_BACKFREQ[] = "Back freq";
constexpr char TEXT_BACKPATTERN[] = "Back pattern";
constexpr char TEXT_REVERSEBEEP[] = "Reverse beep";
constexpr char TEXT_REVERSEBEEPFREQ0[] = "Reverse beep freq0";
constexpr char TEXT_REVERSEBEEPFREQ1[] = "Reverse beep freq1";
constexpr char TEXT_REVERSEBEEPDURATION0[] = "Reverse beep duration0";
constexpr char TEXT_REVERSEBEEPDURATION1[] = "Reverse beep duration1";
constexpr char TEXT_BACK[] = "Back";

struct FrontFreqAccessor : public espgui::RefAccessor<uint8_t> { uint8_t &getRef() const override { return controllers.front.command.buzzer.freq; } };
using FrontFreqChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_FRONTFREQ>,
    FrontFreqAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

struct FrontPatternAccessor : public espgui::RefAccessor<uint8_t> { uint8_t &getRef() const override { return controllers.front.command.buzzer.pattern; } };
using FrontPatternChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_FRONTPATTERN>,
    FrontPatternAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

struct BackFreqAccessor : public espgui::RefAccessor<uint8_t> { uint8_t &getRef() const override { return controllers.back.command.buzzer.freq; } };
using BackFreqChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_BACKFREQ>,
    BackFreqAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

struct BackPatternAccessor : public espgui::RefAccessor<uint8_t> { uint8_t &getRef() const override { return controllers.back.command.buzzer.pattern; } };
using BackPatternChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_BACKPATTERN>,
    BackPatternAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using ReverseBeepFreq0ChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_REVERSEBEEPFREQ0>,
    ReverseBeepFreq0Accessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using ReverseBeepFreq1ChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_REVERSEBEEPFREQ1>,
    ReverseBeepFreq1Accessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using ReverseBeepDuration0ChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_REVERSEBEEPDURATION0>,
    ReverseBeepDuration0Accessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using ReverseBeepDuration1ChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_REVERSEBEEPDURATION1>,
    ReverseBeepDuration1Accessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

using namespace espgui;

BuzzerMenu::BuzzerMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTFREQ>,            PushScreenAction<FrontFreqChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTPATTERN>,         PushScreenAction<FrontPatternChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKFREQ>,             PushScreenAction<BackFreqChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKPATTERN>,          PushScreenAction<BackPatternChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REVERSEBEEP>,          BobbyCheckbox, ReverseBeepAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REVERSEBEEPFREQ0>,     PushScreenAction<ReverseBeepFreq0ChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REVERSEBEEPFREQ1>,     PushScreenAction<ReverseBeepFreq1ChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REVERSEBEEPDURATION0>, PushScreenAction<ReverseBeepDuration0ChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REVERSEBEEPDURATION1>, PushScreenAction<ReverseBeepDuration1ChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                 PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string BuzzerMenu::text() const
{
    return TEXT_BUZZER;
}

void BuzzerMenu::back()
{
    popScreen();
}
} // namespace bobinette
