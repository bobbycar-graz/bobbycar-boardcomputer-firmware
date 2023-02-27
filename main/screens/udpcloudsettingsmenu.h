#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class UdpCloudSettingsMenu : public BobbyMenuDisplay
{
public:
    UdpCloudSettingsMenu();

    std::string text() const override;

    void back() override;
};
} // namespace bobby
