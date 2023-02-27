#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class GraphsMenu : public BobbyMenuDisplay
{
public:
    GraphsMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
