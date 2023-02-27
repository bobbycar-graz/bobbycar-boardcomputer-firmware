#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class WifiStaSettingsMenu : public BobbyMenuDisplay
{
public:
    WifiStaSettingsMenu();

    std::string text() const override;
    void back() override;
};
} // namespace bobby
