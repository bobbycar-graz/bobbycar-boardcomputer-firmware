#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class BoardcomputerHardwareSettingsMenu : public BobbyMenuDisplay
{
public:
    BoardcomputerHardwareSettingsMenu();

    std::string title() const override;

    void back() override;
};
