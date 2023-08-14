#pragma once

// Local includes
#include "displays/bobbymenudisplay.h"

class BatteryDebugMenu : public BobbyMenuDisplay
{
public:
    BatteryDebugMenu();

    std::string title() const override;

    void back() override;
};
