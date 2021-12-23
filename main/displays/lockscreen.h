#pragma once

// system includes
#include <array>

// local includes
#include "display.h"
#include "widgets/label.h"
#include "modes/ignoreinputmode.h"

#ifdef LOCKSCREEN_PLUGIN
#include "hardware/ledstrip.h"
#endif

class Lockscreen : public espgui::Display
{
    using Base = espgui::Display;

    static constexpr auto boxWidth = 35;
    static constexpr auto boxHeight = 50;
    static constexpr auto spacing = 20;

public:
    void start() override;
    void initScreen() override;
    void update() override;
    void redraw() override;
    void stop() override;

    void confirm() override;
    void back() override;
    void rotate(int offset) override;

private:
    void drawRect(int index, int offset, uint32_t color) const;

    std::array<espgui::Label, 4> m_labels {{
        espgui::Label{spacing, 100}, // boxWidth, boxHeight
        espgui::Label{spacing*2+boxWidth, 100}, // boxWidth, boxHeight
        espgui::Label{spacing*3+boxWidth*2, 100}, // boxWidth, boxHeight
        espgui::Label{spacing*4+boxWidth*3, 100} // boxWidth, boxHeight
    }};

    std::array<int8_t, 4> m_numbers;

    uint8_t m_currentIndex{};

    bool m_pressed;
    bool m_back_pressed;
    int m_rotated;

    ModeInterface *m_oldMode;
    IgnoreInputMode m_mode{0, bobbycar::protocol::ControlType::FieldOrientedControl, bobbycar::protocol::ControlMode::Speed};
};
