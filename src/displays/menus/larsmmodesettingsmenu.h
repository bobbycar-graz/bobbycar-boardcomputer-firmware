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
class LarsmModeSettingsMenu;
class ModesSettingsMenu;
}

namespace {
using LarsmModeModeChangeDisplay = makeComponent<
    ChangeValueDisplay<LarsmModeMode>,
    StaticText<TEXT_LARSMMODECHANGEMODE>,
    LarsmModeModeAccessor,
    BackActionInterface<SwitchScreenAction<LarsmModeSettingsMenu>>,
    SwitchScreenAction<LarsmModeSettingsMenu>
>;
using LarsmModeIterationsChangeDisplay = makeComponent<
    ChangeValueDisplay<uint8_t>,
    StaticText<TEXT_LARSMMODECHANGEITERATIONS>,
    LarsmModeIterationsAccessor,
    BackActionInterface<SwitchScreenAction<LarsmModeSettingsMenu>>,
    SwitchScreenAction<LarsmModeSettingsMenu>
>;

class LarsmModeSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_LARSMMODESETTINGS>,
    public BackActionInterface<SwitchScreenAction<ModesSettingsMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_LARSMMODECHANGEMODE>,       SwitchScreenAction<LarsmModeModeChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_LARSMMODECHANGEITERATIONS>, SwitchScreenAction<LarsmModeIterationsChangeDisplay>>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,                      SwitchScreenAction<ModesSettingsMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}
#endif
