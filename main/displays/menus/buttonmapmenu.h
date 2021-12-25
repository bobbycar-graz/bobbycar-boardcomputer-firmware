#pragma once

// 3rd party includes
#include <menudisplay.h>

// local includes
#include "texts.h"

class ButtonMapMenu :
        public espgui::MenuDisplay,
        public espgui::StaticText<TEXT_BUTTONMAPMENU>
{
public:
    ButtonMapMenu();
    void back() override;
};
