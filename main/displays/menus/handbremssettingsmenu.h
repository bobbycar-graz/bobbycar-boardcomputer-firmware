#pragma once

#include "menudisplay.h"
#include "texts.h"

class HandbremsSettingsMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_DEBUG>
{
public:
    HandbremsSettingsMenu();
    void back() override;
};
