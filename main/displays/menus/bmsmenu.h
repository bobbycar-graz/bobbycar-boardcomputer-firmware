#pragma once

#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
// local includes
#include "menudisplay.h"
#include "texts.h"

class BmsMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_BMS>
{
public:
    BmsMenu();
    void back() override;
};
#endif
