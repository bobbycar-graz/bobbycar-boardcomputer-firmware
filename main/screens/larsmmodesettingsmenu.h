#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class LarsmModeSettingsMenu : public BobbyMenuDisplay
{
public:
    LarsmModeSettingsMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
