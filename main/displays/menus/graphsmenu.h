#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class GraphsMenu : public BobbyMenuDisplay
{
public:
    GraphsMenu();

    std::string title() const override;

    void back() override;
};
