#include "controllerhardwaresettingsmenu.h"

// system includes
#include <ratio>

// 3rdparty lib includes
#include "changevaluedisplay.h"
#include "actions/toggleboolaction.h"
#include "actions/switchscreenaction.h"
#include "checkboxicon.h"
#include "icons/back.h"

// local includes
#include "utilities/utils.h"
#include "globals.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/enablemenu.h"
#include "displays/menus/invertmenu.h"
#include "displays/menus/settingsmenu.h"

namespace {
using WheelDiameterMmChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_WHEELDIAMETERMM>,
    WheelDiameterMmAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<ControllerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<ControllerHardwareSettingsMenu>
>;
using WheelDiameterInchChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<float>,
    espgui::StaticText<TEXT_WHEELDIAMETERINCH>,
    WheelDiameterInchAccessor,
    espgui::RatioNumberStep<float, std::ratio<1,10>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<ControllerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<ControllerHardwareSettingsMenu>
>;
using NumMagnetPolesChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_NUMMAGNETPOLES>,
    NumMagnetPolesAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<ControllerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<ControllerHardwareSettingsMenu>
>;
#ifdef FEATURE_CAN
using CanTransmitTimeoutChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_CANTRANSMITTIMEOUT>,
    CanTransmitTimeoutAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<ControllerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<ControllerHardwareSettingsMenu>
>;
using CanReceiveTimeoutChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_CANRECEIVETIMEOUT>,
    CanReceiveTimeoutAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<ControllerHardwareSettingsMenu>>,
    espgui::SwitchScreenAction<ControllerHardwareSettingsMenu>
>;
#endif
} // namespace

using namespace espgui;

ControllerHardwareSettingsMenu::ControllerHardwareSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETENABLED>,        SwitchScreenAction<EnableMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETINVERTED>,       SwitchScreenAction<InvertMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WHEELDIAMETERMM>,   SwitchScreenAction<WheelDiameterMmChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WHEELDIAMETERINCH>, SwitchScreenAction<WheelDiameterInchChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NUMMAGNETPOLES>,    SwitchScreenAction<NumMagnetPolesChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SWAPFRONTBACK>,     ToggleBoolAction, CheckboxIcon, SwapFrontBackAccessor>>();
#ifdef FEATURE_CAN
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTSENDCAN>,       ToggleBoolAction, CheckboxIcon, SendFrontCanCmdAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKSENDCAN>,        ToggleBoolAction, CheckboxIcon, SendBackCanCmdAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CANTRANSMITTIMEOUT>, SwitchScreenAction<CanTransmitTimeoutChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CANRECEIVETIMEOUT>,  SwitchScreenAction<CanReceiveTimeoutChangeScreen>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,              SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void ControllerHardwareSettingsMenu::back()
{
    switchScreen<SettingsMenu>();
}
