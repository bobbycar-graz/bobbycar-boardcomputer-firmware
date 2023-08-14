#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class GarageMenu : public BobbyMenuDisplay
{
public:
    GarageMenu();

    std::string title() const override;

    void back() override;
};
