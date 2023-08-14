#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class TimersMenu : public BobbyMenuDisplay
{
public:
    TimersMenu();

    std::string title() const override;

    void back() override;
};
