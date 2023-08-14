#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class WifiApSettingsMenu : public BobbyMenuDisplay
{
public:
    WifiApSettingsMenu();

    std::string title() const override;
    void back() override;
};
