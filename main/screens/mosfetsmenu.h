#pragma once

#ifdef FEATURE_MOSFETS

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class MosfetsMenu : public BobbyMenuDisplay
{
public:
    MosfetsMenu();

    std::string text() const override;

    void back() override;
};
} // namespace bobby
#endif
