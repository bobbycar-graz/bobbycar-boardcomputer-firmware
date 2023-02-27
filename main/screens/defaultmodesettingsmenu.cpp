#include "defaultmodesettingsmenu.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay.h>
#include <menuitem.h>
#include <textwithvaluehelper.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "changevaluedisplay_unifiedmodelmode.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"
#include "screens/handbremssettingsmenu.h"
#include "utils.h"

namespace bobby {

namespace {
constexpr char TEXT_DEFAULTMODESETTIGNS[] = "Default mode settings";
constexpr char TEXT_MODELMODE[] = "Model mode";
constexpr char TEXT_ALLOWREMOTECONTROL[] = "Allow remote control";
constexpr char TEXT_SQUAREGAS[] = "Square gas";
constexpr char TEXT_SQUAREBREMS[] = "Square brems";
constexpr char TEXT_ENABLESMOOTHINGUP[] = "Enable up smoothing";
constexpr char TEXT_ENABLESMOOTHINGDOWN[] = "Enable down smoothing";
constexpr char TEXT_ENABLEFWSMOOTHINGUP[] = "Enable fw up smoothing";
constexpr char TEXT_ENABLEFWSMOOTHINGDOWN[] = "Enable fw down smoothing";
constexpr char TEXT_FWSMOOTHING_LIMIT[] = "Fw smooth lower limit";
constexpr char TEXT_SMOOTHINGVAL[] = "Smoothing";
constexpr char TEXT_FRONTPERCENTAGE[] = "Front %";
constexpr char TEXT_BACKPERCENTAGE[] = "Back %";
constexpr char TEXT_ADDSCHWELLE[] = "Add/Sub Lim";
constexpr char TEXT_SUBGASVAL[] = "Add Gas";
constexpr char TEXT_SUBBRAKEVAL[] = "Add Brake";
constexpr char TEXT_ADDGASVAL[] = "Sub Gas";
constexpr char TEXT_ADDBRAKEVAL[] = "Sub Brake";
constexpr char TEXT_HANDBREMSE_ENABLE[] = "Enable Handbremse";
constexpr char TEXT_HANDBREMSE_AUTOMATIC[] = "Automatic Handbremse";
constexpr char TEXT_HANDBREMSE_VISUALIZE[] = "Visualize Handbremse";
constexpr char TEXT_HANDBREMSE_MODE[] = "Handbrems Mode";
constexpr char TEXT_HANDBREMSE_TRIGGERTIMEOUT[] = "Handbrems Timeout";
constexpr char TEXT_HANDBREMSE[] = "Handbremse";
constexpr char TEXT_BACK[] = "Back";

using DefaultModeModelModeChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<UnifiedModelMode>,
    espgui::StaticText<TEXT_MODELMODE>,
    DefaultModeModelModeAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using DefaultModeSmoothingChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SMOOTHINGVAL>,
    DefaultModeSmoothingAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using DefaultModeFwSmoothingLowerLimitChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_FWSMOOTHING_LIMIT>,
    DefaultModeEnableFieldWeakSmoothingLowerLimitAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using DefaultModeFrontPercentageChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_FRONTPERCENTAGE>,
    DefaultModeFrontPercentageAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using DefaultModeBackPercentageChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_BACKPERCENTAGE>,
    DefaultModeBackPercentageAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using DefaultModeAddSchwelleChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_ADDSCHWELLE>,
    DefaultModeAddSchwelleAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using DefaultModeGas1WertChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_ADDGASVAL>,
    DefaultModeGas1WertAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using DefaultModeGas2WertChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SUBGASVAL>,
    DefaultModeGas2WertAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using DefaultModeBrems1WertChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_ADDBRAKEVAL>,
    DefaultModeBrems1WertAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using DefaultModeBrems2WertChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SUBBRAKEVAL>,
    DefaultModeBrems2WertAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

using namespace espgui;

DefaultModeSettingsMenu::DefaultModeSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODELMODE>,          PushScreenAction<DefaultModeModelModeChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ALLOWREMOTECONTROL>, BobbyCheckbox,  DefaultModeAllowRemoteControlAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SQUAREGAS>,          BobbyCheckbox,  DefaultModeSquareGasAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SQUAREBREMS>,        BobbyCheckbox,  DefaultModeSquareBremsAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLESMOOTHINGUP>,    BobbyCheckbox,  DefaultModeEnableSmoothingUpAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLESMOOTHINGDOWN>,    BobbyCheckbox,  DefaultModeEnableSmoothingDownAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLEFWSMOOTHINGUP>,  BobbyCheckbox,  DefaultModeEnableFieldWeakSmoothingUpAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLEFWSMOOTHINGDOWN>,  BobbyCheckbox,  DefaultModeEnableFieldWeakSmoothingDownAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_HANDBREMSE>,            PushScreenAction<HandbremsSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_FWSMOOTHING_LIMIT, DefaultModeEnableFieldWeakSmoothingLowerLimitAccessor>, PushScreenAction<DefaultModeFwSmoothingLowerLimitChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SMOOTHINGVAL, DefaultModeSmoothingAccessor>,                     PushScreenAction<DefaultModeSmoothingChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_FRONTPERCENTAGE, DefaultModeFrontPercentageAccessor>, PushScreenAction<DefaultModeFrontPercentageChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BACKPERCENTAGE, DefaultModeBackPercentageAccessor>,   PushScreenAction<DefaultModeBackPercentageChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_ADDSCHWELLE, DefaultModeAddSchwelleAccessor>,                    PushScreenAction<DefaultModeAddSchwelleChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SUBGASVAL, DefaultModeGas2WertAccessor>,                         PushScreenAction<DefaultModeGas2WertChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SUBBRAKEVAL, DefaultModeBrems2WertAccessor>,                     PushScreenAction<DefaultModeBrems2WertChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_ADDGASVAL, DefaultModeGas1WertAccessor>,                         PushScreenAction<DefaultModeGas1WertChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_ADDBRAKEVAL, DefaultModeBrems1WertAccessor>,                     PushScreenAction<DefaultModeBrems1WertChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                                                    PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string DefaultModeSettingsMenu::text() const
{
    return TEXT_DEFAULTMODESETTIGNS;
}

void DefaultModeSettingsMenu::back()
{
    popScreen();
}
} // namespace bobby
