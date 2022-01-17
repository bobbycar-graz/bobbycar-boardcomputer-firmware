#pragma once

// Local includes
#include "displays/bobbymenudisplay.h"

class LedstripSelectBlinkMenu : public BobbyMenuDisplay
{
public:
    LedstripSelectBlinkMenu();

    std::string text() const override;
    void back() override;
};
