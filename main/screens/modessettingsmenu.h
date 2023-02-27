#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class ModesSettingsMenu : public BobbyMenuDisplay
{
public:
    ModesSettingsMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
