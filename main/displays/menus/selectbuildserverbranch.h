#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class SelectBuildserverBranchMenu : public BobbyMenuDisplay
{
    using Base = espgui::MenuDisplay;
public:
    SelectBuildserverBranchMenu();

    std::string text() const override;

    void buildMenuRequestError(std::string error);
    void update() override;
    void back() override;
};
