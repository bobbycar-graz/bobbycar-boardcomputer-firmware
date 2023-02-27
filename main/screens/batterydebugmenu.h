#pragma once

// Local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class BatteryDebugMenu : public BobbyMenuDisplay
{
public:
    BatteryDebugMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
