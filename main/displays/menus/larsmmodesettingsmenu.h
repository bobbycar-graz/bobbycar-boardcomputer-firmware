#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"
#include "settingsaccessors.h"

// forward declares
namespace {
class LarsmModeSettingsMenu;
class ModesSettingsMenu;
} // namespace

namespace {
using LarsmModeModelModeChangeDisplay = makeComponent<
    ChangeValueDisplay<UnifiedModelMode>,
    StaticText<TEXT_SETMODELMODE>,
    LarsmModeModelModeAccessor,
    BackActionInterface<SwitchScreenAction<LarsmModeSettingsMenu>>,
    SwitchScreenAction<LarsmModeSettingsMenu>
>;
using LarsmModeModeChangeDisplay = makeComponent<
    ChangeValueDisplay<LarsmModeMode>,
    StaticText<TEXT_SETMODE>,
    LarsmModeModeAccessor,
    BackActionInterface<SwitchScreenAction<LarsmModeSettingsMenu>>,
    SwitchScreenAction<LarsmModeSettingsMenu>
>;
using LarsmModeIterationsChangeDisplay = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_SETITERATIONS>,
    LarsmModeIterationsAccessor,
    BackActionInterface<SwitchScreenAction<LarsmModeSettingsMenu>>,
    SwitchScreenAction<LarsmModeSettingsMenu>
>;

class LarsmModeSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_LARSMMODESETTINGS>,
    public BackActionInterface<SwitchScreenAction<ModesSettingsMenu>>
{
public:
    LarsmModeSettingsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETMODELMODE>,  SwitchScreenAction<LarsmModeModelModeChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETMODE>,       SwitchScreenAction<LarsmModeModeChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETITERATIONS>, SwitchScreenAction<LarsmModeIterationsChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,          SwitchScreenAction<ModesSettingsMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
} // namespace
