#pragma once

// local includes
#include "displays/menudisplaywithtime.h"

class TimeSettingsMenu : public bobbygui::MenuDisplayWithTime
{
public:
    TimeSettingsMenu();

    std::string title() const override;

    void back() override;
};
