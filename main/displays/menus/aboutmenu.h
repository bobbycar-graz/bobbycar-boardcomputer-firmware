#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "esptexthelpers.h"
#include "texts.h"

// forward declares
class SettingsMenu;

namespace {
constexpr char TEXT_VERSION[] = "Version: 1.0";
} // namespace

class AboutMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_ABOUT>,
    public espgui::BackActionInterface<espgui::SwitchScreenAction<SettingsMenu>>
{
public:
    AboutMenu();
};
