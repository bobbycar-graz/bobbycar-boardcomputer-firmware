#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class SelectBuildMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;

public:
    SelectBuildMenu();

    std::string title() const override;

    void update() override;
    void back() override;

private:
    void buildMenuFromJson();
};
