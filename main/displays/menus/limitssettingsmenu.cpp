#include "limitssettingsmenu.h"

// 3rdparty lib includes
#include <changevaluedisplay.h>
#include <menuitem.h>
#include <actions/pushscreenaction.h>
#include <actions/popscreenaction.h>
#include <icons/back.h>
#include <textwithvaluehelper.h>

// local includes
#include "displays/bobbychangevaluedisplay.h"
#include "utils.h"
#include "accessors/settingsaccessors.h"

namespace {
constexpr char TEXT_LIMITSSETTINGS[] = "Limit settings";
constexpr char TEXT_IMOTMAX[] = "iMotMax";
constexpr char TEXT_IDCMAX[] = "iDcMax";
constexpr char TEXT_NMOTMAXKMH[] = "nMotMaxKmh";
constexpr char TEXT_NMOTMAX[] = "nMotMax";
constexpr char TEXT_FIELDWEAKMAX[] = "fldWkMax";
constexpr char TEXT_PHASEADVMAX[] = "phsAdvMax";
constexpr char TEXT_BACK[] = "Back";

using IMotMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_IMOTMAX>,
    IMotMaxAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using IDcMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_IDCMAX>,
    IDcMaxAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using NMotMaxKmhChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_NMOTMAXKMH>,
    NMotMaxKmhAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using NMotMaxRpmChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_NMOTMAX>,
    NMotMaxRpmAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using FieldWeakMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_FIELDWEAKMAX>,
    FieldWeakMaxAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
using PhaseAdvMaxChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_PHASEADVMAX>,
    PhaseAdvMaxAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

using namespace espgui;

LimitsSettingsMenu::LimitsSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_IMOTMAX, IMotMaxAccessor>,           PushScreenAction<IMotMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_IDCMAX, IDcMaxAccessor>,             PushScreenAction<IDcMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_NMOTMAXKMH, NMotMaxKmhAccessor>,     PushScreenAction<NMotMaxKmhChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_NMOTMAX, NMotMaxRpmAccessor>,        PushScreenAction<NMotMaxRpmChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_FIELDWEAKMAX, FieldWeakMaxAccessor>, PushScreenAction<FieldWeakMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_PHASEADVMAX, PhaseAdvMaxAccessor>,   PushScreenAction<PhaseAdvMaxChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                        PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string LimitsSettingsMenu::text() const
{
    return TEXT_LIMITSSETTINGS;
}

void LimitsSettingsMenu::back()
{
    popScreen();
}
