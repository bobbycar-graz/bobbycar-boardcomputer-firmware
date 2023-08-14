#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class TaskmanagerMenu : public BobbyMenuDisplay
{
public:
    TaskmanagerMenu();

    std::string title() const override;

    void back() override;
};
