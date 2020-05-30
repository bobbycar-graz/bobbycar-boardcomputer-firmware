#pragma once

#include <HardwareSerial.h>

#include "menudisplay.h"
#include "staticmenudefinition.h"
#include "utils.h"
#include "changevaluedisplay.h"
#include "actions/toggleboolaction.h"
#include "actions/switchscreenaction.h"
#include "checkboxicon.h"
#include "icons/wifi.h"
#include "icons/bluetooth.h"
#include "icons/hardware.h"
#include "icons/buzzer.h"
#include "icons/info.h"
#include "icons/back.h"
#include "texts.h"
#include "globals.h"
#include "settingsaccessors.h"

namespace {
class LimitsSettingsMenu;
class WifiSettingsMenu;
class BluetoothSettingsMenu;
class ModesSettingsMenu;
class ControllerHardwareSettingsMenu;
class BoardcomputerHardwareSettingsMenu;
class BuzzerMenu;
class AboutMenu;
class MainMenu;
}

namespace {
#ifdef GLUMP_CONTROLLER
class SettingsMenu;
using BluetoothModeChangeDisplay = makeComponent<
    ChangeValueDisplay<BluetoothMode>,
    StaticText<TEXT_BLUETOOTHMODE>,
    BluetoothModeAccessor,
    BackActionInterface<SwitchScreenAction<SettingsMenu>>,
    SwitchScreenAction<SettingsMenu>
>;

struct FrontLedAccessor : public RefAccessor<bool> { bool &getRef() const override { return front.command.led; } };
struct BackLedAccessor : public RefAccessor<bool> { bool &getRef() const override { return back.command.led; } };
#endif

class SettingsMenu :
    public MenuDisplay,
    public StaticText<TEXT_SETTINGS>,
    public BackActionInterface<SwitchScreenAction<MainMenu>>,
    public StaticMenuDefinition<
        makeComponent<MenuItem, StaticText<TEXT_LIMITSSETTINGS>,                SwitchScreenAction<LimitsSettingsMenu>>,
        makeComponent<MenuItem, StaticText<TEXT_WIFISETTINGS>,                  SwitchScreenAction<WifiSettingsMenu>, StaticMenuItemIcon<&icons::wifi>>,
        makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHSETTINGS>,             SwitchScreenAction<BluetoothSettingsMenu>, StaticMenuItemIcon<&icons::bluetooth>>,
#ifdef GLUMP_CONTROLLER
        makeComponent<MenuItem, StaticText<TEXT_MODESSETTINGS>,                 SwitchScreenAction<ModesSettingsMenu>>,
#endif
        makeComponent<MenuItem, StaticText<TEXT_CONTROLLERHARDWARESETTINGS>,    SwitchScreenAction<ControllerHardwareSettingsMenu>, StaticMenuItemIcon<&icons::hardware>>,
        makeComponent<MenuItem, StaticText<TEXT_BOARDCOMPUTERHARDWARESETTINGS>, SwitchScreenAction<BoardcomputerHardwareSettingsMenu>, StaticMenuItemIcon<&icons::hardware>>,
#endif
        makeComponent<MenuItem, StaticText<TEXT_BLUETOOTHMODE>,                 SwitchScreenAction<BluetoothModeChangeDisplay>>,
#ifdef FEATURE_BMS
        makeComponent<MenuItem, StaticText<TEXT_AUTOCONNECTBMS>,                ToggleBoolAction, CheckboxIcon, AutoConnectBmsAccessor>,
#endif
#ifdef GLUMP_CONTROLLER
        makeComponent<MenuItem, StaticText<TEXT_BUZZER>,                        SwitchScreenAction<BuzzerMenu>, StaticMenuItemIcon<&icons::buzzer>>,
        makeComponent<MenuItem, StaticText<TEXT_FRONTLED>,                      ToggleBoolAction, CheckboxIcon, FrontLedAccessor>,
        makeComponent<MenuItem, StaticText<TEXT_BACKLED>,                       ToggleBoolAction, CheckboxIcon, BackLedAccessor>,
#endif
        makeComponent<MenuItem, StaticText<TEXT_ABOUT>,                         SwitchScreenAction<AboutMenu>, StaticMenuItemIcon<&icons::info>>,
        makeComponent<MenuItem, StaticText<TEXT_BACK>,                          SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&icons::back>>
    >
{};
}
