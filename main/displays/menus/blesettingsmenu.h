#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class BleSettingsMenu : public BobbyMenuDisplay
{
public:
    BleSettingsMenu();

    std::string title() const override;

    void back() override;
};
