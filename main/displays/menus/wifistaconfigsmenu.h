#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class WifiStaConfigsMenu : public BobbyMenuDisplay
{
public:
    WifiStaConfigsMenu();

    std::string title() const override;
    void back() override;
};
