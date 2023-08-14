#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class LockscreenSettingsMenu : public BobbyMenuDisplay
{
public:
    LockscreenSettingsMenu();

    std::string title() const override;

    void back() override;
};
