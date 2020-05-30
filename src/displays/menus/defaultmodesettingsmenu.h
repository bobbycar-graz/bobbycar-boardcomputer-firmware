#pragma once

#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/toggleboolaction.h"
#include "actions/switchscreenaction.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "texts.h"
#include "settingsaccessors.h"

namespace {
class DefaultModeSettingsMenu;
class ModesSettingsMenu;
}

namespace {
#ifdef GLUMP_CONTROLLER
using DefaultModeCtrlTypChangeDisplay = makeComponent<
    ChangeValueDisplay<ControlType>,
    StaticText<TEXT_SETCONTROLTYPE>,
    DefaultModeCtrlTypAccessor,
    BackActionInterface<SwitchScreenAction<DefaultModeSettingsMenu>>,
    SwitchScreenAction<DefaultModeSettingsMenu>
>;
using DefaultModeCtrlModChangeDisplay = makeComponent<
    ChangeValueDisplay<ControlMode>,
    StaticText<TEXT_SETCONTROLMODE>,
    DefaultModeCtrlModAccessor,
    BackActionInterface<SwitchScreenAction<DefaultModeSettingsMenu>>,
    SwitchScreenAction<DefaultModeSettingsMenu>
>;
#endif
using DefaultModeSmoothingChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_SETSMOOTHING>,
    DefaultModeSmoothingAccessor,
    BackActionInterface<SwitchScreenAction<DefaultModeSettingsMenu>>,
    SwitchScreenAction<DefaultModeSettingsMenu>
>;
#ifdef GLUMP_CONTROLLER
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
#endif
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
    public BackActionInterface<SwitchScreenAction<ModesSettingsMenu>>,
    public StaticMenuDefinition<
#ifdef GLUMP_CONTROLLER
        makeComponent<MenuItem, StaticText<TEXT_SETCONTROLTYPE>,     SwitchScreenAction<DefaultModeCtrlTypChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_SETCONTROLMODE>,     SwitchScreenAction<DefaultModeCtrlModChangeDisplay>>,
#endif
        makeComponent<MenuItem, StaticText<TEXT_ENABLESMOOTHING>,     ToggleBoolAction, CheckboxIcon, DefaultModeEnableSmoothingAccessor>,
        makeComponent<MenuItem, StaticText<TEXT_SETSMOOTHING>,       SwitchScreenAction<DefaultModeSmoothingChangeDisplay>>,
#ifdef GLUMP_CONTROLLER
        makeComponent<MenuItem, StaticText<TEXT_SETFRONTPERCENTAGE>, SwitchScreenAction<DefaultModeFrontPercentageChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_SETBACKPERCENTAGE>,  SwitchScreenAction<DefaultModeBackPercentageChangeDisplay>>,
#endif
        makeComponent<MenuItem, StaticText<TEXT_SETADDSCHWELLE>,     SwitchScreenAction<DefaultModeAddSchwelleChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_SETGAS1WERT>,        SwitchScreenAction<DefaultModeGas1WertChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_SETGAS2WERT>,        SwitchScreenAction<DefaultModeGas2WertChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_SETBREMS1WERT>,      SwitchScreenAction<DefaultModeBrems1WertChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_SETBREMS2WERT>,      SwitchScreenAction<DefaultModeBrems2WertChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,               SwitchScreenAction<ModesSettingsMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}
