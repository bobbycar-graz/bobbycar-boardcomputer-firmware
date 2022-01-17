#include "udpcloudsettingsmenu.h"

// 3rdparty lib includes
#include <fmt/core.h>
#include "menuitem.h"
#include "changevaluedisplay.h"
#include "actions/switchscreenaction.h"
#include "actions/dummyaction.h"
#include "icons/back.h"

// local includes
#include "displays/bobbychangevaluedisplay.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/settingsmenu.h"
#include "bobbycheckbox.h"

namespace {
constexpr char TEXT_UDPCLOUDSETTINGS[] = "UDP Cloud settings";
constexpr char TEXT_UDPCLOUDENABLED[] = "Udp Cloud enabled";
constexpr char TEXT_UDPSENDRATE[] = "Udp send rate";
constexpr char TEXT_UDPUSESTRING[] = "Udp use std::string";
constexpr char TEXT_BACK[] = "Back";

using UdpCloudSendRateChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_UDPSENDRATE>,
    UdpCloudSendIntervalAccessor,
    espgui::ConfirmActionInterface<espgui::SwitchScreenAction<UdpCloudSettingsMenu>>,
    espgui::BackActionInterface<espgui::SwitchScreenAction<UdpCloudSettingsMenu>>
>;
} // namespace

using namespace espgui;

UdpCloudSettingsMenu::UdpCloudSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UDPCLOUDENABLED>,      BobbyCheckbox, UdpCloudEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UDPUSESTRING>,         BobbyCheckbox, UdpUseStdStringAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UDPSENDRATE>,          SwitchScreenAction<UdpCloudSendRateChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                 SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string UdpCloudSettingsMenu::text() const
{
    return TEXT_UDPCLOUDSETTINGS;
}

void UdpCloudSettingsMenu::back()
{
    switchScreen<SettingsMenu>();
}
