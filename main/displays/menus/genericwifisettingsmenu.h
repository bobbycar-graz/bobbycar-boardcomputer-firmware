#pragma once

// local includes
#include "menudisplay.h"
#include "changevaluedisplay.h"
#include "menuitem.h"
#include "actions/dummyaction.h"
#include "actions/toggleboolaction.h"
#include "actions/switchscreenaction.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "wifitexthelpers.h"
#include "wifiaccessors.h"
#include "texts.h"

// forward declares
namespace {
class GenericWifiSettingsMenu;
class WifiSettingsMenu;
} // namespace

namespace {
using WifiModeChangeScreen = makeComponent<
    ChangeValueDisplay<wifi_mode_t>,
    StaticText<TEXT_WIFICHANGEMODE>,
    WifiModeAccessor,
    BackActionInterface<SwitchScreenAction<GenericWifiSettingsMenu>>,
    SwitchScreenAction<GenericWifiSettingsMenu>
>;

//using WifiTxPowerChangeScreen = makeComponent<
//    ChangeValueDisplay<wifi_power_t>,
//    StaticText<TEXT_WIFICHANGETXPOWER>,
//    WifiTxPowerAccessor,
//    BackActionInterface<SwitchScreenAction<GenericWifiSettingsMenu>>,
//    SwitchScreenAction<GenericWifiSettingsMenu>
//>;

class GenericWifiSettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_GENERICWIFISETTINGS>,
    public BackActionInterface<SwitchScreenAction<WifiSettingsMenu>>
{
public:
    GenericWifiSettingsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, WifiStatusBitsText,                 DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, WifiChannelText,                    DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFICHANGEMODE>,    SwitchScreenAction<WifiModeChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFICHANGESLEEP>,   ToggleBoolAction, CheckboxIcon, WifiSleepAccessor>>();
        //constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_WIFICHANGETXPOWER>, SwitchScreenAction<WifiTxPowerChangeScreen>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,              SwitchScreenAction<WifiSettingsMenu>, StaticMenuItemIcon<&icons::back>>>();
    }
};
} // namespace
