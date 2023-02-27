#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class SetupQuickActionsMenu : public BobbyMenuDisplay
{
public:
    SetupQuickActionsMenu();

    std::string text() const override;

    void back() override;
};
} // namespace bobby
