#pragma once

// 3rdparty lib includes

// local includes
#include "menudisplay.h"
#include "texts.h"

class MotortestModeSettingsMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_MOTORTESTMODESETTINGS>
{
public:
    MotortestModeSettingsMenu();

    void back() override;
};
