#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class UdpCloudSettingsMenu : public BobbyMenuDisplay
{
public:
    UdpCloudSettingsMenu();

    std::string title() const override;

    void back() override;
};
