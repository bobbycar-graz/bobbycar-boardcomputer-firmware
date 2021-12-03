#pragma once

// local includes
#include "menudisplay.h"
#include "texts.h"

using namespace espgui;

class TimersMenu :
    public MenuDisplay,
    public StaticText<TEXT_TIMERS>
{
public:
    TimersMenu();
    void back() override;
};
