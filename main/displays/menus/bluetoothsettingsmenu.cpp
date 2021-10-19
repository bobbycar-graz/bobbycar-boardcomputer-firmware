#include "bluetoothsettingsmenu.h"

using namespace espgui;

#ifdef FEATURE_BLUETOOTH
using AutoBluetoothModeChangeDisplay = makeComponent<
    ChangeValueDisplay<BluetoothMode>,
    StaticText<TEXT_AUTOBLUETOOTHMODE>,
    AutoBluetoothModeAccessor,
    BackActionInterface<SwitchScreenAction<BluetoothSettingsMenu>>,
    SwitchScreenAction<BluetoothSettingsMenu>
>;

BluetoothSettingsMenu::BluetoothSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, BluetoothAvailableText,                 DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BluetoothHasClientText,                 DisabledColor, DummyAction>>();
//    constructMenuItem<makeComponent<MenuItem, BluetoothConnectedText,                 DisabledColor, DummyAction>>(); // crashes
    constructMenuItem<makeComponent<MenuItem, BluetoothIsReadyText,                   DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BluetoothIsReadyMasterText,             DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHBEGIN>,        BluetoothBeginAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHBEGINMASTER>,  BluetoothBeginMasterAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHFLUSH>,        BluetoothFlushAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHEND>,          BluetoothEndAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHDISCONNECT>,   BluetoothDisconnectAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_AUTOBLUETOOTHMODE>,     SwitchScreenAction<AutoBluetoothModeChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                  SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}
#endif
