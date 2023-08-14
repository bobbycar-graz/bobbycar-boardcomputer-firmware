#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class LedstripOtaAnimationChangeMenu : public BobbyMenuDisplay
{
public:
    LedstripOtaAnimationChangeMenu();

    std::string title() const override;
    void back() override;
};
