#pragma once

// 3rdparty lib includes
#include <widgets/label.h>

// local includes
#include "bobbydisplay.h"

class XYDebugDisplay : public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    XYDebugDisplay();
    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;

private:
    typedef struct {
        uint8_t x;
        uint8_t y;
    } cursor_pos_t;

    cursor_pos_t m_current_cursor{0, 0};
    cursor_pos_t m_last_cursor{0, 0};
    espgui::Label m_labelCoordinates;
};
