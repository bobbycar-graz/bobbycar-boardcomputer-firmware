#include "udpcloudsettingsmenu.h"

// 3rdparty lib includes
#include <fmt/core.h>
#include "menuitem.h"
#include "changevaluedisplay.h"
#include "actions/switchscreenaction.h"
#include "actions/toggleboolaction.h"
#include "actions/dummyaction.h"
#include "checkboxicon.h"
#include "icons/back.h"

// local includes
#include "accessors/settingsaccessors.h"
#include "displays/menus/settingsmenu.h"

#ifdef FEATURE_UDPCLOUD
namespace {
using UdpCloudSendRateChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_UDPSENDRATE>,
    UdpCloudSendIntervalAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<UdpCloudSettingsMenu>>,
    espgui::SwitchScreenAction<UdpCloudSettingsMenu>
>;
} // namespace

using namespace espgui;

UdpCloudSettingsMenu::UdpCloudSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UDPCLOUDENABLED>,      ToggleBoolAction, CheckboxIcon, UdpCloudEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UDPUSESTRING>,         ToggleBoolAction, CheckboxIcon, UdpUseStdStringAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UDPSENDRATE>,          SwitchScreenAction<UdpCloudSendRateChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                 SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void UdpCloudSettingsMenu::back()
{
    switchScreen<SettingsMenu>();
}
#endif
