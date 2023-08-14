#include "controllerhardwaresettingsmenu.h"

// system includes
#include <ratio>

// 3rdparty lib includes
#include "actions/popscreenaction.h"
#include "actions/pushscreenaction.h"
#include "changevaluedisplay.h"
#include "icons/back.h"

// local includes
#include "displays/bobbychangevaluedisplay.h"
#include "utils.h"
#include "globals.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/enablemenu.h"
#include "displays/menus/invertmenu.h"
#include "displays/menus/settingsmenu.h"
#include "bobbycheckbox.h"

namespace {
constexpr char TEXT_CONTROLLERHARDWARESETTINGS[] = "Controller H/W settings";
constexpr char TEXT_WHEELDIAMETERMM[] = "Wheel diameter (mm)";
constexpr char TEXT_WHEELDIAMETERINCH[] = "Wheel diameter (inch)";
constexpr char TEXT_NUMMAGNETPOLES[] = "Num magnet poles";
constexpr char TEXT_SETENABLED[] = "Set enabled";
constexpr char TEXT_SETINVERTED[] = "Set inverted";
constexpr char TEXT_SWAPFRONTBACK[] = "Swap front/back";
#ifdef FEATURE_CAN
constexpr char TEXT_FRONTSENDCAN[] = "Front send CAN";
constexpr char TEXT_BACKSENDCAN[] = "Back send CAN";
constexpr char TEXT_CANTRANSMITTIMEOUT[] = "CanTransmitTimeout";
constexpr char TEXT_CANRECEIVETIMEOUT[] = "CanReceiveTimeout";
constexpr char TEXT_CANRESETONERROR[] = "Reset on error";
constexpr char TEXT_CANREINSTALLDRIVER[] = "Reinstall on error";
#endif
constexpr char TEXT_BACK[] = "Back";

using WheelDiameterMmChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticTitle<TEXT_WHEELDIAMETERMM>,
    WheelDiameterMmAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using WheelDiameterInchChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<float>,
    espgui::StaticTitle<TEXT_WHEELDIAMETERINCH>,
    WheelDiameterInchAccessor,
    espgui::RatioNumberStep<float, std::ratio<1,10>>,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using NumMagnetPolesChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticTitle<TEXT_NUMMAGNETPOLES>,
    NumMagnetPolesAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
#ifdef FEATURE_CAN
using CanTransmitTimeoutChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticTitle<TEXT_CANTRANSMITTIMEOUT>,
    CanTransmitTimeoutAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using CanReceiveTimeoutChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticTitle<TEXT_CANRECEIVETIMEOUT>,
    CanReceiveTimeoutAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
#endif
} // namespace

ControllerHardwareSettingsMenu::ControllerHardwareSettingsMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETENABLED>,         PushScreenAction<EnableMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETINVERTED>,        PushScreenAction<InvertMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WHEELDIAMETERMM>,    PushScreenAction<WheelDiameterMmChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WHEELDIAMETERINCH>,  PushScreenAction<WheelDiameterInchChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NUMMAGNETPOLES>,     PushScreenAction<NumMagnetPolesChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SWAPFRONTBACK>,      BobbyCheckbox, SwapFrontBackAccessor>>();
#ifdef FEATURE_CAN
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTSENDCAN>,       BobbyCheckbox, SendFrontCanCmdAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKSENDCAN>,        BobbyCheckbox, SendBackCanCmdAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CANTRANSMITTIMEOUT>, PushScreenAction<CanTransmitTimeoutChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CANRECEIVETIMEOUT>,  PushScreenAction<CanReceiveTimeoutChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CANRESETONERROR>,    BobbyCheckbox, CanResetOnErrorAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CANREINSTALLDRIVER>, BobbyCheckbox, CanReinstallDriverAccessor>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,               PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string ControllerHardwareSettingsMenu::title() const
{
    return TEXT_CONTROLLERHARDWARESETTINGS;
}

void ControllerHardwareSettingsMenu::back()
{
    espgui::popScreen();
}
