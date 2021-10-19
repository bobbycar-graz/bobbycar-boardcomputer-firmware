#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "menuitem.h"
#ifdef FEATURE_BLUETOOTH
#include "actions/bluetoothconnectbmsaction.h"
#include "actions/bluetoothdisconnectaction.h"
#endif
#include "actions/dummyaction.h"
#include "actions/bmsturnonchargeaction.h"
#include "actions/bmsturnoffchargeaction.h"
#include "actions/bmsturnondischargeaction.h"
#include "actions/bmsturnoffdischargeaction.h"
#include "actions/switchscreenaction.h"
#include "bluetoothtexthelpers.h"
#include "icons/back.h"
#include "texts.h"

//forward declares
class MainMenu;

#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
class BmsMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_BMS>,
    public espgui::BackActionInterface<espgui::SwitchScreenAction<MainMenu>>
{
public:
    BmsMenu();
};
#endif
