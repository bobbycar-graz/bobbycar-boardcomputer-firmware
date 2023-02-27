#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class ControllerHardwareSettingsMenu : public BobbyMenuDisplay
{
public:
    ControllerHardwareSettingsMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
