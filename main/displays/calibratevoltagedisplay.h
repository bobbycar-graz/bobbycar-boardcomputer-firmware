#pragma once

// local includes
#include "bobbymenudisplay.h"

class CalibrateVoltageDisplay : public BobbyMenuDisplay
{
public:
    CalibrateVoltageDisplay();

    std::string title() const override;

    void back() override;
};
