#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class UdpCloudSettingsMenu : public BobbyMenuDisplay
{
public:
    UdpCloudSettingsMenu();

    std::string text() const override;

    void back() override;
};
