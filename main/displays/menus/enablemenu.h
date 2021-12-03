#pragma once

// local includes
#include "menudisplay.h"
#include "texts.h"

using namespace espgui;

class EnableMenu :
    public MenuDisplay,
    public StaticText<TEXT_SETENABLED>
{
public:
    EnableMenu();
    void back() override;
};
