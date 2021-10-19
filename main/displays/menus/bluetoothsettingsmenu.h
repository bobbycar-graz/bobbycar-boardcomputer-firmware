#pragma once

// local includes
#include "changevaluedisplay.h"
#ifdef FEATURE_BLUETOOTH
#include "changevaluedisplay_bluetoothmode.h"
#endif
#include "menudisplay.h"
#include "menuitem.h"
#include "actions/dummyaction.h"
#ifdef FEATURE_BLUETOOTH
#include "actions/bluetoothbeginaction.h"
#include "actions/bluetoothbeginmasteraction.h"
#include "actions/bluetoothflushaction.h"
#include "actions/bluetoothendaction.h"
#include "actions/bluetoothdisconnectaction.h"
#endif
#include "actions/switchscreenaction.h"
#include "bluetoothtexthelpers.h"
#include "accessors/settingsaccessors.h"
#include "icons/back.h"
#include "texts.h"

// forward declares
class SettingsMenu;

#ifdef FEATURE_BLUETOOTH
class BluetoothSettingsMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_BLUETOOTHSETTINGS>,
    public espgui::BackActionInterface<espgui::SwitchScreenAction<SettingsMenu>>
{
public:
    BluetoothSettingsMenu();
};
#endif
