#pragma once

#ifdef FEATURE_ESPNOW

#include <accessorinterface.h>
#include <actioninterface.h>
#include <espchrono.h>
#include <fmt/core.h>
#include <menudisplay.h>
#include <textinterface.h>
#include <texts.h>

#include "utilities/espnowfunctions.h"

using namespace espgui;

namespace espnowsettingsmenu {
} // namespace

class EspNowSettingsMenu :
        public espgui::MenuDisplay,
        public espgui::StaticText<TEXT_ESPNOW_SETTINGS>
{
public:
    EspNowSettingsMenu();
    void back() override;
};

#endif
