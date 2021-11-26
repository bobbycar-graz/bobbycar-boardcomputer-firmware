#include "debugmenu.h"

// 3rdparty lib includes
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "actions/dummyaction.h"
#include "actions/toggleboolaction.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "accessors/settingsaccessors.h"
#include <screenmanager.h>

// local includes
#include "utils.h"
#include "actions/loadsettingsaction.h"
#include "actions/savesettingsaction.h"
#include "actions/erasenvsaction.h"
#include "icons/lock.h"
#include "icons/battery.h"
#include "debugcolorhelpers.h"
#include "esptexthelpers.h"
#include "displays/menus/commanddebugmenu.h"
#include "displays/menus/motorstatedebugmenu.h"
#include "displays/menus/feedbackdebugmenu.h"
#include "displays/menus/motorfeedbackdebugmenu.h"
#include "displays/menus/dynamicdebugmenu.h"
#include "displays/menus/mainmenu.h"
#include "displays/menus/batterydebugmenu.h"
#include "displays/popups/alertdisplay.cpp"

namespace {
class AlertAction : public espgui::MenuItem
{
public:
    std::string text() const override { return "Open popup"; }
    void triggered() override
    {
        auto newDisplay = std::make_unique<AlertDisplay>(std::move(currentDisplay));
        newDisplay->initOverlay();
        currentDisplay = std::move(newDisplay);
    }
};
} // namespace

using namespace espgui;

DebugMenu::DebugMenu()
{
    constructMenuItem<AlertAction>();
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
    constructMenuItem<makeComponent<MenuItem, LastRebootReasonText,                  StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTLEFTFEEDBACK>,    SwitchScreenAction<FrontLeftMotorFeedbackDebugMenu>, FrontFeedbackColor<TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FRONTRIGHTFEEDBACK>,   SwitchScreenAction<FrontRightMotorFeedbackDebugMenu>, FrontFeedbackColor<TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKLEFTFEEDBACK>,     SwitchScreenAction<BackLeftMotorFeedbackDebugMenu>, BackFeedbackColor<TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACKRIGHTFEEDBACK>,    SwitchScreenAction<BackRightMotorFeedbackDebugMenu>, BackFeedbackColor<TFT_WHITE>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DYNAMICMENU>,          SwitchScreenAction<DynamicDebugMenu>>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                             DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERYDEBUG>,         SwitchScreenAction<BatteryDebugMenu>, StaticMenuItemIcon<&bobbyicons::battery>>>();
#ifdef FEATURE_CLOUD
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TOGGLECLOUDDEBUG>,     ToggleBoolAction, CheckboxIcon, CloudDebugEnableAccessor>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                 SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void DebugMenu::back()
{
    switchScreen<MainMenu>();
}
