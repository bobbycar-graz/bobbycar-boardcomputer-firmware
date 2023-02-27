#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class CloudSettingsMenu : public BobbyMenuDisplay
{
public:
    CloudSettingsMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
