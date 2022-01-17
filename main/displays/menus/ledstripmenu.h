#pragma once

// local includes
#include "displays/menudisplaywithtime.h"

class LedstripMenu : public bobbygui::MenuDisplayWithTime
{
public:
    LedstripMenu();

    std::string text() const override;

    void back() override;
};
