#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class DebugMenu : public BobbyMenuDisplay
{
public:
    DebugMenu();

    std::string title() const override;

    void back() override;
};
