#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class SelectModeMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;

public:
    SelectModeMenu();

    std::string title() const override;

    void start() override;
    void back() override;
};
} // namespace bobby
