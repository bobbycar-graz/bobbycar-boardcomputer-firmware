#include "lockscreensettingsmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay.h>
#include <menuitem.h>
#include <textwithvaluehelper.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "globals.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"

namespace bobby {

namespace {
constexpr char TEXT_LOCKSCREENSETTINGS[] = "Lockscreen Settings";
constexpr char TEXT_ALLOWPRESETSWITCH[] = "Allow preset switch";
constexpr char TEXT_KEEPLOCKED[] = "Keep locked";
constexpr char TEXT_PINDIGIT0[] = "PIN digit0";
constexpr char TEXT_PINDIGIT1[] = "PIN digit1";
constexpr char TEXT_PINDIGIT2[] = "PIN digit2";
constexpr char TEXT_PINDIGIT3[] = "PIN digit3";
constexpr char TEXT_BACK[] = "Back";

using LockscreenPinDigit0ChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int8_t>,
    espgui::StaticText<TEXT_PINDIGIT0>,
    LockscreenPinDigitAccessor<0>,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using LockscreenPinDigit1ChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int8_t>,
    espgui::StaticText<TEXT_PINDIGIT1>,
    LockscreenPinDigitAccessor<1>,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using LockscreenPinDigit2ChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int8_t>,
    espgui::StaticText<TEXT_PINDIGIT2>,
    LockscreenPinDigitAccessor<2>,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using LockscreenPinDigit3ChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int8_t>,
    espgui::StaticText<TEXT_PINDIGIT3>,
    LockscreenPinDigitAccessor<3>,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

LockscreenSettingsMenu::LockscreenSettingsMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ALLOWPRESETSWITCH>, BobbyCheckbox, LockscreenAllowPresetSwitchAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_KEEPLOCKED>, BobbyCheckbox, LockscreenKeepLockedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_PINDIGIT0, LockscreenPinDigitAccessor<0>>, PushScreenAction<LockscreenPinDigit0ChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_PINDIGIT1, LockscreenPinDigitAccessor<1>>, PushScreenAction<LockscreenPinDigit1ChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_PINDIGIT2, LockscreenPinDigitAccessor<2>>, PushScreenAction<LockscreenPinDigit2ChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_PINDIGIT3, LockscreenPinDigitAccessor<3>>, PushScreenAction<LockscreenPinDigit3ChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                              PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string LockscreenSettingsMenu::text() const
{
    return TEXT_LOCKSCREENSETTINGS;
}

void LockscreenSettingsMenu::back()
{
    espgui::popScreen();
}
} // namespace bobby
