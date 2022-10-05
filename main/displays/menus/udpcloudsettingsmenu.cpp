#include "udpcloudsettingsmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay.h>
#include <fmt/core.h>
#include <icons/back.h>
#include <menuitem.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "bobbycheckbox.h"
#include "displays/bobbychangevaluedisplay.h"

namespace {
constexpr char TEXT_UDPCLOUDSETTINGS[] = "UDP Cloud settings";
constexpr char TEXT_UDPCLOUDENABLED[] = "Udp Cloud enabled";
constexpr char TEXT_UDPSENDRATE[] = "Udp send rate";
constexpr char TEXT_BACK[] = "Back";

using UdpCloudSendRateChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_UDPSENDRATE>,
    UdpCloudSendIntervalAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

using namespace espgui;

UdpCloudSettingsMenu::UdpCloudSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UDPCLOUDENABLED>,      BobbyCheckbox, UdpCloudEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_UDPSENDRATE>,          PushScreenAction<UdpCloudSendRateChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                 PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string UdpCloudSettingsMenu::text() const
{
    return TEXT_UDPCLOUDSETTINGS;
}

void UdpCloudSettingsMenu::back()
{
    espgui::popScreen();
}
