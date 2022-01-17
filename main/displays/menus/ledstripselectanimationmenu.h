#pragma once

// Local includes
#include "displays/bobbymenudisplay.h"

class LedstripSelectAnimationMenu : public BobbyMenuDisplay
{
public:
    LedstripSelectAnimationMenu();

    std::string text() const override;

    void back() override;
};
