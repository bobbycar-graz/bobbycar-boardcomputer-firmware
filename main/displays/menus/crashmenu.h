#pragma once

// local includes
#include "menudisplay.h"
#include "texts.h"

using namespace espgui;

class CrashMenu :
    public MenuDisplay,
    public StaticText<TEXT_CRASHMENU>
{
public:
    CrashMenu();
    void back() override;
};
