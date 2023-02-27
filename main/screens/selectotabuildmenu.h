#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class SelectBuildMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;

public:
    SelectBuildMenu();

    std::string text() const override;

    void update() override;
    void back() override;

private:
    void buildMenuFromJson();
};

} // namespace bobby
