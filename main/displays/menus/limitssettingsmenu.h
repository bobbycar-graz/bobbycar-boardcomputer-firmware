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
class LimitsSettingsMenu;
class SettingsMenu;
} // namespace

namespace {
template<const char *Tprefix, typename Taccessor>
struct TextWithValueHelper : public virtual TextInterface
{
    std::string text() const override { return Tprefix + (' ' + std::to_string(Taccessor{}.getValue())); }
};

using IMotMaxChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_IMOTMAX>,
    IMotMaxAccessor,
    BackActionInterface<SwitchScreenAction<LimitsSettingsMenu>>,
    SwitchScreenAction<LimitsSettingsMenu>
>;
using IDcMaxChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_IDCMAX>,
    IDcMaxAccessor,
    BackActionInterface<SwitchScreenAction<LimitsSettingsMenu>>,
    SwitchScreenAction<LimitsSettingsMenu>
>;
using NMotMaxKmhChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_NMOTMAXKMH>,
    NMotMaxKmhAccessor,
    BackActionInterface<SwitchScreenAction<LimitsSettingsMenu>>,
    SwitchScreenAction<LimitsSettingsMenu>
>;
using NMotMaxRpmChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_NMOTMAX>,
    NMotMaxRpmAccessor,
    BackActionInterface<SwitchScreenAction<LimitsSettingsMenu>>,
    SwitchScreenAction<LimitsSettingsMenu>
>;
using FieldWeakMaxChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_FIELDWEAKMAX>,
    FieldWeakMaxAccessor,
    BackActionInterface<SwitchScreenAction<LimitsSettingsMenu>>,
    SwitchScreenAction<LimitsSettingsMenu>
>;
using PhaseAdvMaxChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_PHASEADVMAX>,
    PhaseAdvMaxAccessor,
    BackActionInterface<SwitchScreenAction<LimitsSettingsMenu>>,
    SwitchScreenAction<LimitsSettingsMenu>
>;

class LimitsSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_LIMITSSETTINGS>,
    public BackActionInterface<SwitchScreenAction<SettingsMenu>>
{
public:
    LimitsSettingsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_IMOTMAX, IMotMaxAccessor>,           SwitchScreenAction<IMotMaxChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_IDCMAX, IDcMaxAccessor>,             SwitchScreenAction<IDcMaxChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_NMOTMAXKMH, NMotMaxKmhAccessor>,     SwitchScreenAction<NMotMaxKmhChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_NMOTMAX, NMotMaxRpmAccessor>,        SwitchScreenAction<NMotMaxRpmChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_FIELDWEAKMAX, FieldWeakMaxAccessor>, SwitchScreenAction<FieldWeakMaxChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_PHASEADVMAX, PhaseAdvMaxAccessor>,   SwitchScreenAction<PhaseAdvMaxChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                           SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
} // namespace
