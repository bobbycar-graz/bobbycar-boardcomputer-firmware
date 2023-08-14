#pragma once

#ifdef FEATURE_MOSFETS

// local includes
#include "displays/bobbymenudisplay.h"

class MosfetsMenu : public BobbyMenuDisplay
{
public:
    MosfetsMenu();

    std::string title() const override;

    void back() override;
};
#endif
