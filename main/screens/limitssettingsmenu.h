#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class LimitsSettingsMenu : public BobbyMenuDisplay
{
public:
    LimitsSettingsMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
