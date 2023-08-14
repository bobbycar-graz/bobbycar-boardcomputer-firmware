#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class WifiStaScanMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;

public:
    WifiStaScanMenu();

    std::string title() const override;
    void back() override;

    void start() override;
    void update() override;

private:
    bool m_wasScanning;
};
