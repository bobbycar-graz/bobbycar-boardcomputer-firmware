#pragma once

// local includes
#include "menudisplay.h"
#include "texts.h"

#ifdef FEATURE_OTA

class SelectBuildMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_SELECTBUILD>
{
    using Base = MenuDisplay;
public:
    SelectBuildMenu();
    void update() override;
    void back() override;
    void buildMenuFromJson();
    void buildMenuRequestError(std::string error);
};
#endif
