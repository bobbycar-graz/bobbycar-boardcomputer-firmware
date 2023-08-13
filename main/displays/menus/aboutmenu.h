#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class AboutMenu : public BobbyMenuDisplay
{
public:
    AboutMenu();

    std::string title() const override;

    void back() override;
};
