#pragma once

// system includes
#include <cstdint>

// local includes
#include "bobbydisplay.h"

class StarfieldDisplay : public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    StarfieldDisplay();

    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;
    void stop() override;

    void buttonPressed(espgui::Button button) override;

private:
    static constexpr auto NSTARS = 1024;

    uint8_t sx[NSTARS] = {};
    uint8_t sy[NSTARS] = {};
    uint8_t sz[NSTARS] = {};

    uint8_t za, zb, zc, zx;
};
