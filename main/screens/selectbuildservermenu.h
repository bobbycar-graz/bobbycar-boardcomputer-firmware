#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class SelectBuildServerMenu : public BobbyMenuDisplay
{
public:
    SelectBuildServerMenu();

    std::string text() const override;

    void back() override;
};
} // namespace bobby
