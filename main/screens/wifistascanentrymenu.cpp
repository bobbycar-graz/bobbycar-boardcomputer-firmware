#include "wifistascanentrymenu.h"

// system includes
#include <algorithm>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <espwifiutils.h>
#include <fmt/format.h>
#include <menuitem.h>
#include <richtextrenderer.h>
#include <screenmanager.h>
#include <strutils.h>

// local includes
#include "icons/back.h"
#include "newsettings.h"
#include "wifiguiutils.h"
#include "wifistaconfigentrymenu.h"
#include "wifistascanmenu.h"

namespace bobby {

using namespace std::string_literals;
using namespace espgui;

namespace {
constexpr const char * const TAG = "BOBBY";

constexpr char TEXT_SAVEASNEW[] = "Save as new wifi config";
constexpr char TEXT_BACK[] = "Back";

class SaveNewWifiConfigAction : public virtual ActionInterface
{
public:
    SaveNewWifiConfigAction(std::string &&ssid) : m_ssid{std::move(ssid)} {}
    SaveNewWifiConfigAction(std::string_view ssid) : m_ssid{ssid} {}

    void triggered() override;

private:
    const std::string m_ssid;
};
} // namespace

WifiStaScanEntryMenu::WifiStaScanEntryMenu(const wifi_ap_record_t &info) :
    m_info{info}
{
    const std::string_view ssid{reinterpret_cast<const char*>(m_info.ssid)};
    const bool configured = std::any_of(std::begin(configs.wifi_configs), std::end(configs.wifi_configs),
                                        [&ssid](const WiFiConfig &config){ return cpputils::stringEqualsIgnoreCase(config.ssid.value(), ssid); });

    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&sssid: &f{}{}", configured?"&4":"", richTextEscape(ssid)));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&sconfigured: &f{}{}", configured?"&2":"&1", configured?"yes":"no"));
    if (!configured && !ssid.empty())
        constructMenuItem<makeComponentArgs<MenuItem, SaveNewWifiConfigAction, StaticText<TEXT_SAVEASNEW>>>(ssid);
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&sbssid: &f{}", wifi_stack::toString(wifi_stack::mac_t{m_info.bssid})));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&schannel: &f{}", m_info.primary));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&ssecond: &f{}", [&](){
        switch (m_info.second)
        {
        case WIFI_SECOND_CHAN_NONE:  return "NONE"s;
        case WIFI_SECOND_CHAN_ABOVE: return "ABOVE"s;
        case WIFI_SECOND_CHAN_BELOW: return "BELOW"s;
        default: return std::to_string(m_info.second);
        }
    }()));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&srssi: &f{}{}dB", rssiToColor(m_info.rssi), m_info.rssi));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&sauthmode: &f{}", wifi_stack::toString(m_info.authmode)));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&spairwise cipher: &f{}", wifi_stack::toString(m_info.pairwise_cipher)));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&sgroup cipher: &f{}", wifi_stack::toString(m_info.group_cipher)));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&sant: &f{}", [&](){
        switch (m_info.ant)
        {
        case WIFI_ANT_ANT0: return "ANT0"s;
        case WIFI_ANT_ANT1: return "ANT1"s;
        default: return std::to_string(m_info.ant);
        }
    }()));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&smodes:&f{}{}{}", m_info.phy_11b?" b":"", m_info.phy_11g?" g":"", m_info.phy_11n?" n":""));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&slong range: &f{}{}", m_info.phy_lr?"&2":"&1", m_info.phy_lr?"yes":"no"));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&swps: &f{}{}", m_info.wps?"&2":"&1", m_info.wps?"yes":"no"));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&sftm responder: &f{}{}", m_info.ftm_responder?"&2":"&1", m_info.ftm_responder?"yes":"no"));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&sftm initiator: &f{}{}", m_info.ftm_initiator?"&2":"&1", m_info.ftm_initiator?"yes":"no"));
    constructMenuItem<makeComponentArgs<MenuItem, ChangeableText, DummyAction>>(fmt::format("&scountry: &f{} ({}-{}, {}dB, {})",
                                                                                            std::string_view(m_info.country.cc, m_info.country.cc[0]=='\0' ? 0 : (m_info.country.cc[1]=='\0' ? 1 : (m_info.country.cc[2]=='\0' ? 2 : 3))),
                                                                                            m_info.country.schan,
                                                                                            m_info.country.nchan,
                                                                                            m_info.country.max_tx_power,
                                                                                            [&](){
                                                                                                switch (m_info.country.policy)
                                                                                                {
                                                                                                case WIFI_COUNTRY_POLICY_AUTO:   return "AUTO"s;
                                                                                                case WIFI_COUNTRY_POLICY_MANUAL: return "MANUAL"s;
                                                                                                default: return std::to_string(m_info.country.policy);
                                                                                                }
                                                                                            }()));
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string WifiStaScanEntryMenu::text() const
{
    return fmt::format("Scan entry {}", richTextEscape(std::string_view{reinterpret_cast<const char*>(m_info.ssid)}));
}

void WifiStaScanEntryMenu::back()
{
    popScreen();
}

namespace {

void SaveNewWifiConfigAction::triggered()
{
    const auto iter = std::find_if(std::begin(configs.wifi_configs), std::end(configs.wifi_configs),
                                   [](const WiFiConfig &config){ return config.ssid.value().empty() && config.key.value().empty(); });
    if (iter == std::end(configs.wifi_configs))
    {
        ESP_LOGE(TAG, "no free wifi config slot found!");
        return;
    }

    if (const auto result = configs.write_config(iter->ssid, m_ssid); !result)
    {
        ESP_LOGE(TAG, "Could not save ssid: %.*s", result.error().size(), result.error().data());
        return;
    }

    pushScreen<WifiStaConfigEntryMenu>(std::distance(std::begin(configs.wifi_configs), iter));
}

} // namespace
} // namespace bobby
