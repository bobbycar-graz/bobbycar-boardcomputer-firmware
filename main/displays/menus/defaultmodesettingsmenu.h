#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/toggleboolaction.h"
#include "actions/switchscreenaction.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "texts.h"
#include "settingsaccessors.h"

// forward declares
namespace {
class DefaultModeSettingsMenu;
class ModesSettingsMenu;
} // namespace

namespace {
using DefaultModeModelModeChangeDisplay = makeComponent<
    ChangeValueDisplay<UnifiedModelMode>,
    StaticText<TEXT_SETMODELMODE>,
    DefaultModeModelModeAccessor,
    BackActionInterface<SwitchScreenAction<DefaultModeSettingsMenu>>,
    SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeSmoothingChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETSMOOTHING>,
    DefaultModeSmoothingAccessor,
    BackActionInterface<SwitchScreenAction<DefaultModeSettingsMenu>>,
    SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeFrontPercentageChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETFRONTPERCENTAGE>,
    DefaultModeFrontPercentageAccessor,
    BackActionInterface<SwitchScreenAction<DefaultModeSettingsMenu>>,
    SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeBackPercentageChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETBACKPERCENTAGE>,
    DefaultModeBackPercentageAccessor,
    BackActionInterface<SwitchScreenAction<DefaultModeSettingsMenu>>,
    SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeAddSchwelleChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETADDSCHWELLE>,
    DefaultModeAddSchwelleAccessor,
    BackActionInterface<SwitchScreenAction<DefaultModeSettingsMenu>>,
    SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeGas1WertChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETGAS1WERT>,
    DefaultModeGas1WertAccessor,
    BackActionInterface<SwitchScreenAction<DefaultModeSettingsMenu>>,
    SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeGas2WertChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETGAS2WERT>,
    DefaultModeGas2WertAccessor,
    BackActionInterface<SwitchScreenAction<DefaultModeSettingsMenu>>,
    SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeBrems1WertChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETBREMS1WERT>,
    DefaultModeBrems1WertAccessor,
    BackActionInterface<SwitchScreenAction<DefaultModeSettingsMenu>>,
    SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeBrems2WertChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETBREMS2WERT>,
    DefaultModeBrems2WertAccessor,
    BackActionInterface<SwitchScreenAction<DefaultModeSettingsMenu>>,
    SwitchScreenAction<DefaultModeSettingsMenu>
>;

class DefaultModeSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_DEFAULTMODESETTIGNS>,
    public BackActionInterface<SwitchScreenAction<ModesSettingsMenu>>
{
public:
    DefaultModeSettingsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETMODELMODE>,       SwitchScreenAction<DefaultModeModelModeChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SQUAREGAS>,          ToggleBoolAction, CheckboxIcon, DefaultModeSquareGasAccessor>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SQUAREBREMS>,        ToggleBoolAction, CheckboxIcon, DefaultModeSquareBremsAccessor>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLESMOOTHING>,    ToggleBoolAction, CheckboxIcon, DefaultModeEnableSmoothingAccessor>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETSMOOTHING>,       SwitchScreenAction<DefaultModeSmoothingChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETFRONTPERCENTAGE>, SwitchScreenAction<DefaultModeFrontPercentageChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETBACKPERCENTAGE>,  SwitchScreenAction<DefaultModeBackPercentageChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETADDSCHWELLE>,     SwitchScreenAction<DefaultModeAddSchwelleChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAS1WERT>,        SwitchScreenAction<DefaultModeGas1WertChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETGAS2WERT>,        SwitchScreenAction<DefaultModeGas2WertChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETBREMS1WERT>,      SwitchScreenAction<DefaultModeBrems1WertChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETBREMS2WERT>,      SwitchScreenAction<DefaultModeBrems2WertChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,               SwitchScreenAction<ModesSettingsMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
} // namespace
