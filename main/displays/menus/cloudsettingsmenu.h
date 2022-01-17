#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class CloudSettingsMenu : public BobbyMenuDisplay
{
public:
    CloudSettingsMenu();

    std::string text() const override;

    void back() override;
};
