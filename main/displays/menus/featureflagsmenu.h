#pragma once

// local includes
#include "displays/menudisplaywithtime.h"

class FeatureFlagsMenu : public bobbygui::MenuDisplayWithTime
{
    using Base = bobbygui::MenuDisplayWithTime;
public:
    FeatureFlagsMenu();
    void start() override;

    std::string title() const override;

    void back() override;
};
