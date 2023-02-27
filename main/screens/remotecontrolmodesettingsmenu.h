#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class RemoteControlModeSettingsMenu : public BobbyMenuDisplay
{
public:
    RemoteControlModeSettingsMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
