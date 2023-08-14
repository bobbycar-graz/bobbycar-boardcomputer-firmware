#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class CrashMenu : public BobbyMenuDisplay
{
public:
    CrashMenu();

    std::string title() const override;

    void back() override;
};
