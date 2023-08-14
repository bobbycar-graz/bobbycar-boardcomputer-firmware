#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class InvertMenu : public BobbyMenuDisplay
{
public:
    InvertMenu();

    std::string title() const override;

    void back() override;
};
