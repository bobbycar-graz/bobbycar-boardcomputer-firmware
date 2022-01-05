#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class SetupQuickActionsMenu : public BobbyMenuDisplay
{
public:
    SetupQuickActionsMenu();

    std::string text() const override;

    void back() override;
};
