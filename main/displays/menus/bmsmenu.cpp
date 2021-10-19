#include "bmsmenu.h"

using namespace espgui;

#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
BmsMenu::BmsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CONNECTBMS>,       BluetoothConnectBmsAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DISCONNECTBMS>,    BluetoothDisconnectAction>>();
    constructMenuItem<makeComponent<MenuItem, BluetoothHasClientText,            DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TURNONCHARGE>,     BmsTurnOnChargeAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TURNOFFCHARGE>,    BmsTurnOffChargeAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TURNONDISCHARGE>,  BmsTurnOnDischargeAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TURNOFFDISCHARGE>, BmsTurnOffDischargeAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,             SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}
#endif
