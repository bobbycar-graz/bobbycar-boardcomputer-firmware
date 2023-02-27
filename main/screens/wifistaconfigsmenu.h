#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class WifiStaConfigsMenu : public BobbyMenuDisplay
{
public:
    WifiStaConfigsMenu();

    std::string text() const override;
    void back() override;
};

} // namespace bobby
