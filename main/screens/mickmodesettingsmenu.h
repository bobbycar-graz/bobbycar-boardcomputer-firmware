#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class MickModeSettingsMenu : public BobbyMenuDisplay
{
public:
    MickModeSettingsMenu();

    std::string text() const override;

    void back() override;
};
} // namespace bobby
