#pragma once

// local includes
#include "displays/menudisplaywithtime.h"

class FeatureFlagsMenu : public bobbygui::MenuDisplayWithTime
{
public:
    FeatureFlagsMenu();

    std::string text() const override;

    void back() override;
};
