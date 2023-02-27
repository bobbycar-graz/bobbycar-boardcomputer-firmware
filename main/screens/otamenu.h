#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class OtaMenu : public BobbyMenuDisplay
{
public:
    OtaMenu();

    std::string text() const override;

    void back() override;
};
} // namespace bobby
