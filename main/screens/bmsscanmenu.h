#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class BMSScanMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;
public:
    BMSScanMenu();

    std::string text() const override;

    void start() override;
    void update() override;

    void back() override;

private:
    bool m_wasScanning{false};
};

} // namespace bobby
