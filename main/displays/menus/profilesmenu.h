#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class ProfilesMenu : public BobbyMenuDisplay
{
public:
    ProfilesMenu();

    std::string title() const override;

    void back() override;
};
