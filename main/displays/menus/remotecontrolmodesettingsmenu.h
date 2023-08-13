#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class RemoteControlModeSettingsMenu : public BobbyMenuDisplay
{
public:
    RemoteControlModeSettingsMenu();

    std::string title() const override;

    void back() override;
};
