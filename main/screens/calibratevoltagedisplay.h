#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class CalibrateVoltageDisplay : public BobbyMenuDisplay
{
public:
    CalibrateVoltageDisplay();

    std::string title() const override;

    void back() override;
};

} // namespace bobby
