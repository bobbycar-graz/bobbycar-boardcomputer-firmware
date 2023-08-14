#pragma once

// local includes
#include "displays/menudisplaywithtime.h"

class SettingsMenu : public bobbygui::MenuDisplayWithTime
{
public:
    SettingsMenu();

    std::string title() const override;

    void back() override;
};
