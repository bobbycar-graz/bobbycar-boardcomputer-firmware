#pragma once

// system includes
#include <cstdint>

// 3rdparty lib includes
#include <tftinterface.h>

// local includes
#include "guihelpers/bobbydisplay.h"

namespace bobby {

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

} // namespace bobby
