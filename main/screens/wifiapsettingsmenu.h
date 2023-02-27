#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class WifiApSettingsMenu : public BobbyMenuDisplay
{
public:
    WifiApSettingsMenu();

    std::string text() const override;
    void back() override;
};
} // namespace bobby
