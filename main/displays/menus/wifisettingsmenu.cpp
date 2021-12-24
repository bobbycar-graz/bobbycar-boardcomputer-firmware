#include "wifisettingsmenu.h"

// 3rdparty lib includes
#include "actions/switchscreenaction.h"
#include "icons/back.h"

// local includes
#include "utilities/utils.h"
#include "displays/menus/genericwifisettingsmenu.h"
#include "displays/menus/stationwifisettingsmenu.h"
#include "displays/menus/accesspointwifisettingsmenu.h"
#include "displays/menus/settingsmenu.h"
#include "globals.h"

using namespace espgui;

#ifdef FEATURE_DNS_NS
class ResendDNSRequest : public virtual ActionInterface
{
public:
    void triggered() override { dns_lastIpAddress_v4 = "---"; dns_lastIpAddress_v6 = "---"; dns_lastIpAddress_v6_global = "---"; }
};
#endif

WifiSettingsMenu::WifiSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GENERICWIFISETTINGS>,     SwitchScreenAction<GenericWifiSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STATIONWIFISETTINGS>,     SwitchScreenAction<StationWifiSettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ACCESSPOINTWIFISETTINGS>, SwitchScreenAction<AccessPointWifiSettingsMenu>>>();
#ifdef FEATURE_DNS_NS
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_RESEND_DNS>,              ResendDNSRequest>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                    SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void WifiSettingsMenu::back()
{
    switchScreen<SettingsMenu>();
}
