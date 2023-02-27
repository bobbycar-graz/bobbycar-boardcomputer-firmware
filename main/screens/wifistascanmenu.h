#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class WifiStaScanMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;

public:
    WifiStaScanMenu();

    std::string text() const override;
    void back() override;

    void start() override;
    void update() override;

private:
    bool m_wasScanning{false};
};
} // namespace bobby
