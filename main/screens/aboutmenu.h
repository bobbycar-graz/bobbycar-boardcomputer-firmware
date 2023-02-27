#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class AboutMenu : public BobbyMenuDisplay
{
public:
    AboutMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
