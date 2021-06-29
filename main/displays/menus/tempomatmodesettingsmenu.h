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
class TempomatModeSettingsMenu;
class ModesSettingsMenu;
} // namespace

namespace {
using TempomatModeModelModeChangeScreen = makeComponent<
    ChangeValueDisplay<UnifiedModelMode>,
    StaticText<TEXT_SETMODELMODE>,
    TempomatModeModelModeAccessor,
    BackActionInterface<SwitchScreenAction<TempomatModeSettingsMenu>>,
    SwitchScreenAction<TempomatModeSettingsMenu>
>;

class TempomatModeSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_TEMPOMATMODESETTINGS>,
    public BackActionInterface<SwitchScreenAction<ModesSettingsMenu>>
{
public:
    TempomatModeSettingsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETMODELMODE>, SwitchScreenAction<TempomatModeModelModeChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,         SwitchScreenAction<ModesSettingsMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
} // namespace
