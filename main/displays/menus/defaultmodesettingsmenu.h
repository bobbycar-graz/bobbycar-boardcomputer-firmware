#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class DefaultModeSettingsMenu : public BobbyMenuDisplay
{
public:
    DefaultModeSettingsMenu();

    std::string title() const override;

    void back() override;
};
