#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class DemosMenu : public BobbyMenuDisplay
{
public:
    DemosMenu();

    std::string title() const override;

    void back() override;
};
