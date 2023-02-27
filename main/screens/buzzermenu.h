#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class BuzzerMenu : public BobbyMenuDisplay
{
public:
    BuzzerMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
