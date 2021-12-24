#include "limitssettingsmenu.h"

// 3rdparty lib includes
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"

// local includes
#include "utilities/utils.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/settingsmenu.h"

namespace {
using IMotMaxChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_IMOTMAX>,
    IMotMaxAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LimitsSettingsMenu>>,
    espgui::SwitchScreenAction<LimitsSettingsMenu>
>;
using IDcMaxChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_IDCMAX>,
    IDcMaxAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LimitsSettingsMenu>>,
    espgui::SwitchScreenAction<LimitsSettingsMenu>
>;
using NMotMaxKmhChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_NMOTMAXKMH>,
    NMotMaxKmhAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LimitsSettingsMenu>>,
    espgui::SwitchScreenAction<LimitsSettingsMenu>
>;
using NMotMaxRpmChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_NMOTMAX>,
    NMotMaxRpmAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LimitsSettingsMenu>>,
    espgui::SwitchScreenAction<LimitsSettingsMenu>
>;
using FieldWeakMaxChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_FIELDWEAKMAX>,
    FieldWeakMaxAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LimitsSettingsMenu>>,
    espgui::SwitchScreenAction<LimitsSettingsMenu>
>;
using PhaseAdvMaxChangeScreen = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_PHASEADVMAX>,
    PhaseAdvMaxAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<LimitsSettingsMenu>>,
    espgui::SwitchScreenAction<LimitsSettingsMenu>
>;
} // namespace

using namespace espgui;

LimitsSettingsMenu::LimitsSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_IMOTMAX, IMotMaxAccessor>,           SwitchScreenAction<IMotMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_IDCMAX, IDcMaxAccessor>,             SwitchScreenAction<IDcMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_NMOTMAXKMH, NMotMaxKmhAccessor>,     SwitchScreenAction<NMotMaxKmhChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_NMOTMAX, NMotMaxRpmAccessor>,        SwitchScreenAction<NMotMaxRpmChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_FIELDWEAKMAX, FieldWeakMaxAccessor>, SwitchScreenAction<FieldWeakMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_PHASEADVMAX, PhaseAdvMaxAccessor>,   SwitchScreenAction<PhaseAdvMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                        SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void LimitsSettingsMenu::back()
{
    switchScreen<SettingsMenu>();
}
