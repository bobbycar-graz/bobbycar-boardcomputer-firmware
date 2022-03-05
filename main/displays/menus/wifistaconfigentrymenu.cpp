#include "wifistaconfigentrymenu.h"

// 3rdparty lib includes
#include <menuitem.h>
#include <actions/dummyaction.h>
#include <actions/pushscreenaction.h>
#include <actions/popscreenaction.h>
#include <icons/back.h>
#include <screenmanager.h>
#include <changevaluedisplay.h>
#include <changevaluedisplay_string.h>
#include <changevaluedisplay_ip_address_t.h>
#include <textwithvaluehelper.h>

// local includes
#include "displays/bobbychangevaluedisplay.h"
#include "accessors/wifistaconfigaccessors.h"
#include "wifistaconfigsmenu.h"
#include "newsettings.h"
#include "bobbycheckbox.h"

namespace {
constexpr char TEXT_SSID_TITLE[] = "WiFi {} SSID";
constexpr char TEXT_SSID_FORMATTED[] = "&sSSID: &f";
constexpr char TEXT_KEY_TITLE[] = "WiFi {} Key";
constexpr char TEXT_KEY_FORMATTED[] = "&sKey: &f";
constexpr char TEXT_USESTATICIP[] = "Use Static Ip";
constexpr char TEXT_STATICIP_TITLE[] = "WiFi {} Static Ip";
constexpr char TEXT_STATICIP_FORMATTED[] = "&sStatic Ip: &f";
constexpr char TEXT_STATICMASK_TITLE[] = "WiFi {} Static Mask";
constexpr char TEXT_STATICMASK_FORMATTED[] = "&sStatic Mask: &f";
constexpr char TEXT_STATICGW_TITLE[] = "WiFi {} Static Gw";
constexpr char TEXT_STATICGW_FORMATTED[] = "&sStatic Gw: &f";
constexpr char TEXT_USESTATICDNS[] = "Use Static Dns";
constexpr char TEXT_STATICDNS0_TITLE[] = "WiFi {} Static Dns0";
constexpr char TEXT_STATICDNS0_FORMATTED[] = "&sStatic Dns0: &f";
constexpr char TEXT_STATICDNS1_TITLE[] = "WiFi {} Static Dns1";
constexpr char TEXT_STATICDNS1_FORMATTED[] = "&sStatic Dns1: &f";
constexpr char TEXT_STATICDNS2_TITLE[] = "WiFi {} Static Dns2";
constexpr char TEXT_STATICDNS2_FORMATTED[] = "&sStatic Dns2: &f";
constexpr char TEXT_DELETECONFIG[] = "Delete config";
constexpr char TEXT_BACK[] = "Back";

template<typename T, typename Taccessor, const char *Ttitle>
class StaChangeScreen :
    public BobbyChangeValueDisplay<T>,
    public Taccessor
{
public:
    explicit StaChangeScreen(int index) : Taccessor{index}, m_index{index} {}
    std::string text() const override { return fmt::format(Ttitle, m_index + 1); }
    void confirm() override { espgui::popScreen(); }
    void back() override { espgui::popScreen(); }

private:
    const int m_index;
};

template<typename TchangeScreen, typename Taccessor, const char *Ttext>
class StaChangeMenuItem :
    public espgui::MenuItem,
    public espgui::TextWithValueHelper<Ttext, Taccessor>
{
public:
    explicit StaChangeMenuItem(int index) :
        espgui::TextWithValueHelper<Ttext, Taccessor>{index},
        m_index{index}
    {}

    void triggered() override { espgui::pushScreen<TchangeScreen>(m_index); }

private:
    const int m_index;
};

using StaSsidChangeScreen = StaChangeScreen<std::string, WifiStaConfigSsidAccessor, TEXT_SSID_TITLE>;
using StaSsidMenuItem = StaChangeMenuItem<StaSsidChangeScreen, WifiStaConfigSsidAccessor, TEXT_SSID_FORMATTED>;

using StaKeyChangeScreen = StaChangeScreen<std::string, WifiStaConfigKeyAccessor, TEXT_KEY_TITLE>;
using StaKeyMenuItem = StaChangeMenuItem<StaKeyChangeScreen, WifiStaConfigKeyAccessor, TEXT_KEY_FORMATTED>;

using StaStaticIpChangeScreen = StaChangeScreen<wifi_stack::ip_address_t, WifiStaConfigStaticIpAccessor, TEXT_STATICIP_TITLE>;
using StaStaticIpMenuItem = StaChangeMenuItem<StaStaticIpChangeScreen, WifiStaConfigStaticIpAccessor, TEXT_STATICIP_FORMATTED>;

using StaStaticMaskChangeScreen = StaChangeScreen<wifi_stack::ip_address_t, WifiStaConfigStaticSubnetAccessor, TEXT_STATICMASK_TITLE>;
using StaStaticMaskMenuItem = StaChangeMenuItem<StaStaticMaskChangeScreen, WifiStaConfigStaticSubnetAccessor, TEXT_STATICMASK_FORMATTED>;

using StaStaticGwChangeScreen = StaChangeScreen<wifi_stack::ip_address_t, WifiStaConfigStaticSubnetAccessor, TEXT_STATICGW_TITLE>;
using StaStaticGwMenuItem = StaChangeMenuItem<StaStaticGwChangeScreen, WifiStaConfigStaticSubnetAccessor, TEXT_STATICGW_FORMATTED>;

using StaStaticDns0ChangeScreen = StaChangeScreen<wifi_stack::ip_address_t, WifiStaConfigStaticSubnetAccessor, TEXT_STATICDNS0_TITLE>;
using StaStaticDns0MenuItem = StaChangeMenuItem<StaStaticDns0ChangeScreen, WifiStaConfigStaticSubnetAccessor, TEXT_STATICDNS0_FORMATTED>;

using StaStaticDns1ChangeScreen = StaChangeScreen<wifi_stack::ip_address_t, WifiStaConfigStaticSubnetAccessor, TEXT_STATICDNS1_TITLE>;
using StaStaticDns1MenuItem = StaChangeMenuItem<StaStaticDns1ChangeScreen, WifiStaConfigStaticSubnetAccessor, TEXT_STATICDNS1_FORMATTED>;

using StaStaticDns2ChangeScreen = StaChangeScreen<wifi_stack::ip_address_t, WifiStaConfigStaticSubnetAccessor, TEXT_STATICDNS2_TITLE>;
using StaStaticDns2MenuItem = StaChangeMenuItem<StaStaticDns2ChangeScreen, WifiStaConfigStaticSubnetAccessor, TEXT_STATICDNS2_FORMATTED>;

class ClearConfigAction : public virtual espgui::ActionInterface
{
public:
    explicit ClearConfigAction(int index) : m_index{index} {}

    void triggered() override;

private:
    const int m_index;
};
} // namespace

WifiStaConfigEntryMenu::WifiStaConfigEntryMenu(int index) :
    m_index{index}
{
    using namespace espgui;
    constructMenuItem<StaSsidMenuItem>(index);
    constructMenuItem<StaKeyMenuItem>(index);
    constructMenuItem<makeComponentArgs<MenuItem, WifiStaConfigUseStaticIpAccessor,  StaticText<TEXT_USESTATICIP>, BobbyCheckbox>>(index);
    constructMenuItem<StaStaticIpMenuItem>(index);
    constructMenuItem<StaStaticMaskMenuItem>(index);
    constructMenuItem<StaStaticGwMenuItem>(index);
    constructMenuItem<makeComponentArgs<MenuItem, WifiStaConfigUseStaticDnsAccessor, StaticText<TEXT_USESTATICDNS>, BobbyCheckbox>>(index);
    constructMenuItem<StaStaticDns0MenuItem>(index);
    constructMenuItem<StaStaticDns1MenuItem>(index);
    constructMenuItem<StaStaticDns2MenuItem>(index);
    constructMenuItem<makeComponentArgs<MenuItem, ClearConfigAction,                 StaticText<TEXT_DELETECONFIG>>>(index);
    constructMenuItem<makeComponent<MenuItem,     StaticText<TEXT_BACK>,             PopScreenAction, StaticMenuItemIcon<&icons::back>>>();
}

std::string WifiStaConfigEntryMenu::text() const
{
    return fmt::format("Wifi Config {}", m_index + 1);
}

void WifiStaConfigEntryMenu::back()
{
    espgui::popScreen();
}

namespace {
void ClearConfigAction::triggered()
{
    auto &config = configs.wifi_configs[m_index];
    configs.reset_config(config.ssid);
    configs.reset_config(config.key);
    configs.reset_config(config.useStaticIp);
    configs.reset_config(config.staticIp);
    configs.reset_config(config.staticSubnet);
    configs.reset_config(config.staticGateway);
    configs.reset_config(config.useStaticDns);
    configs.reset_config(config.staticDns0);
    configs.reset_config(config.staticDns1);
    configs.reset_config(config.staticDns2);
}
} // namespace
