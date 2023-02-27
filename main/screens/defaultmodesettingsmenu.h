#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class DefaultModeSettingsMenu : public BobbyMenuDisplay
{
public:
    DefaultModeSettingsMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
