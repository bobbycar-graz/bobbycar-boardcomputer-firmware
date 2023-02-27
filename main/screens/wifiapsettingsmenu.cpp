#include "wifiapsettingsmenu.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <espwifistack.h>
#include <menuitem.h>
#include <screenmanager.h>
#include <textwithvaluehelper.h>

// local includes
#include "accessors/wifiapconfigaccessors.h"
#include "changevaluedisplay.h"
#include "changevaluedisplay_ip_address_t.h"
#include "changevaluedisplay_string.h"
#include "changevaluedisplay_wifi_auth_mode_t.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"
#include "networksettingsmenu.h"
#include "texthelpers/wifiaptexthelpers.h"
#include "wifiapclientsmenu.h"

namespace bobby {

using namespace espgui;

namespace {
constexpr const char * const TAG = "BOBBY";

constexpr char TEXT_ENABLED[] = "Enabled";
constexpr char TEXT_DISABLEWHENONLINE[] = "Disable when online";
constexpr char TEXT_BACK[] = "Back";

constexpr char TEXT_SSID[] = "AP SSID";
constexpr char TEXT_SSID_FORMATTED[] = "&sssid: ";
constexpr char TEXT_KEY[] = "AP Key";
constexpr char TEXT_KEY_FORMATTED[] = "&skey: ";
constexpr char TEXT_IP[] = "AP IP";
constexpr char TEXT_IP_FORMATTED[] = "&sIP: &f";
constexpr char TEXT_MASK[] = "AP Mask";
constexpr char TEXT_MASK_FORMATTED[] = "&sMask: &f";
constexpr char TEXT_CHANNEL[] = "AP Channel";
constexpr char TEXT_CHANNEL_FORMATTED[] = "&sChannel: &f";
constexpr char TEXT_AUTHMODE[] = "AP Authmode";
constexpr char TEXT_AUTHMODE_FORMATTED[] = "&sAuthmode: ";
constexpr char TEXT_HIDDEN[] = "AP Hidden";

class WifiApClientsAction : public virtual ActionInterface
{
public:
    void triggered() override;
};

using ApSsidChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<std::string>,
    espgui::StaticText<TEXT_SSID>,
    WifiApSsidAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using ApKeyChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<std::string>,
    espgui::StaticText<TEXT_KEY>,
    WifiApKeyAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using ApIpChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<wifi_stack::ip_address_t>,
    espgui::StaticText<TEXT_IP>,
    WifiApIpAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using ApMaskChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<wifi_stack::ip_address_t>,
    espgui::StaticText<TEXT_MASK>,
    WifiApMaskAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using ApChannelChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint8_t>,
    espgui::StaticText<TEXT_CHANNEL>,
    WifiApChannelAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using ApAuthmodeChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<wifi_auth_mode_t>,
    espgui::StaticText<TEXT_AUTHMODE>,
    WifiApAuthmodeAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

WifiApSettingsMenu::WifiApSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLED>,            BobbyCheckbox, WifiApEnabledAccessor>>();
  //constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DISABLEWHENONLINE>,  BobbyCheckbox, WifiApDisableWhenOnlineAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_HIDDEN>,             BobbyCheckbox, WiFiApHiddenAccessor>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_SSID_FORMATTED, WifiApSsidAccessor>,         PushScreenAction<ApSsidChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_KEY_FORMATTED, WifiApKeyAccessor>,           PushScreenAction<ApKeyChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, WifiApMacText,                       DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_IP_FORMATTED, WifiApIpAccessor>,             PushScreenAction<ApIpChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_MASK_FORMATTED, WifiApMaskAccessor>,         PushScreenAction<ApMaskChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_CHANNEL_FORMATTED, WifiApChannelAccessor>,   PushScreenAction<ApChannelChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithValueHelper<TEXT_AUTHMODE_FORMATTED, WifiApAuthmodeAccessor>, PushScreenAction<ApAuthmodeChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, WifiApHostnameText,                                                   DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, WifiApClientsText,                                                    WifiApClientsAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string WifiApSettingsMenu::text() const
{
    return "AP Settings";
}

void WifiApSettingsMenu::back()
{
    popScreen();
}

namespace {
void WifiApClientsAction::triggered()
{
    wifi_sta_list_t clients;
    if (const auto result = esp_wifi_ap_get_sta_list(&clients); result == ESP_OK)
        pushScreen<WifiApClientsMenu>();
    else
        ESP_LOGW(TAG, "esp_wifi_ap_get_sta_list() failed with %s", esp_err_to_name(result));
}
} // namespace

} // namespace bobby
