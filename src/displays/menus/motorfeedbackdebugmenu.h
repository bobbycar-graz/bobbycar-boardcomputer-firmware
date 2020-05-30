#pragma once

#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "menuitem.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "texts.h"
#include "debugtexthelpers.h"
#include "debugcolorhelpers.h"

#ifdef GLUMP_CONTROLLER
namespace {
class DebugMenu;
}

namespace {
template<const char *Ttext, typename Ttexts, template<int> class ColorInterface>
class MotorFeedbackDebugMenu :
    public MenuDisplay,
    public StaticText<Ttext>,
    public BackActionInterface<SwitchScreenAction<DebugMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, typename Ttexts::AngleText,        ColorInterface<TFT_DARKGREY>, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::SpeedText,        ColorInterface<TFT_DARKGREY>, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::SpeedKmhText,     ColorInterface<TFT_DARKGREY>, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::ErrorText,        ColorInterface<TFT_DARKGREY>, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::CurrentText,      ColorInterface<TFT_DARKGREY>, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::CurrentFixedText, ColorInterface<TFT_DARKGREY>, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::ChopsText,        ColorInterface<TFT_DARKGREY>, DummyAction>,
        makeComponent<MenuItem, typename Ttexts::HallText,         ColorInterface<TFT_DARKGREY>, DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,                             SwitchScreenAction<DebugMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};

class FrontLeftMotorFeedbackDebugMenu : public MotorFeedbackDebugMenu<TEXT_FRONTLEFTFEEDBACK, FrontTexts::LeftFeedback, FrontFeedbackColor> {};
class FrontRightMotorFeedbackDebugMenu : public MotorFeedbackDebugMenu<TEXT_FRONTRIGHTFEEDBACK, FrontTexts::RightFeedback, FrontFeedbackColor> {};
class BackLeftMotorFeedbackDebugMenu : public MotorFeedbackDebugMenu<TEXT_BACKLEFTFEEDBACK, BackTexts::LeftFeedback, BackFeedbackColor> {};
class BackRightMotorFeedbackDebugMenu : public MotorFeedbackDebugMenu<TEXT_BACKRIGHTFEEDBACK, BackTexts::RightFeedback, BackFeedbackColor> {};
}
#endif