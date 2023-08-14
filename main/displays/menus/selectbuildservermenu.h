#pragma once

// 3rdparty lib includes

// local includes
#include "displays/bobbymenudisplay.h"

class SelectBuildServerMenu : public BobbyMenuDisplay
{
public:
    SelectBuildServerMenu();

    std::string title() const override;

    void back() override;
};
