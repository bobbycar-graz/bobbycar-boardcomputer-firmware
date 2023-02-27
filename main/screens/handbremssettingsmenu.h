#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class HandbremsSettingsMenu : public BobbyMenuDisplay
{
public:
    HandbremsSettingsMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
