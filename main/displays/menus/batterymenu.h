#pragma once

// Local includes
#include "menudisplay.h"
#include "utils.h"
#include "menuitem.h"
#include "icons/back.h"
#include "icons/settings.h"
#include "texts.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "mainmenu.h"
#include "battery.h"
#include "selectbatterytypemenu.h"
#include "displays/calibratevoltagedisplay.h"

// Helper
class currentBatteryStatus : public virtual TextInterface { public: std::string text() const override { return getBatteryPercentageString(); } };

class BatteryMenu;
class CalibrateVoltageDisplay;

class BatteryMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_BATTERY>,
    public espgui::BackActionInterface<espgui::SwitchScreenAction<MainMenu>>
{
public:
    BatteryMenu();
};
