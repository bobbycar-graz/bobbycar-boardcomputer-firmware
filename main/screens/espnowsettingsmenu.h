#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class EspNowSettingsMenu : public BobbyMenuDisplay
{
public:
    EspNowSettingsMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
