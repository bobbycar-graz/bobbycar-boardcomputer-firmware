#pragma once

// Local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class LedstripSelectBlinkMenu : public BobbyMenuDisplay
{
public:
    LedstripSelectBlinkMenu();

    std::string title() const override;
    void back() override;
};

} // namespace bobby
