#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class BleSettingsMenu : public BobbyMenuDisplay
{
public:
    BleSettingsMenu();

    std::string text() const override;

    void back() override;
};
