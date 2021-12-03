#pragma once

// local includes
#include "menudisplay.h"
#include "texts.h"

using namespace espgui;

class InvertMenu :
    public MenuDisplay,
    public StaticText<TEXT_SETINVERTED>
{
public:
    InvertMenu();
    void back() override;
};
