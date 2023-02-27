#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class EspNowMenu : public BobbyMenuDisplay
{
public:
    EspNowMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
