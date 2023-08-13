#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class MickModeSettingsMenu : public BobbyMenuDisplay
{
public:
    MickModeSettingsMenu();

    std::string title() const override;

    void back() override;
};
