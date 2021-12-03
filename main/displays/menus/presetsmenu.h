#pragma once

// local includes
#include "menudisplay.h"
#include "texts.h"

using namespace espgui;

class PresetsMenu :
    public MenuDisplay,
    public StaticText<TEXT_PRESETS>
{
public:
    PresetsMenu();
    void back() override;
};
