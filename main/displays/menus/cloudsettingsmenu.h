#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class CloudSettingsMenu : public BobbyMenuDisplay
{
public:
    CloudSettingsMenu();

    std::string title() const override;

    void back() override;
};
