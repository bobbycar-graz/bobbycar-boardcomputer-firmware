#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class CrashMenu : public BobbyMenuDisplay
{
public:
    CrashMenu();

    std::string title() const override;

    void back() override;
};

} // namespace bobby
