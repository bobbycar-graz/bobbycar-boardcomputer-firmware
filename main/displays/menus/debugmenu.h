#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "menuitem.h"
#include "actions/loadsettingsaction.h"
#include "actions/savesettingsaction.h"
#include "actions/erasenvsaction.h"
#include "actions/switchscreenaction.h"
#include "actions/dummyaction.h"
#include "actions/toggleboolaction.h"
#include "icons/lock.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "texts.h"
#include "debugcolorhelpers.h"

// forward declares
namespace {
class MainMenu;
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
class DynamicDebugMenu;
} // namespace

namespace {
class DebugMenu :
    public MenuDisplay,
    public StaticText<TEXT_DEBUG>,
    public BackActionInterface<SwitchScreenAction<MainMenu>>
{
public:
    DebugMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LOADSETTINGS>,         LoadSettingsAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SAVESETTINGS>,         SaveSettingsAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ERASENVS>,             EraseNvsAction>>();
        constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTCOMMAND>,         SwitchScreenAction<FrontCommandDebugMenu>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKCOMMAND>,          SwitchScreenAction<BackCommandDebugMenu>>>();
        constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTLEFTCOMMAND>,     SwitchScreenAction<FrontLeftMotorStateDebugMenu>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTRIGHTCOMMAND>,    SwitchScreenAction<FrontRightMotorStateDebugMenu>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLEFTCOMMAND>,      SwitchScreenAction<BackLeftMotorStateDebugMenu>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKRIGHTCOMMAND>,     SwitchScreenAction<BackRightMotorStateDebugMenu>>>();
        constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTFEEDBACK>,        SwitchScreenAction<FrontFeedbackDebugMenu>, FrontFeedbackColor<TFT_WHITE>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKFEEDBACK>,         SwitchScreenAction<BackFeedbackDebugMenu>, BackFeedbackColor<TFT_WHITE>>>();
        constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTLEFTFEEDBACK>,    SwitchScreenAction<FrontLeftMotorFeedbackDebugMenu>, FrontFeedbackColor<TFT_WHITE>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTRIGHTFEEDBACK>,   SwitchScreenAction<FrontRightMotorFeedbackDebugMenu>, FrontFeedbackColor<TFT_WHITE>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLEFTFEEDBACK>,     SwitchScreenAction<BackLeftMotorFeedbackDebugMenu>, BackFeedbackColor<TFT_WHITE>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKRIGHTFEEDBACK>,    SwitchScreenAction<BackRightMotorFeedbackDebugMenu>, BackFeedbackColor<TFT_WHITE>>>();
        constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DYNAMICMENU>,          SwitchScreenAction<DynamicDebugMenu>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                 SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
} // namespace
