#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class SelectBuildserverBranchMenu : public BobbyMenuDisplay
{
    using Base = espgui::MenuDisplay;
public:
    SelectBuildserverBranchMenu();

    std::string text() const override;

    void update() override;
    void back() override;
};
} // namespace bobby
