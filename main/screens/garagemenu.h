#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class GarageMenu : public BobbyMenuDisplay
{
public:
    GarageMenu();

    std::string title() const override;

    void back() override;
};

} // namespace bobby
