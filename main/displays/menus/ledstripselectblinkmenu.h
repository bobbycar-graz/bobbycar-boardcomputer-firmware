#pragma once

// Local includes
#include "displays/bobbymenudisplay.h"

class LedstripSelectBlinkMenu : public BobbyMenuDisplay
{
public:
    LedstripSelectBlinkMenu();

    std::string title() const override;
    void back() override;
};
