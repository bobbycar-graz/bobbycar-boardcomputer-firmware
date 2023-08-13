#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class TempomatModeSettingsMenu : public BobbyMenuDisplay
{
public:
    TempomatModeSettingsMenu();

    std::string title() const override;

    void back() override;
};
