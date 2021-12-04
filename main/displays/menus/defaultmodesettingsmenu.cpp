#include "defaultmodesettingsmenu.h"

// 3rdparty lib includes
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/toggleboolaction.h"
#include "actions/switchscreenaction.h"
#include "actions/dummyaction.h"
#include "checkboxicon.h"
#include "icons/back.h"

// local includes
#include "utils.h"
#include "changevaluedisplay_unifiedmodelmode.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/handbremssettingsmenu.h"
#include "displays/menus/modessettingsmenu.h"

namespace {
using DefaultModeModelModeChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<UnifiedModelMode>,
    espgui::StaticText<TEXT_MODELMODE>,
    DefaultModeModelModeAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeHybridModelModeChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<UnifiedModelMode>,
    espgui::StaticText<TEXT_HYBRIDMODE>,
    DefaultModeHybridModelModeAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeSmoothingChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SMOOTHINGVAL>,
    DefaultModeSmoothingAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeFwSmoothingLowerLimitChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_FWSMOOTHING_LIMIT>,
    DefaultModeEnableFieldWeakSmoothingLowerLimitAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeFrontPercentageChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_FRONTPERCENTAGE>,
    DefaultModeFrontPercentageAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeBackPercentageChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_BACKPERCENTAGE>,
    DefaultModeBackPercentageAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeAddSchwelleChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_ADDSCHWELLE>,
    DefaultModeAddSchwelleAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeGas1WertChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_ADDGASVAL>,
    DefaultModeGas1WertAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeGas2WertChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SUBGASVAL>,
    DefaultModeGas2WertAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeBrems1WertChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_ADDBRAKEVAL>,
    DefaultModeBrems1WertAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeBrems2WertChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_SUBBRAKEVAL>,
    DefaultModeBrems2WertAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeHybridActivationLimitChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_HYBRIDACTIVATIONLIMIT>,
    DefaultModeHybridActivationLimitAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeHybridDeactivationLimitChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_HYBRIDDEACTIVATIONLIMIT>,
    DefaultModeHybridDeactivationLimitAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<DefaultModeSettingsMenu>>,
    espgui::SwitchScreenAction<DefaultModeSettingsMenu>
>;
} // namespace

using namespace espgui;

DefaultModeSettingsMenu::DefaultModeSettingsMenu()
{
    auto diff = std::abs(settings.hybrid.activationLimit - settings.hybrid.deactivationLimit);
    if (diff < 20) { constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LIMITS_TO_NEAR>, StaticFont<2>, StaticColor<TFT_RED>, DummyAction>>(); }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_MODELMODE>,          SwitchScreenAction<DefaultModeModelModeChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_HYBRIDMODE>,         SwitchScreenAction<DefaultModeHybridModelModeChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SQUAREGAS>,          ToggleBoolAction, CheckboxIcon,  DefaultModeSquareGasAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SQUAREBREMS>,        ToggleBoolAction, CheckboxIcon,  DefaultModeSquareBremsAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLESMOOTHINGUP>,    ToggleBoolAction, CheckboxIcon,  DefaultModeEnableSmoothingUpAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLESMOOTHINGDOWN>,    ToggleBoolAction, CheckboxIcon,  DefaultModeEnableSmoothingDownAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLEFWSMOOTHINGUP>,  ToggleBoolAction, CheckboxIcon,  DefaultModeEnableFieldWeakSmoothingUpAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLEFWSMOOTHINGDOWN>,  ToggleBoolAction, CheckboxIcon,  DefaultModeEnableFieldWeakSmoothingDownAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_HYBRIDENABLE>,  ToggleBoolAction, CheckboxIcon,  DefaultModeEnableHybridAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_HANDBREMSE>, SwitchScreenAction<HandbremsSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_FWSMOOTHING_LIMIT, DefaultModeEnableFieldWeakSmoothingLowerLimitAccessor>, SwitchScreenAction<DefaultModeFwSmoothingLowerLimitChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SMOOTHINGVAL, DefaultModeSmoothingAccessor>,                     SwitchScreenAction<DefaultModeSmoothingChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_FRONTPERCENTAGE, DefaultModeFrontPercentageAccessor>, SwitchScreenAction<DefaultModeFrontPercentageChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_BACKPERCENTAGE, DefaultModeBackPercentageAccessor>,   SwitchScreenAction<DefaultModeBackPercentageChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_ADDSCHWELLE, DefaultModeAddSchwelleAccessor>,                    SwitchScreenAction<DefaultModeAddSchwelleChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SUBGASVAL, DefaultModeGas2WertAccessor>,                         SwitchScreenAction<DefaultModeGas2WertChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SUBBRAKEVAL, DefaultModeBrems2WertAccessor>,                     SwitchScreenAction<DefaultModeBrems2WertChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_ADDGASVAL, DefaultModeGas1WertAccessor>,                         SwitchScreenAction<DefaultModeGas1WertChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_ADDBRAKEVAL, DefaultModeBrems1WertAccessor>,                     SwitchScreenAction<DefaultModeBrems1WertChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_HYBRIDACTIVATIONLIMIT, DefaultModeHybridActivationLimitAccessor>,        SwitchScreenAction<DefaultModeHybridActivationLimitChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_HYBRIDDEACTIVATIONLIMIT, DefaultModeHybridDeactivationLimitAccessor>,    SwitchScreenAction<DefaultModeHybridDeactivationLimitChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                                                    SwitchScreenAction<ModesSettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void DefaultModeSettingsMenu::back()
{
    switchScreen<ModesSettingsMenu>();
}
