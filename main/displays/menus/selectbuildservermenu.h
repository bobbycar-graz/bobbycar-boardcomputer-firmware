#pragma once

// 3rdparty lib includes
#include <menudisplay.h>
#include <icons/back.h>

// local includes
#include "texts.h"

#ifdef FEATURE_OTA

class SelectBuildServerMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_SELECTBUILDSERVERMENU>
{
public:
    SelectBuildServerMenu();

    void back() override;
};
#endif
