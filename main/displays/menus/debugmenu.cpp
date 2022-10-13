#include "debugmenu.h"

// 3rdparty lib includes
#include <menuitem.h>
#include <actions/pushscreenaction.h>
#include <actions/popscreenaction.h>
#include <actions/dummyaction.h>
#include <icons/back.h>
#include <screenmanager.h>

// local includes
#include "utils.h"
#include "actions/loadsettingsaction.h"
#include "actions/savesettingsaction.h"
#include "actions/erasenvsaction.h"
#include "actions/resetnvsaction.h"
#include "icons/lock.h"
#include "icons/battery.h"
#include "icons/info.h"
#include "debugcolorhelpers.h"
#include "esptexthelpers.h"
#include "accessors/settingsaccessors.h"
#include "displays/qrcodedebug.h"
#include "displays/xydebugdisplay.h"
#include "displays/menus/taskmanagermenu.h"
#ifdef FEATURE_CAN
#include "displays/menus/candebugmenu.h"
#endif
#include "displays/menus/commanddebugmenu.h"
#include "displays/menus/motorstatedebugmenu.h"
#include "displays/menus/feedbackdebugmenu.h"
#include "displays/menus/motorfeedbackdebugmenu.h"
#include "displays/menus/dynamicdebugmenu.h"
#include "displays/menus/batterydebugmenu.h"
#include "bobbycheckbox.h"

namespace {
constexpr char TEXT_DEBUG[] = "Debug";
constexpr char TEXT_TASKMANAGER[] = "Taskmanager";
#ifdef FEATURE_CAN
constexpr char TEXT_CANDEBUG[] = "CAN Debug";
#endif
constexpr char TEXT_QRCODE_DEBUG[] = "QR Debug";
constexpr char TEXT_XY_DEBUG[] = "XY Debug";
constexpr char TEXT_BATTERYDEBUG[] = "Bat Debug Menu";
constexpr char TEXT_TOGGLECLOUDDEBUG[] = "Cloud Debug";
//constexpr char TEXT_FRONTCOMMAND[] = "Front command";
//constexpr char TEXT_BACKCOMMAND[] = "Back command";
//constexpr char TEXT_FRONTLEFTCOMMAND[] = "Front left command";
//constexpr char TEXT_FRONTRIGHTCOMMAND[] = "Front right command";
//constexpr char TEXT_BACKLEFTCOMMAND[] = "Back left command";
//constexpr char TEXT_BACKRIGHTCOMMAND[] = "Back right command";
//constexpr char TEXT_FRONTFEEDBACK[] = "Front feedback";
//constexpr char TEXT_BACKFEEDBACK[] = "Back feedback";
//constexpr char TEXT_FRONTLEFTFEEDBACK[] = "Front left feedback";
//constexpr char TEXT_FRONTRIGHTFEEDBACK[] = "Front right feedback";
//constexpr char TEXT_BACKLEFTFEEDBACK[] = "Back left feedback";
//constexpr char TEXT_BACKRIGHTFEEDBACK[] = "Back right feedback";
constexpr char TEXT_LOADSETTINGS[] = "Load settings (old)";
constexpr char TEXT_SAVESETTINGS[] = "Save settings (old)";
constexpr char TEXT_ERASENVS[] = "Erase NVS (old)";
constexpr char TEXT_RESET_NVS_NEW[] = "Reset NVS (new)";
constexpr char TEXT_DYNAMICMENU[] = "GUI experiments";
constexpr char TEXT_BACK[] = "Back";
} // namespace

DebugMenu::DebugMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TASKMANAGER>,          PushScreenAction<TaskmanagerMenu>>>();
#ifdef FEATURE_CAN
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CANDEBUG>,             PushScreenAction<CanDebugMenu>>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_QRCODE_DEBUG>,         PushScreenAction<QrCodeDebugDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_XY_DEBUG>,             PushScreenAction<XYDebugDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERYDEBUG>,         PushScreenAction<BatteryDebugMenu>, StaticMenuItemIcon<&bobbyicons::battery>>>();
    if (configs.feature.udpcloud.isEnabled.value())
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TOGGLECLOUDDEBUG>, BobbyCheckbox, UdpCloudDebugEnableAccessor>>();
    }
    constructMenuItem<makeComponent<MenuItem, LastRebootReasonText,                  StaticFont<2>, DisabledColor, DummyAction>>();
#ifdef FEATURE_CAN
    constructMenuItem<makeComponent<MenuItem, CanIcCrashText,                        StaticFont<2>, DisabledColor, DummyAction>>();
#endif
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTCOMMAND>,         PushScreenAction<FrontCommandDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKCOMMAND>,          PushScreenAction<BackCommandDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTLEFTCOMMAND>,     PushScreenAction<FrontLeftMotorStateDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTRIGHTCOMMAND>,    PushScreenAction<FrontRightMotorStateDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLEFTCOMMAND>,      PushScreenAction<BackLeftMotorStateDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKRIGHTCOMMAND>,     PushScreenAction<BackRightMotorStateDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTFEEDBACK>,        PushScreenAction<FrontFeedbackDebugMenu>, FrontFeedbackColor<TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKFEEDBACK>,         PushScreenAction<BackFeedbackDebugMenu>, BackFeedbackColor<TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTLEFTFEEDBACK>,    PushScreenAction<FrontLeftMotorFeedbackDebugMenu>, FrontFeedbackColor<TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTRIGHTFEEDBACK>,   PushScreenAction<FrontRightMotorFeedbackDebugMenu>, FrontFeedbackColor<TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLEFTFEEDBACK>,     PushScreenAction<BackLeftMotorFeedbackDebugMenu>, BackFeedbackColor<TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKRIGHTFEEDBACK>,    PushScreenAction<BackRightMotorFeedbackDebugMenu>, BackFeedbackColor<TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LOADSETTINGS>,         LoadSettingsAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SAVESETTINGS>,         SaveSettingsAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ERASENVS>,             EraseNvsAction>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_RESET_NVS_NEW>,        ResetNVSAction, StaticMenuItemIcon<&bobbyicons::info>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DYNAMICMENU>,          PushScreenAction<DynamicDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                 PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string DebugMenu::text() const
{
    return TEXT_DEBUG;
}

void DebugMenu::back()
{
    espgui::popScreen();
}
