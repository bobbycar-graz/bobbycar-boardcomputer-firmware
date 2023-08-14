#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class WifiStaSettingsMenu : public BobbyMenuDisplay
{
public:
    WifiStaSettingsMenu();

    std::string title() const override;
    void back() override;
};
