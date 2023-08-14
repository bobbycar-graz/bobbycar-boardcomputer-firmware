#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class LimitsSettingsMenu : public BobbyMenuDisplay
{
public:
    LimitsSettingsMenu();

    std::string title() const override;

    void back() override;
};
