#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class TimersMenu : public BobbyMenuDisplay
{
public:
    TimersMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
