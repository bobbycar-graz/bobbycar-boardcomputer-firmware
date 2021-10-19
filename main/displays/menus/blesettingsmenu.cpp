#include "blesettingsmenu.h"

using namespace espgui;

#ifdef FEATURE_BLE
BleSettingsMenu::BleSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLEENABLED>, ToggleBoolAction, CheckboxIcon, BleEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, BleServerPeerDevicesText,    DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BleCharacSubscribedText,     DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,       SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}
#endif
