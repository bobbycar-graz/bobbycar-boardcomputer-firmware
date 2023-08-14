#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class BuzzerMenu : public BobbyMenuDisplay
{
public:
    BuzzerMenu();

    std::string title() const override;

    void back() override;
};
