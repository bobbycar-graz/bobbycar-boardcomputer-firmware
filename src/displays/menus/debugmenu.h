#pragma once

#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "menuitem.h"
#include "actions/loadsettingsaction.h"
#include "actions/savesettingsaction.h"
#include "actions/switchscreenaction.h"
#include "actions/dummyaction.h"
#include "actions/toggleboolaction.h"
#include "icons/lock.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "texts.h"
#include "debugcolorhelpers.h"

namespace {
class MainMenu;
#ifdef GLUMP_CONTROLLER
class FrontCommandDebugMenu;
class BackCommandDebugMenu;
class FrontLeftMotorStateDebugMenu;
class FrontRightMotorStateDebugMenu;
class BackLeftMotorStateDebugMenu;
class BackRightMotorStateDebugMenu;
class FrontFeedbackDebugMenu;
class BackFeedbackDebugMenu;
class FrontLeftMotorFeedbackDebugMenu;
class FrontRightMotorFeedbackDebugMenu;
class BackLeftMotorFeedbackDebugMenu;
class BackRightMotorFeedbackDebugMenu;
#endif
class DynamicDebugMenu;
}

namespace {
class DebugMenu :
    public MenuDisplay,
    public StaticText<TEXT_DEBUG>,
    public BackActionInterface<SwitchScreenAction<MainMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_LOADSETTINGS>,         LoadSettingsAction>,
        makeComponent<MenuItem, StaticText<TEXT_SAVESETTINGS>,         SaveSettingsAction>,
        makeComponent<MenuItem, StaticText<nullptr>,                   DummyAction>,
#ifdef GLUMP_CONTROLLER
        makeComponent<MenuItem, StaticText<TEXT_FRONTCOMMAND>,         SwitchScreenAction<FrontCommandDebugMenu>>,
        makeComponent<MenuItem, StaticText<TEXT_BACKCOMMAND>,          SwitchScreenAction<BackCommandDebugMenu>>,
        makeComponent<MenuItem, StaticText<nullptr>,                   DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_FRONTLEFTCOMMAND>,     SwitchScreenAction<FrontLeftMotorStateDebugMenu>>,
        makeComponent<MenuItem, StaticText<TEXT_FRONTRIGHTCOMMAND>,    SwitchScreenAction<FrontRightMotorStateDebugMenu>>,
        makeComponent<MenuItem, StaticText<TEXT_BACKLEFTCOMMAND>,      SwitchScreenAction<BackLeftMotorStateDebugMenu>>,
        makeComponent<MenuItem, StaticText<TEXT_BACKRIGHTCOMMAND>,     SwitchScreenAction<BackRightMotorStateDebugMenu>>,
        makeComponent<MenuItem, StaticText<nullptr>,                   DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_FRONTFEEDBACK>,        SwitchScreenAction<FrontFeedbackDebugMenu>, FrontFeedbackColor<TFT_WHITE>>,
        makeComponent<MenuItem, StaticText<TEXT_BACKFEEDBACK>,         SwitchScreenAction<BackFeedbackDebugMenu>, BackFeedbackColor<TFT_WHITE>>,
        makeComponent<MenuItem, StaticText<nullptr>,                   DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_FRONTLEFTFEEDBACK>,    SwitchScreenAction<FrontLeftMotorFeedbackDebugMenu>, FrontFeedbackColor<TFT_WHITE>>,
        makeComponent<MenuItem, StaticText<TEXT_FRONTRIGHTFEEDBACK>,   SwitchScreenAction<FrontRightMotorFeedbackDebugMenu>, FrontFeedbackColor<TFT_WHITE>>,
        makeComponent<MenuItem, StaticText<TEXT_BACKLEFTFEEDBACK>,     SwitchScreenAction<BackLeftMotorFeedbackDebugMenu>, BackFeedbackColor<TFT_WHITE>>,
        makeComponent<MenuItem, StaticText<TEXT_BACKRIGHTFEEDBACK>,    SwitchScreenAction<BackRightMotorFeedbackDebugMenu>, BackFeedbackColor<TFT_WHITE>>,
#endif
        makeComponent<MenuItem, StaticText<nullptr>,                   DummyAction>,
        makeComponent<MenuItem, StaticText<TEXT_DYNAMICMENU>,          SwitchScreenAction<DynamicDebugMenu>>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,                 SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}
