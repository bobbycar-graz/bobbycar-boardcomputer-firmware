#pragma once

// local includes
#include "displays/menudisplaywithtime.h"

class LedstripMenu : public bobbygui::MenuDisplayWithTime
{
public:
    LedstripMenu();

    std::string title() const override;

    void back() override;
};
