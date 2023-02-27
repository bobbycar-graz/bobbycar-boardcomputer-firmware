#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class TempomatModeSettingsMenu : public BobbyMenuDisplay
{
public:
    TempomatModeSettingsMenu();

    std::string text() const override;

    void back() override;
};
} // namespace bobby
