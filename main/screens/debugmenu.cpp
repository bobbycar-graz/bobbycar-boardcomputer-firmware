#include "debugmenu.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <menuitem.h>
#include <screenmanager.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "actions/erasenvsaction.h"
#include "actions/loadsettingsaction.h"
#include "actions/resetnvsaction.h"
#include "actions/savesettingsaction.h"
#include "debugcolorhelpers.h"
#include "screens/taskmanagermenu.h"
#include "screens/qrcodedebug.h"
#include "screens/xydebugdisplay.h"
#include "icons/back.h"
#include "icons/battery.h"
#include "icons/info.h"
#include "icons/lock.h"
#include "texthelpers/esptexthelpers.h"
#include "utils.h"
#ifdef FEATURE_CAN
#include "screens/candebugmenu.h"
#endif
#include "screens/batterydebugmenu.h"
#include "screens/commanddebugmenu.h"
#include "screens/dynamicdebugmenu.h"
#include "screens/feedbackdebugmenu.h"
#include "screens/motorfeedbackdebugmenu.h"
#include "screens/motorstatedebugmenu.h"
#include "guihelpers/bobbycheckbox.h"

namespace bobby {

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

class AccelerationText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("Accel: {:.6f}", avgAccel);
    }
};
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
    constructMenuItem<makeComponent<MenuItem, AccelerationText,                      StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTCOMMAND>,         PushScreenAction<FrontCommandDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKCOMMAND>,          PushScreenAction<BackCommandDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTLEFTCOMMAND>,     PushScreenAction<FrontLeftMotorStateDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTRIGHTCOMMAND>,    PushScreenAction<FrontRightMotorStateDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLEFTCOMMAND>,      PushScreenAction<BackLeftMotorStateDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKRIGHTCOMMAND>,     PushScreenAction<BackRightMotorStateDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTFEEDBACK>,        PushScreenAction<FrontFeedbackDebugMenu>, FrontFeedbackColor<espgui::TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKFEEDBACK>,         PushScreenAction<BackFeedbackDebugMenu>, BackFeedbackColor<espgui::TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTLEFTFEEDBACK>,    PushScreenAction<FrontLeftMotorFeedbackDebugMenu>, FrontFeedbackColor<espgui::TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTRIGHTFEEDBACK>,   PushScreenAction<FrontRightMotorFeedbackDebugMenu>, FrontFeedbackColor<espgui::TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLEFTFEEDBACK>,     PushScreenAction<BackLeftMotorFeedbackDebugMenu>, BackFeedbackColor<espgui::TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKRIGHTFEEDBACK>,    PushScreenAction<BackRightMotorFeedbackDebugMenu>, BackFeedbackColor<espgui::TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LOADSETTINGS>,         LoadSettingsAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SAVESETTINGS>,         SaveSettingsAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ERASENVS>,             EraseNvsAction>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_RESET_NVS_NEW>,        ResetNVSAction, StaticMenuItemIcon<&bobbyicons::info>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DYNAMICMENU>,          PushScreenAction<DynamicDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                 PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string DebugMenu::text() const
{
    return TEXT_DEBUG;
}

void DebugMenu::back()
{
    espgui::popScreen();
}

} // namespace bobby
