#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class ModesSettingsMenu : public BobbyMenuDisplay
{
public:
    ModesSettingsMenu();

    std::string title() const override;

    void back() override;
};
