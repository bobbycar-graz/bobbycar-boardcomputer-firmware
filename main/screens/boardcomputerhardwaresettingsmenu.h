#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class BoardcomputerHardwareSettingsMenu : public BobbyMenuDisplay
{
public:
    BoardcomputerHardwareSettingsMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
