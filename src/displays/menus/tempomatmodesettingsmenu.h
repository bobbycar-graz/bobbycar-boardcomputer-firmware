#pragma once

#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"
#include "settingsaccessors.h"

#ifdef GLUMP_CONTROLLER
namespace {
class TempomatModeSettingsMenu;
class ModesSettingsMenu;
}

namespace {
using TempomatModeCtrlTypChangeScreen = makeComponent<
    ChangeValueDisplay<ControlType>,
    StaticText<TEXT_SETCONTROLMODE>,
    TempomatModeCtrlTypAccessor,
    BackActionInterface<SwitchScreenAction<TempomatModeSettingsMenu>>,
    SwitchScreenAction<TempomatModeSettingsMenu>
>;
using TempomatModeCtrlModChangeScreen = makeComponent<
    ChangeValueDisplay<ControlMode>,
    StaticText<TEXT_SETCONTROLMODE>,
    TempomatModeCtrlModAccessor,
    BackActionInterface<SwitchScreenAction<TempomatModeSettingsMenu>>,
    SwitchScreenAction<TempomatModeSettingsMenu>
>;

class TempomatModeSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_TEMPOMATMODESETTINGS>,
    public BackActionInterface<SwitchScreenAction<ModesSettingsMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_SETCONTROLTYPE>, SwitchScreenAction<TempomatModeCtrlTypChangeScreen>>,
        makeComponent<MenuItem, StaticText<TEXT_SETCONTROLMODE>, SwitchScreenAction<TempomatModeCtrlModChangeScreen>>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,           SwitchScreenAction<ModesSettingsMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}
#endif